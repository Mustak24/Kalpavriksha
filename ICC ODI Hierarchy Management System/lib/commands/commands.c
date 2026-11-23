#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commands.h"
#include "common.h"
#include "heap.h"

void handleAddPlayer(ICCSystemContext* ctx) {
    if(ctx == NULL) return;

    int teamId = promptAndReadInt("Enter Team ID to add player: ", 1, 1000);
    ICCTeam* team = ctx->findTeamById(ctx, teamId);
    if(team == NULL) {
        printf("Team with ID %d not found!\n", teamId);
        return;
    }

    if(team->totalPlayers >= 50) {
        printf("Team %s has reached the maximum capacity of 50 players!\n", team->name);
        return;
    }

    printf("Enter Player Details:\n");
    int playerId = promptAndReadInt("Player ID: ", 1, 10000);
    if(ctx->isPlayerIdExists(ctx, playerId)) {
        printf("Player with ID %d already exists in the system!\n", playerId);
        return;
    }

    char* name = promptAndReadString("Name: ", MAX_NAME_LEN - 1);
    if(name == NULL) return;

    int roleChoice = promptAndReadInt("Role (1-Batsman, 2-Bowler, 3-All-rounder): ", 1, 3);
    ICCPlayerRole role;
    if(!parseRoleChoice(roleChoice, &role)) {
        printf("Invalid role choice!\n");
        free(name);
        return;
    }

    int totalRuns = promptAndReadInt("Total Runs: ", 0, 100000);
    float battingAverage = promptAndReadFloat("Batting Average: ", 0.0f, 1000.0f);
    float strikeRate = promptAndReadFloat("Strike Rate: ", 0.0f, 1000.0f);
    int wickets = promptAndReadInt("Wickets: ", 0, 10000);
    float economyRate = promptAndReadFloat("Economy Rate: ", 0.0f, 100.0f);

    ICCPlayer player;
    player.id = playerId;
    strncpy(player.name, name, MAX_NAME_LEN - 1);
    player.name[MAX_NAME_LEN - 1] = '\0';
    free(name);

    player.role = role;
    player.totalRuns = totalRuns;
    player.battingAverage = battingAverage;
    player.strikeRate = strikeRate;
    player.wickets = wickets;
    player.economyRate = economyRate;

    bool success = ctx->addPlayerToTeam(ctx, teamId, &player);
    if(success) {
        printf("\nPlayer added successfully to Team %s!\n", team->name);
    } else {
        printf("\nFailed to add player to Team %s!\n", team->name);
    }
}

void handleDisplayTeamPlayer(ICCSystemContext* ctx) {
    if(ctx == NULL) return;

    int teamId = promptAndReadInt("Enter Team ID: ", 1, 1000);
    ICCTeam* team = ctx->findTeamById(ctx, teamId);
    if(team == NULL) {
        printf("Team with ID %d not found!\n", teamId);
        return;
    }

    printf("Players of Team %s:\n", team->name);
    printf("====================================================================================\n");
    printf("%-5s %-22s %-13s %-6s %-6s %-6s %-5s %-5s %-10s\n",
           "ID", "Name", "Role", "Runs", "Avg", "SR", "Wkts", "ER", "Perf.Index");
    printf("====================================================================================\n");

    for(int r = 0; r < TOTAL_ICC_PLAYERS_ROLES; r++) {
        SortedListNode* curr = team->players[r]->head;
        while(curr != NULL) {
            ICCPlayer* p = (ICCPlayer*)curr->data;
            printf("%-5d %-22s %-13s %-6d %-6.1f %-6.1f %-5d %-5.1f %-10.2f\n",
                   p->id, p->name, getRoleName(p->role), p->totalRuns,
                   p->battingAverage, p->strikeRate, p->wickets,
                   p->economyRate, p->performanceIndex);
            curr = curr->next;
        }
    }

    printf("====================================================================================\n");
    printf("Total Players: %d\n", team->totalPlayers);
    printf("Average Batting Strike Rate: %.2f\n", team->averageBattingStrikeRate);
}

static int compareTeamByAvgSRDesc(const void* a, const void* b) {
    const ICCTeam* t1 = *(const ICCTeam* const*)a;
    const ICCTeam* t2 = *(const ICCTeam* const*)b;

    if(t1->averageBattingStrikeRate < t2->averageBattingStrikeRate) return 1;
    if(t1->averageBattingStrikeRate > t2->averageBattingStrikeRate) return -1;
    return t1->id - t2->id;
}

void handleDisplayTeamByStrikeRate(ICCSystemContext* ctx) {
    if(ctx == NULL || ctx->teamCount <= 0) return;

    ICCTeam* teamPointers[MAX_TEAMS];
    for(int i = 0; i < ctx->teamCount; i++) {
        teamPointers[i] = &ctx->teams[i];
    }

    qsort(teamPointers, ctx->teamCount, sizeof(ICCTeam*), compareTeamByAvgSRDesc);

    printf("Teams Sorted by Average Batting Strike Rate\n");
    printf("=========================================================\n");
    printf("%-4s %-20s %-14s %-12s\n", "ID", "Team Name", "Avg Bat SR", "Total Players");
    printf("=========================================================\n");

    for(int i = 0; i < ctx->teamCount; i++) {
        printf("%-4d %-20s %-14.1f %-12d\n",
               teamPointers[i]->id,
               teamPointers[i]->name,
               teamPointers[i]->averageBattingStrikeRate,
               teamPointers[i]->totalPlayers);
    }
    printf("=========================================================\n");
}

void handleDisplayTopKPlayer(ICCSystemContext* ctx) {
    if(ctx == NULL) return;

    int teamId = promptAndReadInt("Enter Team ID: ", 1, 1000);
    ICCTeam* team = ctx->findTeamById(ctx, teamId);
    if(team == NULL) {
        printf("Team with ID %d not found!\n", teamId);
        return;
    }

    int roleChoice = promptAndReadInt("Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): ", 1, 3);
    ICCPlayerRole role;
    if(!parseRoleChoice(roleChoice, &role)) {
        printf("Invalid role choice!\n");
        return;
    }

    int k = promptAndReadInt("Enter number of players: ", 1, 100);

    const char* pluralRole = (role == PLAYER_ROLE_BATSMAN) ? "Batsmen" :
                             (role == PLAYER_ROLE_BOWLER) ? "Bowlers" : "All-rounders";

    printf("\nTop %d %s of Team %s:\n", k, pluralRole, team->name);
    printf("====================================================================================\n");
    printf("%-5s %-22s %-13s %-6s %-6s %-6s %-5s %-5s %-10s\n",
           "ID", "Name", "Role", "Runs", "Avg", "SR", "Wkts", "ER", "Perf.Index");
    printf("====================================================================================\n");

    SortedListNode* curr = team->players[role]->head;
    int count = 0;
    while(curr != NULL && count < k) {
        ICCPlayer* p = (ICCPlayer*)curr->data;
        printf("%-5d %-22s %-13s %-6d %-6.1f %-6.1f %-5d %-5.1f %-10.2f\n",
               p->id, p->name, getRoleName(p->role), p->totalRuns,
               p->battingAverage, p->strikeRate, p->wickets,
               p->economyRate, p->performanceIndex);
        curr = curr->next;
        count++;
    }

    if(count == 0) {
        printf("No %s found in Team %s.\n", pluralRole, team->name);
    }
}

void handleDisplayTopKPLayer(ICCSystemContext* ctx) {
    handleDisplayTopKPlayer(ctx);
}

typedef struct MergeHeapNode {
    ICCPlayer player;
    SortedListNode* nextNode;
} MergeHeapNode;

static int compareMergeHeapNode(const void* a, const void* b) {
    const MergeHeapNode* m1 = (const MergeHeapNode*)a;
    const MergeHeapNode* m2 = (const MergeHeapNode*)b;

    if(m1->player.performanceIndex < m2->player.performanceIndex) return 1;
    if(m1->player.performanceIndex > m2->player.performanceIndex) return -1;
    return m1->player.id - m2->player.id;
}

void handleDisplayRoleAcrossTeams(ICCSystemContext* ctx) {
    if(ctx == NULL || ctx->teamCount <= 0) return;

    int roleChoice = promptAndReadInt("Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): ", 1, 3);
    ICCPlayerRole role;
    if(!parseRoleChoice(roleChoice, &role)) {
        printf("Invalid role choice!\n");
        return;
    }

    const char* pluralRole = (
        (role == PLAYER_ROLE_BATSMAN) ? 
            "Batsmen" :
            (role == PLAYER_ROLE_BOWLER) ? 
                "Bowlers" : 
            "All-rounders"
    );

    printf("%s of all teams:\n", pluralRole);
    printf("======================================================================================\n");
    printf("%-5s %-22s %-15s %-13s %-6s %-6s %-6s %-5s %-5s %-10s\n",
           "ID", "Name", "Team", "Role", "Runs", "Avg", "SR", "Wkts", "ER", "Perf.Index");
    printf("======================================================================================\n");

    Heap* heap = initHeap(ctx->teamCount, sizeof(MergeHeapNode), compareMergeHeapNode);
    if(heap == NULL) return;

    for(int i = 0; i < ctx->teamCount; i++) {
        if(ctx->teams[i].players[role]->head != NULL) {
            MergeHeapNode node;
            node.player = *(ICCPlayer*)ctx->teams[i].players[role]->head->data;
            node.nextNode = ctx->teams[i].players[role]->head->next;
            heap->push(heap, &node);
        }
    }

    while(heap->size > 0) {
        MergeHeapNode* top = (MergeHeapNode*)heap->pop(heap);
        if(top == NULL) break;

        printf("%-5d %-22s %-15s %-13s %-6d %-6.1f %-6.1f %-5d %-5.1f %-10.2f\n",
               top->player.id, top->player.name, top->player.teamName,
               getRoleName(top->player.role), top->player.totalRuns,
               top->player.battingAverage, top->player.strikeRate,
               top->player.wickets, top->player.economyRate,
               top->player.performanceIndex);

        if(top->nextNode != NULL) {
            MergeHeapNode next;
            next.player = *(ICCPlayer*)top->nextNode->data;
            next.nextNode = top->nextNode->next;
            heap->push(heap, &next);
        }

        free(top);
    }

    destroyHeap(&heap);
    printf("======================================================================================\n");
}
