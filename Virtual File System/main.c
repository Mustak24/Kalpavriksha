#include <stdio.h>
#include <string.h>

#include "filesystem.h"
#include "commands.h"
#include "common.h"
#include "inputHandlers.h"



int main(void) {
    printf("\n-----VIRTUAL FILE SYSTEM-----\n\n");

    printf("Enter Memory Blocks Count: ");
    size_t blockCount;
    while(!readSize_t(&blockCount, STORAGE_BLOCK_MIN_COUNT, STORAGE_BLOCK_MAX_COUNT)) {
        printf("Warning, Please Enter a valid Count b/w (%d - %d): ", STORAGE_BLOCK_MIN_COUNT, STORAGE_BLOCK_MAX_COUNT);
        clearInputBuffer();
    }

    clearInputBuffer();

    FileSystemContext* fsContext = initializeFileSystemContext(blockCount);
    if(fsContext == NULL) {
        printf("Error: Failed to Initalized File System.\n");
        return 1;
    }

    printf("Compact VFS is now read. use exit command for quit.\n");

    while(fsContext != NULL) {
        char* path = getFileNodePath(fsContext->cwd);
        printf("%s > ", path);
        free(path);

        handleTerminalInput(&fsContext);
    }

    return 0;
}