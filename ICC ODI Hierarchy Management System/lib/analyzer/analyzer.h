#ifndef ANALYZER_H
#define ANALYZER_H

#include <stdbool.h>

#include "sortedList.h"

#define MAX_TEAMS 10
#define TOTAL_ICC_PLAYERS_ROLES 3

#define MIN_TEAM_ID 1
#define MAX_TEAM_ID 1000

#define MIN_PLAYER_ID 1
#define MAX_PLAYER_ID 10000

#define MIN_PLAYERS_PER_TEAM 11
#define MAX_PLAYERS_PER_TEAM 50

#define MIN_NAME_LEN 1
#define MAX_NAME_LEN 50
#define NAME_BUFFER_SIZE (MAX_NAME_LEN + 1)

#define MIN_ROLE_CHOICE 1
#define MAX_ROLE_CHOICE 3
#define ROLE_CHOICE_BATSMAN 1
#define ROLE_CHOICE_BOWLER 2
#define ROLE_CHOICE_ALL_ROUNDER 3

#define MIN_TOTAL_RUNS 0
#define MAX_TOTAL_RUNS 100000

#define MIN_BATTING_AVG 0.0f
#define MAX_BATTING_AVG 1000.0f

#define MIN_STRIKE_RATE 0.0f
#define MAX_STRIKE_RATE 1000.0f

#define MIN_WICKETS 0
#define MAX_WICKETS 10000

#define MIN_ECONOMY_RATE 0.0f
#define MAX_ECONOMY_RATE 100.0f

#define MIN_TOP_K 1
#define MAX_TOP_K 100

typedef enum {
    PLAYER_ROLE_BATSMAN = 0,
    PLAYER_ROLE_BOWLER = 1,
    PLAYER_ROLE_ALL_ROUNDER = 2
} ICCPlayerRole;

typedef struct ICCPlayer {
    int id;
    char name[NAME_BUFFER_SIZE];
    char teamName[NAME_BUFFER_SIZE];
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
    char name[NAME_BUFFER_SIZE];
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
void destroyICCSystem(ICCSystemContext** systemContext);

float calculatePerformanceIndex(ICCPlayerRole role, float battingAverage, float strikeRate, int wickets, float economyRate);
const char* getRoleName(ICCPlayerRole role);
bool parseRoleChoice(int roleChoice, ICCPlayerRole* outRole);
int comparePlayerByPerfIndexDesc(const void* firstPlayerPtr, const void* secondPlayerPtr);

#endif
