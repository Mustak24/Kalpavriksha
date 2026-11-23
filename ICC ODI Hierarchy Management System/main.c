#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "analyzer.h"
#include "commands.h"
#include "common.h"

#define MIN_MENU_CHOICE 1
#define MAX_MENU_CHOICE 6


static void printMenu(void) {
    printf("\n");
    printf("==============================================================================\n");
    printf("ICC ODI Player Performance Analyzer\n");
    printf("==============================================================================\n");
    printf("1. Add Player to Team\n");
    printf("2. Display Players of a Specific Team\n");
    printf("3. Display Teams by Average Batting Strike Rate\n");
    printf("4. Display Top K Players of a Specific Team by Role\n");
    printf("5. Display all Players of specific role Across All Teams by performance index\n");
    printf("6. Exit\n");
    printf("==============================================================================\n");
}

int main(void) {
    ICCSystemContext* systemContext = initICCSystem();
    if(systemContext == NULL) {
        fprintf(stderr, "Failed to initialize ICC ODI System!\n");
        return 1;
    }

    bool isProgramRunning = true;
    while(isProgramRunning) {
        printMenu();
        int menuChoice = promptAndReadInt("Enter your choice: ", MIN_MENU_CHOICE, MAX_MENU_CHOICE);

        if(feof(stdin)) {
            printf("\nExiting program...\n");
            break;
        }

        switch(menuChoice) {
            case 1:
                handleAddPlayer(systemContext);
                break;
            case 2:
                handleDisplayTeamPlayer(systemContext);
                break;
            case 3:
                handleDisplayTeamByStrikeRate(systemContext);
                break;
            case 4:
                handleDisplayTopKPlayer(systemContext);
                break;
            case 5:
                handleDisplayRoleAcrossTeams(systemContext);
                break;
            case 6:
                printf("\nExiting ICC ODI Player Performance Analyzer. Goodbye!\n");
                isProgramRunning = false;
                break;
            default:
                printf(
                    "Invalid choice! Please select an option between %d and %d.\n", 
                    MIN_MENU_CHOICE, 
                    MAX_MENU_CHOICE
                );
                break;
        }
    }

    destroyICCSystem(&systemContext);
    return 0;
}
