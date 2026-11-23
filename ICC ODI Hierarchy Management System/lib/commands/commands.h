#ifndef COMMANDS_H
#define COMMANDS_H

#include "analyzer.h"

void handleAddPlayer(ICCSystemContext* ctx);
void handleDisplayTeamPlayer(ICCSystemContext* ctx);
void handleDisplayTeamByStrikeRate(ICCSystemContext* ctx);
void handleDisplayTopKPlayer(ICCSystemContext* ctx);
void handleDisplayRoleAcrossTeams(ICCSystemContext* ctx);
void handleDisplayTopKPLayer(ICCSystemContext* ctx);

#endif
