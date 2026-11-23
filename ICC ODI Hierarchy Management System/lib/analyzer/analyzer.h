#ifndef ANALYZER_H
#define ANALYZER_H

#include <stdbool.h>
#include "sortedList.h"

#define TOTAL_ICC_PLAYERS_ROLES 3
#define MAX_TEAMS 10
#define MAX_NAME_LEN 51

typedef enum {
    PLAYER_ROLE_BATSMAN = 0,
    PLAYER_ROLE_BOWLER = 1,
    PLAYER_ROLE_ALL_ROUNDER = 2
} ICCPlayerRole;


typedef struct ICCPlayer {
    int id;
    char name[MAX_NAME_LEN];
    char teamName[MAX_NAME_LEN];
    ICCPlayerRole role;
    int totalRuns;
    float battingAverage;
    float strikeRate;
    int wickets;
    float economyRate;
    float performanceIndex;
} ICCPlayer;


typedef struct ICCTeam {
    int id;
    char name[MAX_NAME_LEN];
    int totalPlayers;
    float averageBattingStrikeRate;
    float totalBattingStrikeRate;
    int battingCount;

    SortedList* players[TOTAL_ICC_PLAYERS_ROLES];
} ICCTeam;


typedef struct ICCSystemContext {
    ICCTeam teams[MAX_TEAMS];
    int teamCount;

    ICCTeam* (*findTeamById)(struct ICCSystemContext* self, int teamId);
    ICCTeam* (*findTeamByName)(struct ICCSystemContext* self, const char* teamName);
    bool (*addPlayerToTeam)(struct ICCSystemContext* self, int teamId, const ICCPlayer* player);
    bool (*isPlayerIdExists)(struct ICCSystemContext* self, int playerId);
} ICCSystemContext;


ICCSystemContext* initICCSystem(void);
void destroyICCSystem(ICCSystemContext** ctx);

float calculatePerformanceIndex(ICCPlayerRole role, float battingAverage, float strikeRate, int wickets, float economyRate);
const char* getRoleName(ICCPlayerRole role);
bool parseRoleChoice(int choice, ICCPlayerRole* outRole);
int comparePlayerByPerfIndexDesc(const void* a, const void* b);

#endif
