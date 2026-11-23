#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "analyzer.h"
#include "playersData.h"

float calculatePerformanceIndex(ICCPlayerRole role, float battingAverage, float strikeRate, int wickets, float economyRate) {
    switch(role) {
        case PLAYER_ROLE_BATSMAN:
            return (battingAverage * strikeRate) / 100.0f;
        case PLAYER_ROLE_BOWLER:
            return ((float)wickets * 2.0f) + (100.0f - economyRate);
        case PLAYER_ROLE_ALL_ROUNDER:
            return ((battingAverage * strikeRate) / 100.0f) + ((float)wickets * 2.0f);
        default:
            return 0.0f;
    }
}

const char* getRoleName(ICCPlayerRole role) {
    switch(role) {
        case PLAYER_ROLE_BATSMAN:
            return "Batsman";
        case PLAYER_ROLE_BOWLER:
            return "Bowler";
        case PLAYER_ROLE_ALL_ROUNDER:
            return "All-Rounder";
        default:
            return "Unknown";
    }
}

bool parseRoleChoice(int roleChoice, ICCPlayerRole* outRole) {
    if(outRole == NULL) return false;
    switch(roleChoice) {
        case ROLE_CHOICE_BATSMAN:
            *outRole = PLAYER_ROLE_BATSMAN;
            return true;
        case ROLE_CHOICE_BOWLER:
            *outRole = PLAYER_ROLE_BOWLER;
            return true;
        case ROLE_CHOICE_ALL_ROUNDER:
            *outRole = PLAYER_ROLE_ALL_ROUNDER;
            return true;
        default:
            return false;
    }
}

int comparePlayerByPerfIndexDesc(const void* firstPlayerPtr, const void* secondPlayerPtr) {
    const ICCPlayer* firstPlayer = (const ICCPlayer*)firstPlayerPtr;
    const ICCPlayer* secondPlayer = (const ICCPlayer*)secondPlayerPtr;

    if (firstPlayer->performanceIndex > secondPlayer->performanceIndex) return 1;
    if (firstPlayer->performanceIndex < secondPlayer->performanceIndex) return -1;

    return firstPlayer->id - secondPlayer->id;
}

static ICCTeam* findTeamById(ICCSystemContext* self, int teamId) {
    if(self == NULL || self->teamCount <= 0) return NULL;

    int lowIndex = 0;
    int highIndex = self->teamCount - 1;

    while(lowIndex <= highIndex) {
        int middleIndex = lowIndex + (highIndex - lowIndex) / 2;
        if(self->teams[middleIndex].id == teamId) {
            return &self->teams[middleIndex];
        } else if(self->teams[middleIndex].id < teamId) {
            lowIndex = middleIndex + 1;
        } else {
            highIndex = middleIndex - 1;
        }
    }

    return NULL;
}

static ICCTeam* findTeamByName(ICCSystemContext* self, const char* teamName) {
    if(self == NULL || teamName == NULL) return NULL;

    for(int teamIndex = 0; teamIndex < self->teamCount; teamIndex++) {
        if(strcmp(self->teams[teamIndex].name, teamName) == 0) {
            return &self->teams[teamIndex];
        }
    }

    return NULL;
}

static bool isPlayerIdExists(ICCSystemContext* self, int playerId) {
    if(self == NULL) return false;

    for(int teamIndex = 0; teamIndex < self->teamCount; teamIndex++) {
        for(int roleIndex = 0; roleIndex < TOTAL_ICC_PLAYERS_ROLES; roleIndex++) {
            SortedListNode* currentNode = self->teams[teamIndex].players[roleIndex]->head;
            while(currentNode != NULL) {
                ICCPlayer* currentPlayer = (ICCPlayer*)currentNode->data;
                if(currentPlayer->id == playerId) return true;
                currentNode = currentNode->next;
            }
        }
    }

    return false;
}

static bool addPlayerToTeam(ICCSystemContext* self, int teamId, const ICCPlayer* player) {
    if(self == NULL || player == NULL) return false;

    ICCTeam* team = self->findTeamById(self, teamId);
    if(team == NULL) return false;

    if(player->role < 0 || player->role >= TOTAL_ICC_PLAYERS_ROLES) return false;

    ICCPlayer newPlayer = *player;
    strncpy(newPlayer.teamName, team->name, NAME_BUFFER_SIZE - 1);
    newPlayer.teamName[NAME_BUFFER_SIZE - 1] = '\0';
    newPlayer.performanceIndex = calculatePerformanceIndex(
        newPlayer.role, newPlayer.battingAverage, newPlayer.strikeRate,
        newPlayer.wickets, newPlayer.economyRate
    );

    bool isInserted = team->players[newPlayer.role]->insert(team->players[newPlayer.role], &newPlayer);
    if(!isInserted) return false;

    team->totalPlayers++;
    if(newPlayer.role == PLAYER_ROLE_BATSMAN || newPlayer.role == PLAYER_ROLE_ALL_ROUNDER) {
        team->totalBattingStrikeRate += newPlayer.strikeRate;
        team->battingCount++;
        team->averageBattingStrikeRate = team->totalBattingStrikeRate / (float)team->battingCount;
    }

    return true;
}

ICCSystemContext* initICCSystem(void) {
    ICCSystemContext* systemContext = (ICCSystemContext*)malloc(sizeof(ICCSystemContext));
    if(systemContext == NULL) return NULL;

    systemContext->teamCount = teamCount;
    systemContext->findTeamById = findTeamById;
    systemContext->findTeamByName = findTeamByName;
    systemContext->addPlayerToTeam = addPlayerToTeam;
    systemContext->isPlayerIdExists = isPlayerIdExists;

    for(int teamIndex = 0; teamIndex < teamCount && teamIndex < MAX_TEAMS; teamIndex++) {
        systemContext->teams[teamIndex].id = teamIndex + 1;
        strncpy(systemContext->teams[teamIndex].name, teams[teamIndex], NAME_BUFFER_SIZE - 1);
        systemContext->teams[teamIndex].name[NAME_BUFFER_SIZE - 1] = '\0';
        systemContext->teams[teamIndex].totalPlayers = 0;
        systemContext->teams[teamIndex].averageBattingStrikeRate = 0.0f;
        systemContext->teams[teamIndex].totalBattingStrikeRate = 0.0f;
        systemContext->teams[teamIndex].battingCount = 0;

        for(int roleIndex = 0; roleIndex < TOTAL_ICC_PLAYERS_ROLES; roleIndex++) {
            systemContext->teams[teamIndex].players[roleIndex] = initSortedList(sizeof(ICCPlayer), comparePlayerByPerfIndexDesc);
        }
    }

    for(int playerIndex = 0; playerIndex < playerCount; playerIndex++) {
        ICCTeam* team = systemContext->findTeamByName(systemContext, players[playerIndex].team);
        if(team == NULL) continue;

        ICCPlayer initialPlayer;
        initialPlayer.id = players[playerIndex].id;
        strncpy(initialPlayer.name, players[playerIndex].name, NAME_BUFFER_SIZE - 1);
        initialPlayer.name[NAME_BUFFER_SIZE - 1] = '\0';
        strncpy(initialPlayer.teamName, team->name, NAME_BUFFER_SIZE - 1);
        initialPlayer.teamName[NAME_BUFFER_SIZE - 1] = '\0';

        if(strcmp(players[playerIndex].role, "Batsman") == 0) {
            initialPlayer.role = PLAYER_ROLE_BATSMAN;
        } else if(strcmp(players[playerIndex].role, "Bowler") == 0) {
            initialPlayer.role = PLAYER_ROLE_BOWLER;
        } else if(strcmp(players[playerIndex].role, "All-rounder") == 0) {
            initialPlayer.role = PLAYER_ROLE_ALL_ROUNDER;
        }

        initialPlayer.totalRuns = players[playerIndex].totalRuns;
        initialPlayer.battingAverage = players[playerIndex].battingAverage;
        initialPlayer.strikeRate = players[playerIndex].strikeRate;
        initialPlayer.wickets = players[playerIndex].wickets;
        initialPlayer.economyRate = players[playerIndex].economyRate;
        initialPlayer.performanceIndex = calculatePerformanceIndex(
            initialPlayer.role,
            initialPlayer.battingAverage,
            initialPlayer.strikeRate,
            initialPlayer.wickets,
            initialPlayer.economyRate
        );

        systemContext->addPlayerToTeam(systemContext, team->id, &initialPlayer);
    }

    return systemContext;
}

void destroyICCSystem(ICCSystemContext** systemContext) {
    if(systemContext == NULL || *systemContext == NULL) return;

    for(int teamIndex = 0; teamIndex < (*systemContext)->teamCount; teamIndex++) {
        for(int roleIndex = 0; roleIndex < TOTAL_ICC_PLAYERS_ROLES; roleIndex++) {
            destroySortedList(&((*systemContext)->teams[teamIndex].players[roleIndex]));
        }
    }

    free(*systemContext);
    *systemContext = NULL;
}
