#ifndef COMMANDS_H
#define COMMANDS_H

#include "analyzer.h"

void handleAddPlayer(ICCSystemContext* systemContext);
void handleDisplayTeamPlayer(ICCSystemContext* systemContext);
void handleDisplayTeamByStrikeRate(ICCSystemContext* systemContext);
void handleDisplayTopKPlayer(ICCSystemContext* systemContext);
void handleDisplayRoleAcrossTeams(ICCSystemContext* systemContext);

#endif
