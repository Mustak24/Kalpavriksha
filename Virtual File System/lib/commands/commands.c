#include <string.h>
#include <stdio.h>

#include "commands.h"
#include "filesystem.h"
#include "common.h"


COMMAND_CODE getCommandCode(const char* command) {
    if(strcmp(command, "delete") == 0) return COMMAND_delete;
    if(strcmp(command, "create") == 0) return COMMAND_create;
    if(strcmp(command, "mkdir") == 0) return COMMAND_mkdir;
    if(strcmp(command, "write") == 0) return COMMAND_write;
    if(strcmp(command, "rmdir") == 0) return COMMAND_rmdir;
    if(strcmp(command, "exit") == 0) return COMMAND_exit;
    if(strcmp(command, "read") == 0) return COMMAND_read;
    if(strcmp(command, "pwd") == 0) return COMMAND_pwd;
    if(strcmp(command, "cd") == 0) return COMMAND_cd;
    if(strcmp(command, "df") == 0) return COMMAND_df;
    if(strcmp(command, "ls") == 0) return COMMAND_ls;
    return COMMAND_INVALID;
}


FunctionStatus fsCreateDirectory(FileSystemContext* fsContext, const char* directoryName) {
    if(
        fsContext == NULL || 
        directoryName == NULL
    ) {
        return sendFunctionStatus(
            false, 
            "Missing required information to create a new directory."
        );
    }

    if(!isValidFileNodeName(directoryName)) {
        return sendFunctionStatus(
            false,
            "%s is not valid name for a directory.", directoryName
        );
    }

    if(searchDirectoryInCwd(fsContext, directoryName, NULL) != NULL) {
        return sendFunctionStatus(
            false,
            "Directory with name '%s' is already exist.", directoryName
        );
    }

    FileNode* fileNode = initializeFileNode();
    if(fileNode == NULL) {
        return sendFunctionStatus(
            false, 
            "Failed to create new directory."
        );
    }

    fileNode->isDirectory = true;
    fileNode->parentDirectory = fsContext->cwd;
    strcpy(fileNode->name, directoryName);

    if(
        fsContext->cwd->subDirectorys->push(
            fsContext->cwd->subDirectorys,
            &fileNode
        ) == false
    ) {
        destroyFileNode(&fileNode);
        return sendFunctionStatus(
            false, 
            "Failed to save directory."
        );
    }

    return sendFunctionStatus(
        true, 
        "Directory with name '%s' was created Successfully.", directoryName
    );
}


FunctionStatus fsCreateFile(FileSystemContext* fsContext, const char* fileName) {
    if(
        fsContext == NULL || 
        fileName == NULL
    ) {
        return sendFunctionStatus(
            false,
            "Missing Required information to create a new file."
        );
    }

    if(!isValidFileNodeName(fileName)) {
        return sendFunctionStatus(
            false,
            "%s is not valid name for a file.", fileName
        );
    }

    if(searchFileInCwd(fsContext, fileName, NULL) != NULL) {
        return sendFunctionStatus(
            false,
            "File with name '%s' already exist.", fileName
        );
    }

    FileNode* fileNode = initializeFileNode();
    if(fileNode == NULL) {
        return sendFunctionStatus(
            false,
            "Failed to create new file node."
        );
    }

    fileNode->isDirectory = false;
    fileNode->parentDirectory = fsContext->cwd;
    strcpy(fileNode->name, fileName);

    if(
        fsContext->cwd->subDirectorys->push(
            fsContext->cwd->subDirectorys,
            &fileNode
        ) == false
    ) {
        destroyFileNode(&fileNode);
        return sendFunctionStatus(
            false,
            "Failed to save new file."
        );
    }

    return sendFunctionStatus(
        true,
        "File with name '%s' create successfully.", fileName
    );
}


FunctionStatus fsWriteFile(FileSystemContext* fsContext, const char* fileName, const char* data) {
    if(
        fsContext == NULL ||
        fileName == NULL ||
        data == NULL ||
        !isValidFileNodeName(fileName)
    ) {
        return sendFunctionStatus(
            false,
            "Missing required information to write file data."
        );
    }

    FileNode* file = searchFileInCwd(fsContext, fileName, NULL);
    if(file == NULL) {
        return sendFunctionStatus(
            false,
            "File with name '%s' was not found.", fileName
        );
    }

    size_t dataChunksCount;
    char** dataChunks = createStringChunks(
        data, 
        STORAGE_BLOCK_SIZE, 
        &dataChunksCount
    );

    if(dataChunks == NULL) {
        return sendFunctionStatus(
            false,
            "Failed to prepare file data for writing."
        );
    }

    if(dataChunksCount > (fsContext->freeBlocksQueue->blocksCount + file->dataBlocks->length)) {
        destroyStringChunks(&dataChunks, dataChunksCount);
        return sendFunctionStatus(
            false,
            "Not enough free storage blocks to write file data."
        );
    }

    while(file->dataBlocks->length) {
        size_t* blockIndex = (size_t*)file->dataBlocks->get(
            file->dataBlocks,
            file->dataBlocks->length - 1
        );
        
        file->dataBlocks->pop(file->dataBlocks);
        if(blockIndex == NULL) continue;
        
        fsContext->freeBlocksQueue->enqueue(
            fsContext->freeBlocksQueue,
            *blockIndex
        );
    }

    for(size_t chunkCount = 0; chunkCount < dataChunksCount; chunkCount++) {
        char* data = *(dataChunks + chunkCount);
        size_t blockIndex = fsContext->freeBlocksQueue->head->blockIndex;

        file->dataBlocks->push(file->dataBlocks, &blockIndex);
        
        strncpy(
            fsContext->storage + blockIndex * STORAGE_BLOCK_SIZE, 
            data, 
            STORAGE_BLOCK_SIZE
        );

        fsContext->freeBlocksQueue->dequeue(fsContext->freeBlocksQueue);
    }

    destroyStringChunks(&dataChunks, dataChunksCount);
    return sendFunctionStatus(
        true,
        "File with name '%s' was written successfully.", fileName
    );
}


FunctionStatus fsReadFile(FileSystemContext* fsContext, const char* fileName, char** resultFileData) {
    if(
        fsContext == NULL ||
        fileName == NULL ||
        resultFileData == NULL
    ) {
        return sendFunctionStatus(
            false,
            "Missing required information to read file data."
        );
    }

    FileNode* file = searchFileInCwd(fsContext, fileName, NULL);
    if(file == NULL) {
        return sendFunctionStatus(
            false,
            "File with name '%s' is not found.", fileName
        );
    }

    *resultFileData = (char*)malloc(file->dataBlocks->length * STORAGE_BLOCK_SIZE * sizeof(char));
    if(*resultFileData == NULL) {
        return sendFunctionStatus(
            false,
            "Failed to allocate memory for reading data."
        );
    }

    for(size_t index=0; index < file->dataBlocks->length; index++) {
        const size_t dataBlockIndex = *(size_t*)file->dataBlocks->get(file->dataBlocks, index);
        char* blockData = (fsContext->storage + dataBlockIndex * STORAGE_BLOCK_SIZE);

        strncpy(*resultFileData + index * STORAGE_BLOCK_SIZE, blockData, STORAGE_BLOCK_SIZE);
    }

    return sendFunctionStatus(
        true,
        "File with name '%s' data read successfully.", fileName
    );
}


FunctionStatus fsDeleteFile(FileSystemContext* fsContext, const char* fileName) {
    if(
        fsContext == NULL ||
        fileName == NULL
    ) {
        return sendFunctionStatus(
            false,
            "Missing required information for delete a file."
        );
    }

    size_t fileIndex;
    FileNode* file = searchFileInCwd(fsContext, fileName, &fileIndex);
    if(file == NULL) {
        return sendFunctionStatus(
            false,
            "File with name '%s' was not found.", fileName
        );
    }

    for(size_t index = 0; index < file->dataBlocks->length; index++) {
        fsContext->freeBlocksQueue->enqueue(
            fsContext->freeBlocksQueue,
            *(size_t*)file->dataBlocks->get(file->dataBlocks, index)
        );
    }

    fsContext->cwd->subDirectorys->remove(
        fsContext->cwd->subDirectorys,
        fileIndex
    );

    destroyFileNode(&file);
    return sendFunctionStatus(
        true,
        "File '%s' delete successfully.", fileName
    );
}


FunctionStatus fsDeleteDirectory(FileSystemContext* fsContext, const char* directoryName) {
    if(
        fsContext == NULL ||
        directoryName == NULL
    ) {
        return sendFunctionStatus(
            false,
            "Missing required information to delete a directory."
        );
    }

    size_t directoryIndex;
    FileNode* directory = searchDirectoryInCwd(fsContext, directoryName, &directoryIndex);
    
    if(directory == NULL) {
        return sendFunctionStatus(
            false,
            "Directory with name '%s' was not found.", directoryName
        );
    }
    if(directory->subDirectorys->length != 0) {
        return sendFunctionStatus(
            false,
            "Directory with name '%s' is not empty.", directoryName
        );
    }

    fsContext->cwd->subDirectorys->remove(
        fsContext->cwd->subDirectorys,
        directoryIndex
    );

    destroyFileNode(&directory);
    return sendFunctionStatus(
        true,
        "Directory '%s' deleted successfully.", directoryName
    );
}


FunctionStatus dsDisplayFileAndFolder(FileSystemContext* fsContext) {
    if(fsContext == NULL || fsContext->cwd == NULL) {
        return sendFunctionStatus(
            false,
            "Missing file system context to list files and directories."
        );
    }
    
    FileNode* cwd = fsContext->cwd;
    for(size_t index = 0; index < cwd->subDirectorys->length; index++) {
        FileNode* const* node = (FileNode* const*)(cwd->subDirectorys->get(
            cwd->subDirectorys, 
            index
        ));

        if(node == NULL || *node == NULL) continue;

        printf("- %s\n", (*node)->name);
    }

    return sendFunctionStatus(
        true,
        "Files and directories listed successfully."
    );
}


FunctionStatus fsChangeCurrentDirectory(FileSystemContext* fsContext, const char* directoryName) {
    if(
        fsContext == NULL ||
        directoryName == NULL ||
        fsContext->cwd == NULL
    ) {
        return sendFunctionStatus(
            false,
            "Missing required information to change directory."
        );
    }

    if(strcmp(directoryName, "/") == 0) {
        fsContext->cwd = fsContext->root;
        return sendFunctionStatus(
            true,
            "Changed to the root directory."
        );
    }

    if(strcmp(directoryName, "..") == 0 && fsContext->cwd->parentDirectory != NULL) {
        fsContext->cwd = fsContext->cwd->parentDirectory;
        return sendFunctionStatus(
            true,
            "Changed to the parent directory."
        );
    }

    FileNode* directory = searchDirectoryInCwd(fsContext, directoryName, NULL);
    if(directory == NULL) {
        return sendFunctionStatus(
            false,
            "Directory with name '%s' was not found.", directoryName
        );
    }

    fsContext->cwd = directory;
    return sendFunctionStatus(
        true,
        "Changed to directory '%s'.", directoryName
    );
}


FunctionStatus fsDisplayPresentWorkingDirectory(FileSystemContext* fsContext) {
    if(fsContext == NULL || fsContext->cwd == NULL) {
        return sendFunctionStatus(
            false,
            "Missing file system context to display the current directory."
        );
    }

    char* path = getFileNodePath(fsContext->cwd);
    if(path == NULL) {
        return sendFunctionStatus(
            false,
            "Failed to get the current directory path."
        );
    }

    printf("%s\n", path);
    free(path);

    return sendFunctionStatus(
        true,
        "Current directory displayed successfully."
    );
}


FunctionStatus fsDisplayDiskUsage(FileSystemContext* fsContext) {
    if(fsContext == NULL || fsContext->freeBlocksQueue == NULL) {
        return sendFunctionStatus(
            false,
            "Missing file system context to display disk usage."
        );
    }

    printf("\n");
    printf("----- DISK USAGE-----\n");
    printf("TOTAL BLOCKS : %zu\n", fsContext->storageBlocksCount);
    printf("USED BLOCKS  : %zu\n", fsContext->storageBlocksCount - fsContext->freeBlocksQueue->blocksCount);
    printf("FREE BLOCKS  : %zu\n", fsContext->freeBlocksQueue->blocksCount);
    printf("DISK USAGE   : %.2f\n", 
        (1 - ((float)fsContext->freeBlocksQueue->blocksCount / fsContext->storageBlocksCount)) * 100
    );
    printf("---------------------\n\n");

    return sendFunctionStatus(
        true,
        "Disk usage displayed successfully."
    );
}


FunctionStatus fsExit(FileSystemContext** fsContext) {
    if(fsContext == NULL || *fsContext == NULL) {
        return sendFunctionStatus(
            false,
            "Missing file system context to exit."
        );
    }

    destroyFileSystemContext(fsContext);
    printf("EXIT...");

    return sendFunctionStatus(
        true,
        "Exited successfully."
    );
}