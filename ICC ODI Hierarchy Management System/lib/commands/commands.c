#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commands.h"
#include "common.h"
#include "heap.h"

void handleAddPlayer(ICCSystemContext* systemContext) {
    if(systemContext == NULL) return;

    int teamId = promptAndReadInt("Enter Team ID to add player: ", MIN_TEAM_ID, MAX_TEAM_ID);
    ICCTeam* team = systemContext->findTeamById(systemContext, teamId);
    if(team == NULL) {
        printf("Team with ID %d not found!\n", teamId);
        return;
    }

    if(team->totalPlayers >= MAX_PLAYERS_PER_TEAM) {
        printf(
            "Team %s has reached the maximum capacity of %d players!\n", 
            team->name, 
            MAX_PLAYERS_PER_TEAM
        );
        return;
    }

    printf("Enter Player Details:\n");
    ICCPlayer newPlayer;
    newPlayer.id = promptAndReadInt("Player ID: ", MIN_PLAYER_ID, MAX_PLAYER_ID);
    if(systemContext->isPlayerIdExists(systemContext, newPlayer.id)) {
        printf("Player with ID %d already exists in the system!\n", newPlayer.id);
        return;
    }

    char* inputName = promptAndReadString("Name: ", MAX_NAME_LEN);
    if(inputName == NULL) return;
    strncpy(newPlayer.name, inputName, NAME_BUFFER_SIZE - 1);
    newPlayer.name[NAME_BUFFER_SIZE - 1] = '\0';
    free(inputName);

    int roleChoice = promptAndReadInt(
        "Role (1-Batsman, 2-Bowler, 3-All-rounder): ", 
        MIN_ROLE_CHOICE, 
        MAX_ROLE_CHOICE
    );

    if(!parseRoleChoice(roleChoice, &newPlayer.role)) {
        printf("Invalid role choice!\n");
        return;
    }

    newPlayer.totalRuns = promptAndReadInt("Total Runs: ", MIN_TOTAL_RUNS, MAX_TOTAL_RUNS);
    newPlayer.battingAverage = promptAndReadFloat("Batting Average: ", MIN_BATTING_AVG, MAX_BATTING_AVG);
    newPlayer.strikeRate = promptAndReadFloat("Strike Rate: ", MIN_STRIKE_RATE, MAX_STRIKE_RATE);
    newPlayer.wickets = promptAndReadInt("Wickets: ", MIN_WICKETS, MAX_WICKETS);
    newPlayer.economyRate = promptAndReadFloat("Economy Rate: ", MIN_ECONOMY_RATE, MAX_ECONOMY_RATE);

    bool isSuccess = systemContext->addPlayerToTeam(systemContext, teamId, &newPlayer);
    if(isSuccess) {
        printf("\nPlayer added successfully to Team %s!\n", team->name);
    } else {
        printf("\nFailed to add player to Team %s!\n", team->name);
    }
}

void handleDisplayTeamPlayer(ICCSystemContext* systemContext) {
    if(systemContext == NULL) return;

    int teamId = promptAndReadInt("Enter Team ID: ", MIN_TEAM_ID, MAX_TEAM_ID);
    ICCTeam* team = systemContext->findTeamById(systemContext, teamId);
    if(team == NULL) {
        printf("Team with ID %d not found!\n", teamId);
        return;
    }

    printf("Players of Team %s:\n", team->name);
    printf("====================================================================================\n");
    printf(
        "%-5s %-22s %-13s %-6s %-6s %-6s %-5s %-5s %-10s\n",
        "ID", "Name", "Role", "Runs", "Avg", "SR", "Wkts", "ER", "Perf.Index"
    );
    printf("====================================================================================\n");

    for(int roleIndex = 0; roleIndex < TOTAL_ICC_PLAYERS_ROLES; roleIndex++) {
        SortedListNode* currentNode = team->players[roleIndex]->head;
        while(currentNode != NULL) {
            ICCPlayer* player = (ICCPlayer*)currentNode->data;
            printf("%-5d %-22s %-13s %-6d %-6.1f %-6.1f %-5d %-5.1f %-10.2f\n",
                   player->id, player->name, getRoleName(player->role), player->totalRuns,
                   player->battingAverage, player->strikeRate, player->wickets,
                   player->economyRate, player->performanceIndex);
            currentNode = currentNode->next;
        }
    }

    printf("====================================================================================\n");
    printf("Total Players: %d\n", team->totalPlayers);
    printf("Average Batting Strike Rate: %.2f\n", team->averageBattingStrikeRate);
}

static int compareTeamByAvgSRDesc(const void* firstTeamPtr, const void* secondTeamPtr) {
    const ICCTeam* firstTeam = *(const ICCTeam* const*)firstTeamPtr;
    const ICCTeam* secondTeam = *(const ICCTeam* const*)secondTeamPtr;

    if(firstTeam->averageBattingStrikeRate < secondTeam->averageBattingStrikeRate) return 1;
    if(firstTeam->averageBattingStrikeRate > secondTeam->averageBattingStrikeRate) return -1;
    return firstTeam->id - secondTeam->id;
}

void handleDisplayTeamByStrikeRate(ICCSystemContext* systemContext) {
    if(systemContext == NULL || systemContext->teamCount <= 0) return;

    ICCTeam* teamPointers[MAX_TEAMS];
    for(int teamIndex = 0; teamIndex < systemContext->teamCount; teamIndex++) {
        teamPointers[teamIndex] = &systemContext->teams[teamIndex];
    }

    qsort(teamPointers, systemContext->teamCount, sizeof(ICCTeam*), compareTeamByAvgSRDesc);

    printf("Teams Sorted by Average Batting Strike Rate\n");
    printf("=========================================================\n");
    printf("%-4s %-20s %-14s %-12s\n", "ID", "Team Name", "Avg Bat SR", "Total Players");
    printf("=========================================================\n");

    for(int teamIndex = 0; teamIndex < systemContext->teamCount; teamIndex++) {
        printf("%-4d %-20s %-14.1f %-12d\n",
               teamPointers[teamIndex]->id,
               teamPointers[teamIndex]->name,
               teamPointers[teamIndex]->averageBattingStrikeRate,
               teamPointers[teamIndex]->totalPlayers);
    }
    printf("=========================================================\n");
}

void handleDisplayTopKPlayer(ICCSystemContext* systemContext) {
    if(systemContext == NULL) return;

    int teamId = promptAndReadInt("Enter Team ID: ", MIN_TEAM_ID, MAX_TEAM_ID);
    ICCTeam* team = systemContext->findTeamById(systemContext, teamId);
    if(team == NULL) {
        printf("Team with ID %d not found!\n", teamId);
        return;
    }

    int roleChoice = promptAndReadInt("Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): ", MIN_ROLE_CHOICE, MAX_ROLE_CHOICE);
    ICCPlayerRole role;
    if(!parseRoleChoice(roleChoice, &role)) {
        printf("Invalid role choice!\n");
        return;
    }

    int topPlayerCount = promptAndReadInt("Enter number of players: ", MIN_TOP_K, MAX_TOP_K);

    const char* pluralRole = (
        (role == PLAYER_ROLE_BATSMAN) ? "Batsmen" :
            (role == PLAYER_ROLE_BOWLER) ? "Bowlers" : 
                "All-rounders"
    );

    printf("\nTop %d %s of Team %s:\n", topPlayerCount, pluralRole, team->name);
    printf("====================================================================================\n");
    printf(
        "%-5s %-22s %-13s %-6s %-6s %-6s %-5s %-5s %-10s\n",
        "ID", "Name", "Role", "Runs", "Avg", "SR", "Wkts", "ER", "Perf.Index"
    );
    
    printf("====================================================================================\n");

    SortedListNode* currentNode = team->players[role]->head;
    int displayedPlayerCount = 0;
    while(currentNode != NULL && displayedPlayerCount < topPlayerCount) {
        ICCPlayer* player = (ICCPlayer*)currentNode->data;
        printf(
            "%-5d %-22s %-13s %-6d %-6.1f %-6.1f %-5d %-5.1f %-10.2f\n",
            player->id, 
            player->name, 
            getRoleName(player->role), 
            player->totalRuns,
            player->battingAverage, 
            player->strikeRate, 
            player->wickets,
            player->economyRate, 
            player->performanceIndex
        );

        currentNode = currentNode->next;
        displayedPlayerCount++;
    }

    if(displayedPlayerCount == 0) {
        printf("No %s found in Team %s.\n", pluralRole, team->name);
    }
}


typedef struct MergeHeapNode {
    ICCPlayer player;
    SortedListNode* nextNode;
} MergeHeapNode;

static int compareMergeHeapNode(const void* firstNodePtr, const void* secondNodePtr) {
    const MergeHeapNode* firstNode = (const MergeHeapNode*)firstNodePtr;
    const MergeHeapNode* secondNode = (const MergeHeapNode*)secondNodePtr;

    if(firstNode->player.performanceIndex < secondNode->player.performanceIndex) return 1;
    if(firstNode->player.performanceIndex > secondNode->player.performanceIndex) return -1;
    return firstNode->player.id - secondNode->player.id;
}

void handleDisplayRoleAcrossTeams(ICCSystemContext* systemContext) {
    if(systemContext == NULL || systemContext->teamCount <= 0) return;

    int roleChoice = promptAndReadInt("Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): ", MIN_ROLE_CHOICE, MAX_ROLE_CHOICE);
    ICCPlayerRole role;
    if(!parseRoleChoice(roleChoice, &role)) {
        printf("Invalid role choice!\n");
        return;
    }

    const char* pluralRole = (role == PLAYER_ROLE_BATSMAN) ? "Batsmen" :
                             (role == PLAYER_ROLE_BOWLER) ? "Bowlers" : "All-rounders";

    printf("%s of all teams:\n", pluralRole);
    printf("======================================================================================\n");
    printf("%-5s %-22s %-15s %-13s %-6s %-6s %-6s %-5s %-5s %-10s\n",
           "ID", "Name", "Team", "Role", "Runs", "Avg", "SR", "Wkts", "ER", "Perf.Index");
    printf("======================================================================================\n");

    Heap* heap = initHeap(systemContext->teamCount, sizeof(MergeHeapNode), compareMergeHeapNode);
    if(heap == NULL) return;

    for(int teamIndex = 0; teamIndex < systemContext->teamCount; teamIndex++) {
        if(systemContext->teams[teamIndex].players[role]->head != NULL) {
            MergeHeapNode node;
            node.player = *(ICCPlayer*)systemContext->teams[teamIndex].players[role]->head->data;
            node.nextNode = systemContext->teams[teamIndex].players[role]->head->next;
            heap->push(heap, &node);
        }
    }

    while(heap->size > 0) {
        MergeHeapNode* highestPerformanceNode = (MergeHeapNode*)heap->pop(heap);
        if(highestPerformanceNode == NULL) break;

        printf(
            "%-5d %-22s %-15s %-13s %-6d %-6.1f %-6.1f %-5d %-5.1f %-10.2f\n",
            highestPerformanceNode->player.id, 
            highestPerformanceNode->player.name, 
            highestPerformanceNode->player.teamName,
            getRoleName(highestPerformanceNode->player.role), 
            highestPerformanceNode->player.totalRuns,
            highestPerformanceNode->player.battingAverage, 
            highestPerformanceNode->player.strikeRate,
            highestPerformanceNode->player.wickets, 
            highestPerformanceNode->player.economyRate,
            highestPerformanceNode->player.performanceIndex
        );

        if(highestPerformanceNode->nextNode != NULL) {
            MergeHeapNode nextHeapNode;
            nextHeapNode.player = *(ICCPlayer*)highestPerformanceNode->nextNode->data;
            nextHeapNode.nextNode = highestPerformanceNode->nextNode->next;
            heap->push(heap, &nextHeapNode);
        }

        free(highestPerformanceNode);
    }

    destroyHeap(&heap);
    printf("======================================================================================\n");
}
