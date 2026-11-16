#ifndef FILESYSTEM
#define FILESYSTEM

#include <stdbool.h>
#include <stdlib.h>
#include "stack.h"

#define ROOT_DIR_NAME "root"
#define FILE_NAME_MIN_LEN 1
#define FILE_NAME_MAX_LEN 50

#define STORAGE_BLOCK_SIZE 512
#define STORAGE_BLOCK_MIN_COUNT 1
#define STORAGE_BLOCK_MAX_COUNT 5000



typedef struct FileNode {
    char name[FILE_NAME_MAX_LEN + 1];
    bool isDirectory;

    struct FileNode* parentDirectory;

    Stack* dataBlocks;
    Stack* subDirectorys;    
} FileNode;

FileNode* initializeFileNode(void);
void destroyFileNode(FileNode** node);


typedef struct FreeBlock {
    size_t blockIndex;

    struct FreeBlock* next;
    struct FreeBlock* pre;
} FreeBlock;

FreeBlock* initializeFreeBlock(size_t blockIndex);
void destroyFreeBlock(FreeBlock** block);


typedef struct FreeBlocksQueue {
    FreeBlock* head;
    FreeBlock* tail;
    size_t blocksCount;

    bool (*enqueue)(struct FreeBlocksQueue* self, size_t blockIndex);
    bool (*dequeue)(struct FreeBlocksQueue* self);
} FreeBlocksQueue;


FreeBlocksQueue* initializeFreeBlocksQueue(void);
void destroyFreeBlocksQueue(FreeBlocksQueue** queue);


typedef struct FileSystemContext {
    FileNode* root;
    FileNode* cwd;

    char* storage;
    size_t storageBlocksCount;
    FreeBlocksQueue* freeBlocksQueue;
} FileSystemContext;

FileSystemContext* initializeFileSystemContext(size_t storageBlocksCount);
void destroyFileSystemContext(FileSystemContext** fsContext);

FileNode* searchFileInCwd(FileSystemContext* fsContext, const char* fileName, size_t* findAt);
FileNode* searchDirectoryInCwd(FileSystemContext* fsContext, const char* directoryName, size_t* findAt);

char* getFileNodePath(FileNode* fileNode);

#endif