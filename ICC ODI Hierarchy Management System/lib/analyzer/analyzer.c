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

bool parseRoleChoice(int choice, ICCPlayerRole* outRole) {
    if(outRole == NULL) return false;
    switch(choice) {
        case 1:
            *outRole = PLAYER_ROLE_BATSMAN;
            return true;
        case 2:
            *outRole = PLAYER_ROLE_BOWLER;
            return true;
        case 3:
            *outRole = PLAYER_ROLE_ALL_ROUNDER;
            return true;
        default:
            return false;
    }
}

int comparePlayerByPerfIndexDesc(const void* a, const void* b) {
    const ICCPlayer* p1 = (const ICCPlayer*)a;
    const ICCPlayer* p2 = (const ICCPlayer*)b;

    if (p1->performanceIndex > p2->performanceIndex) return 1;
    if (p1->performanceIndex < p2->performanceIndex) return -1;

    return p1->id - p2->id;
}

static ICCTeam* findTeamById(ICCSystemContext* self, int teamId) {
    if(self == NULL || self->teamCount <= 0) return NULL;

    int low = 0;
    int high = self->teamCount - 1;

    while(low <= high) {
        int mid = low + (high - low) / 2;
        if(self->teams[mid].id == teamId) {
            return &self->teams[mid];
        } else if(self->teams[mid].id < teamId) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    return NULL;
}

static ICCTeam* findTeamByName(ICCSystemContext* self, const char* teamName) {
    if(self == NULL || teamName == NULL) return NULL;

    for(int i = 0; i < self->teamCount; i++) {
        if(strcmp(self->teams[i].name, teamName) == 0) {
            return &self->teams[i];
        }
    }

    return NULL;
}

static bool isPlayerIdExists(ICCSystemContext* self, int playerId) {
    if(self == NULL) return false;

    for(int t = 0; t < self->teamCount; t++) {
        for(int r = 0; r < TOTAL_ICC_PLAYERS_ROLES; r++) {
            SortedListNode* curr = self->teams[t].players[r]->head;
            while(curr != NULL) {
                ICCPlayer* p = (ICCPlayer*)curr->data;
                if(p->id == playerId) return true;
                curr = curr->next;
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
    strncpy(newPlayer.teamName, team->name, MAX_NAME_LEN - 1);
    newPlayer.teamName[MAX_NAME_LEN - 1] = '\0';
    newPlayer.performanceIndex = calculatePerformanceIndex(
        newPlayer.role, newPlayer.battingAverage, newPlayer.strikeRate,
        newPlayer.wickets, newPlayer.economyRate
    );

    bool inserted = team->players[newPlayer.role]->insert(team->players[newPlayer.role], &newPlayer);
    if(!inserted) return false;

    team->totalPlayers++;
    if(newPlayer.role == PLAYER_ROLE_BATSMAN || newPlayer.role == PLAYER_ROLE_ALL_ROUNDER) {
        team->totalBattingStrikeRate += newPlayer.strikeRate;
        team->battingCount++;
        team->averageBattingStrikeRate = team->totalBattingStrikeRate / (float)team->battingCount;
    }

    return true;
}

ICCSystemContext* initICCSystem(void) {
    ICCSystemContext* ctx = (ICCSystemContext*)malloc(sizeof(ICCSystemContext));
    if(ctx == NULL) return NULL;

    ctx->teamCount = teamCount;
    ctx->findTeamById = findTeamById;
    ctx->findTeamByName = findTeamByName;
    ctx->addPlayerToTeam = addPlayerToTeam;
    ctx->isPlayerIdExists = isPlayerIdExists;

    for(int i = 0; i < teamCount && i < MAX_TEAMS; i++) {
        ctx->teams[i].id = i + 1;
        strncpy(ctx->teams[i].name, teams[i], MAX_NAME_LEN - 1);
        ctx->teams[i].name[MAX_NAME_LEN - 1] = '\0';
        ctx->teams[i].totalPlayers = 0;
        ctx->teams[i].averageBattingStrikeRate = 0.0f;
        ctx->teams[i].totalBattingStrikeRate = 0.0f;
        ctx->teams[i].battingCount = 0;

        for(int r = 0; r < TOTAL_ICC_PLAYERS_ROLES; r++) {
            ctx->teams[i].players[r] = initSortedList(sizeof(ICCPlayer), comparePlayerByPerfIndexDesc);
        }
    }

    for(int i = 0; i < playerCount; i++) {
        ICCTeam* team = ctx->findTeamByName(ctx, players[i].team);
        if(team == NULL) continue;

        ICCPlayerRole role = PLAYER_ROLE_BATSMAN;
        if(strcmp(players[i].role, "Batsman") == 0) {
            role = PLAYER_ROLE_BATSMAN;
        } else if(strcmp(players[i].role, "Bowler") == 0) {
            role = PLAYER_ROLE_BOWLER;
        } else if(strcmp(players[i].role, "All-rounder") == 0) {
            role = PLAYER_ROLE_ALL_ROUNDER;
        }

        ICCPlayer p;
        p.id = players[i].id;
        strncpy(p.name, players[i].name, MAX_NAME_LEN - 1);
        p.name[MAX_NAME_LEN - 1] = '\0';
        strncpy(p.teamName, team->name, MAX_NAME_LEN - 1);
        p.teamName[MAX_NAME_LEN - 1] = '\0';
        p.role = role;
        p.totalRuns = players[i].totalRuns;
        p.battingAverage = players[i].battingAverage;
        p.strikeRate = players[i].strikeRate;
        p.wickets = players[i].wickets;
        p.economyRate = players[i].economyRate;
        p.performanceIndex = calculatePerformanceIndex(role, p.battingAverage, p.strikeRate, p.wickets, p.economyRate);

        ctx->addPlayerToTeam(ctx, team->id, &p);
    }

    return ctx;
}

void destroyICCSystem(ICCSystemContext** ctx) {
    if(ctx == NULL || *ctx == NULL) return;

    for(int i = 0; i < (*ctx)->teamCount; i++) {
        for(int r = 0; r < TOTAL_ICC_PLAYERS_ROLES; r++) {
            destroySortedList(&((*ctx)->teams[i].players[r]));
        }
    }

    free(*ctx);
    *ctx = NULL;
}
