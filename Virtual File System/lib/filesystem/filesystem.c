#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "stack.h"
#include "filesystem.h"
#include "common.h"


FileNode* initializeFileNode() {
    FileNode* fileNode = (FileNode*)malloc(sizeof(FileNode));
    if(fileNode == NULL) {
        return NULL;
    }
    
    fileNode->subDirectorys = initializeStack(sizeof(FileNode*));
    fileNode->dataBlocks = initializeStack(sizeof(size_t));
    
    if(
        fileNode->subDirectorys == NULL ||
        fileNode->dataBlocks == NULL
    ) {
        destroyFileNode(&fileNode);
        return NULL;
    }
    
    fileNode->name[0] = '\0';
    fileNode->isDirectory = false;
    fileNode->parentDirectory = NULL;

    return fileNode;
}


void destroyFileNode(FileNode** fileNode) {
    if(fileNode == NULL || *fileNode == NULL) return;

    for(size_t index = 0; index < (*fileNode)->subDirectorys->length; index++) {
        FileNode* const* subDirectory = (FileNode* const*)((*fileNode)->subDirectorys->get(
            (*fileNode)->subDirectorys, 
            index
        ));
        
        destroyFileNode((FileNode**)subDirectory);
    }

    destroyStack(&((*fileNode)->subDirectorys));
    destroyStack(&((*fileNode)->dataBlocks));
    free(*fileNode);

    *fileNode = NULL;
}


FreeBlock* initializeFreeBlock(size_t blockIndex) {
    FreeBlock* freeBlock = (FreeBlock*)malloc(sizeof(FreeBlock));
    if(freeBlock == NULL) return NULL;

    freeBlock->blockIndex = blockIndex;
    freeBlock->next = NULL;
    freeBlock->pre = NULL;

    return freeBlock;
}


void destroyFreeBlock(FreeBlock** freeBlock) {
    if(freeBlock == NULL || *freeBlock == NULL) return;

    free(*freeBlock);
    *freeBlock = NULL;
}


static bool enqueue(FreeBlocksQueue* queue, size_t blockIndex) {
    if(queue == NULL) return false;

    FreeBlock* newBlock = initializeFreeBlock(blockIndex);
    if(newBlock == NULL) return false;

    if(queue->head == NULL || queue->tail == NULL) {
        queue->head = newBlock;
        queue->tail = newBlock;
    } else {   
        newBlock->pre = queue->tail;
        queue->tail->next = newBlock;
        queue->tail = newBlock;
    }

    queue->blocksCount += 1;
    return true;
}


static bool dequeue(FreeBlocksQueue* queue) {
    if(queue == NULL || queue->head == NULL) return false;

    FreeBlock* removeBlock = queue->head;
    queue->head = queue->head->next;
    destroyFreeBlock(&removeBlock);

    queue->blocksCount -= 1;
    return true;
}


FreeBlocksQueue* initializeFreeBlocksQueue() {
    FreeBlocksQueue* queue = (FreeBlocksQueue*)malloc(sizeof(FreeBlocksQueue));
    if(queue == NULL) return NULL;

    queue->head = NULL;
    queue->tail = NULL;
    queue->blocksCount = 0;

    queue->enqueue = enqueue;
    queue->dequeue = dequeue;
    
    return queue;
}


void destroyFreeBlocksQueue(FreeBlocksQueue** queue) {
    if(queue == NULL || *queue == NULL) return;

    while((*queue)->dequeue(*queue));

    free(*queue);
    *queue = NULL;
}


FileSystemContext* initializeFileSystemContext(size_t storageBlocksCount) {
    FileSystemContext* fsContext = (FileSystemContext*)malloc(sizeof(FileSystemContext));
    char* storage = malloc(storageBlocksCount * STORAGE_BLOCK_SIZE * sizeof(char));
    FreeBlocksQueue* freeBlocksQueue = initializeFreeBlocksQueue();
    FileNode* rootDirectory = initializeFileNode();

    if(
        fsContext == NULL ||
        storage == NULL ||
        freeBlocksQueue == NULL ||
        rootDirectory == NULL
    ) {
        free(fsContext);
        free(rootDirectory);
        free(storage);
        return NULL;
    }

    strcpy(rootDirectory->name, ROOT_DIR_NAME);
    rootDirectory->isDirectory = true;

    fsContext->root = rootDirectory;
    fsContext->cwd = rootDirectory;
    fsContext->storage = storage;
    fsContext->storageBlocksCount = storageBlocksCount;

    for(size_t count = 0; count < storageBlocksCount; count++) {
        if(freeBlocksQueue->enqueue(freeBlocksQueue, count) == false) {
            free(fsContext);
            free(rootDirectory);
            free(storage);
            destroyFreeBlocksQueue(&freeBlocksQueue);
            return NULL;
        }    
    }

    fsContext->freeBlocksQueue = freeBlocksQueue;

    return fsContext;
} 


void destroyFileSystemContext(FileSystemContext** fsContext) {
    if(fsContext == NULL || *fsContext == NULL) return;

    destroyFreeBlocksQueue(&((*fsContext)->freeBlocksQueue));
    destroyFileNode(&((*fsContext)->root));
    free((*fsContext)->storage);

    free(*fsContext);
    *fsContext = NULL;
}


FileNode* searchFileInCwd(FileSystemContext* fsContext, const char* fileName, size_t* findAt) {
    if(
        fsContext == NULL ||
        fileName == NULL ||
        !isValidFileNodeName(fileName)
    ) return NULL;

    FileNode* cwd = fsContext->cwd;

    for(size_t index = 0; index < cwd->subDirectorys->length; index++) {
        FileNode* const* fileNode = (FileNode* const*)cwd->subDirectorys->get(
            cwd->subDirectorys, 
            index
        );

        if(
            fileNode == NULL ||
            *fileNode == NULL ||
            (*fileNode)->isDirectory ||
            strcmp(fileName, (*fileNode)->name) != 0
        ) continue;

        if(findAt != NULL) *findAt = index;
        return *fileNode;
    }

    return NULL;
}


FileNode* searchDirectoryInCwd(FileSystemContext* fsContext, const char* directoryName, size_t* findAt) {
    if(
        fsContext == NULL ||
        directoryName == NULL ||
        !isValidFileNodeName(directoryName)
    ) return NULL;
    
    FileNode* cwd = fsContext->cwd;

    for(size_t index = 0; index < cwd->subDirectorys->length; index++) {
        FileNode* const* fileNode = (FileNode* const*)cwd->subDirectorys->get(
            cwd->subDirectorys, 
            index
        );

        if(
            fileNode == NULL ||
            *fileNode == NULL ||
            !(*fileNode)->isDirectory ||
            strcmp(directoryName, (*fileNode)->name) != 0
        ) continue;

        if(findAt != NULL) *findAt = index;
        return *fileNode;
    }

    return NULL;
}


char* getFileNodePath(FileNode* fileNode) {
    if(fileNode == NULL) return NULL;

    size_t pathLength = strlen(fileNode->name);
    char* path = (char*)malloc((pathLength + 1) * sizeof(char));
    if(path == NULL) return NULL;

    strcpy(path, fileNode->name);
    FileNode* currentDirectory = fileNode->parentDirectory;
    
    while(currentDirectory != NULL) {
        size_t fileNodeNameLength = strlen(currentDirectory->name);
        
        pathLength += (fileNodeNameLength + 1);
        char* tempPath = malloc((pathLength + 1) * sizeof(char));
        if(tempPath == NULL) {
            free(path);
            return NULL;
        }

        snprintf(tempPath, pathLength + 1, "%s/%s", currentDirectory->name, path);
        
        free(path);
        path = tempPath;
        currentDirectory = currentDirectory->parentDirectory;
    }
    
    return path;
}