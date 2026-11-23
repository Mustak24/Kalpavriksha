#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "analyzer.h"
#include "commands.h"
#include "common.h"

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
    ICCSystemContext* ctx = initICCSystem();
    if(ctx == NULL) {
        fprintf(stderr, "Failed to initialize ICC ODI System!\n");
        return 1;
    }

    bool running = true;
    while(running) {
        printMenu();
        int choice = promptAndReadInt("Enter your choice: ", 1, 6);

        if(feof(stdin)) {
            printf("\nExiting program...\n");
            break;
        }

        switch(choice) {
            case 1:
                handleAddPlayer(ctx);
                break;
            case 2:
                handleDisplayTeamPlayer(ctx);
                break;
            case 3:
                handleDisplayTeamByStrikeRate(ctx);
                break;
            case 4:
                handleDisplayTopKPlayer(ctx);
                break;
            case 5:
                handleDisplayRoleAcrossTeams(ctx);
                break;
            case 6:
                printf("\nExiting ICC ODI Player Performance Analyzer. Goodbye!\n");
                running = false;
                break;
            default:
                printf("Invalid choice! Please select an option between 1 and 6.\n");
                break;
        }
    }

    destroyICCSystem(&ctx);
    return 0;
}
