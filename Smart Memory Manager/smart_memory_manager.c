#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define POOL_MIN_SIZE 1
#define POOL_MAX_SIZE 1000000

#define MEMORY_BLOCK_MIN_SIZE 1
#define MEMORY_BLOCK_MIN_ID 1


typedef enum MemoryAllocationMethod {
    FIRST_FIT = 1,
    BEST_FIT,
    WORST_FIT
} MemoryAllocationMethod;


typedef struct MemoryBlock {
    int id;
    int size;
    int startFrom;
    bool isAllocated;
} MemoryBlock;


typedef struct MemoryPool {
    int freeMemory;
    int totalMemory;
    int memoryBlockCount;

    char* memoryBuffer;
    MemoryBlock* memoryBlocks;
    MemoryAllocationMethod activeAllocationMethod;
} MemoryPool;


MemoryPool pool;


void clearInputBuffer() {
    int bufferChar;
    while((bufferChar = getchar()) != '\n' && bufferChar != EOF);
}


int readMemoryBlockId(const char* prompt, int* resultId) {
    if(prompt != NULL) {
        printf("%s", prompt);
    }

    int scanResult;
    while(
        (scanResult = scanf("%d", resultId)) != 1 ||
        *resultId < MEMORY_BLOCK_MIN_ID
    ) {
        if(scanResult == EOF) {
            printf("Error reading input. Exiting.\n");
            return 1;
        }

        printf("Warning, memory block ID must be a positive integer: ");      
        clearInputBuffer();  
    }

    clearInputBuffer();
    return 0;
}


int readMemoryBlockSize(const char* prompt, int* resultBlockSize) {
    if(prompt != NULL) {
        printf("%s", prompt);
    }

    int scanResult;
    while(
        (scanResult = scanf("%d", resultBlockSize)) != 1 ||
        *resultBlockSize < MEMORY_BLOCK_MIN_SIZE
    ) {
        if(scanResult == EOF) {
            printf("Error reading input. Exiting.\n");
            return 1;
        }

        printf("Warning, size must be a positive integer: ");      
        clearInputBuffer();  
    }

    clearInputBuffer();
    return 0;
}


int generateMemoryBlockId() {
    static int currentId = 1;
    return currentId++;
}


MemoryBlock initializeMemoryBlock(int id, int size, int startFrom, bool isAllocated) {
    MemoryBlock block;
    block.id = id;
    block.size = size;
    block.startFrom = startFrom;
    block.isAllocated = isAllocated;
    return block;
}


const char* getAllocationMethodName(MemoryAllocationMethod method) {
    switch(method) {
        case FIRST_FIT:
            return "First Fit";
        case BEST_FIT:
            return "Best Fit";
        case WORST_FIT:
            return "Worst Fit";
        default:
            return "Unknown";
    }
}

void mergePoolFreeBlocks() {
    for(int index = 1; index < pool.memoryBlockCount; index++) {
        MemoryBlock* currentBlock = &pool.memoryBlocks[index];
        MemoryBlock* previousBlock = &pool.memoryBlocks[index - 1];

        if(
            currentBlock->size == 0 || 
            (!currentBlock->isAllocated && !previousBlock->isAllocated)
        ) {
            previousBlock->size += currentBlock->size;

            for(int j = index + 1; j < pool.memoryBlockCount; j++) {
                pool.memoryBlocks[j - 1] = pool.memoryBlocks[j];
            }

            pool.memoryBlockCount -= 1;
            index--;
        }
    }
}


int searchFirstFitMemoryBlock(int size) {
    if(size > pool.freeMemory) {
        printf("Error: not enough memory available in the pool.\n");
        return -1;
    }

    if(pool.memoryBlockCount == 0) return 0;

    for(int index = 0; index < pool.memoryBlockCount; index++) {
        MemoryBlock block = pool.memoryBlocks[index];
        if(!block.isAllocated && block.size >= size) {
            return index;
        }
    }

    return -1;
}

int searchBestFitMemoryBlock(int size) {
    if(size > pool.freeMemory) {
        printf("Error: not enough memory available in the pool.\n");
        return -1;
    }

    if(pool.memoryBlockCount == 0) return 0;

    int bestFitIndex = -1;
    int bestFitSize = pool.totalMemory + 1;


    for(int index = 0; index < pool.memoryBlockCount; index++) {
        MemoryBlock block = pool.memoryBlocks[index];
        if(block.isAllocated) continue;

        if(block.size >= size && block.size < bestFitSize) {
            bestFitSize = block.size;
            bestFitIndex = index;
        }
    }

    return bestFitIndex;
}

int searchWorstFitMemoryBlock(int size) {
    if(size > pool.freeMemory) {
        printf("Error: not enough memory available in the pool.\n");
        return -1;
    }

    if(pool.memoryBlockCount == 0) return 0;

    int worstFitIndex = -1;
    int worstFitSize = -1;

    for(int index = 0; index < pool.memoryBlockCount; index++) {
        MemoryBlock block = pool.memoryBlocks[index];
        if(block.isAllocated) continue;

        if(block.size >= size && block.size > worstFitSize) {
            worstFitSize = block.size;
            worstFitIndex = index;
        }
    }

    return worstFitIndex;
}


int searchAvailableMemorySpace(int size) {
    switch(pool.activeAllocationMethod) {
        case FIRST_FIT:
            return searchFirstFitMemoryBlock(size);
        case BEST_FIT:
            return searchBestFitMemoryBlock(size);
        case WORST_FIT:
            return searchWorstFitMemoryBlock(size);
        default:
            printf("Error: invalid memory allocation method.\n");
            return -1;
    }
}


int searchBlockById(int targetBlockId) {
    for(int index = 0; index < pool.memoryBlockCount; index++) {
        if(
            pool.memoryBlocks[index].id == targetBlockId && 
            pool.memoryBlocks[index].isAllocated
        ) return index;
    }
    return -1;
}


int addNewMemoryBlockInPool(int id, int size) {
    int insertIndex = searchAvailableMemorySpace(size);
    if(insertIndex == -1) {
        printf("Error: no suitable memory block found for allocation.\n");
        return -1;
    }

    MemoryBlock newBlock = initializeMemoryBlock(
        id,
        size,
        pool.memoryBlocks[insertIndex].startFrom,
        true
    );

    if(size == pool.memoryBlocks[insertIndex].size) {
        pool.memoryBlocks[insertIndex] = newBlock;
        pool.freeMemory -= size;
        return insertIndex;
    }

    MemoryBlock* tempMemoryBlocks = realloc(
        pool.memoryBlocks, 
        (pool.memoryBlockCount + 1) * sizeof(MemoryBlock)
    );

    if(tempMemoryBlocks == NULL) {
        printf("Error: failed to allocate memory for new memory block.\n");
        return -1;
    }

    pool.memoryBlocks = tempMemoryBlocks;
    pool.memoryBlockCount += 1;

    for(int index = pool.memoryBlockCount-1; index > insertIndex; index--) {
        pool.memoryBlocks[index] = pool.memoryBlocks[index -  1];
    }

    pool.memoryBlocks[insertIndex] = newBlock;
    
    pool.memoryBlocks[insertIndex + 1].startFrom += size;
    pool.memoryBlocks[insertIndex + 1].size -= size;

    pool.freeMemory -= size;
    return insertIndex;
}


bool releaseMemoryBlockFromPool(int blockId) {
    for(int index = 0; index < pool.memoryBlockCount; index++) {
        if(pool.memoryBlocks[index].id == blockId && pool.memoryBlocks[index].isAllocated) {
            pool.memoryBlocks[index].isAllocated = false;
            pool.freeMemory += pool.memoryBlocks[index].size;

            mergePoolFreeBlocks();
            return true;
        }
    }

    printf("Error: memory block with the ID=%d not found.\n", blockId);
    return false;
}


int resizeMemoryBlockInPool(int blockId, int newSize) {
    int targetBlockIndex = searchBlockById(blockId);
    if(targetBlockIndex == -1) {
        printf("Error: no memory block available with ID %d\n", blockId);
        return 1;
    }

    int requiredSize = newSize - pool.memoryBlocks[targetBlockIndex].size;
    if(requiredSize == 0) return 0;

    if(requiredSize < 0) {
        MemoryBlock* tempNewMemoryBlocks = realloc(
            pool.memoryBlocks, 
            (pool.memoryBlockCount + 1) * sizeof(MemoryBlock)
        );

        if(tempNewMemoryBlocks == NULL) {
            printf("Error: failed to allocate memory for new memory block.\n");
            return 1;
        }
        
        pool.memoryBlocks = tempNewMemoryBlocks;

        pool.memoryBlocks[targetBlockIndex].size = newSize;
        pool.freeMemory += (-requiredSize);
        pool.memoryBlockCount += 1;

        for(int index = pool.memoryBlockCount-1; index > targetBlockIndex+1; index--) {
            tempNewMemoryBlocks[index] = tempNewMemoryBlocks[index - 1];
        }

        pool.memoryBlocks[targetBlockIndex + 1] = initializeMemoryBlock(
            -1,
            -requiredSize,
            pool.memoryBlocks[targetBlockIndex].startFrom + newSize,
            false
        );

        mergePoolFreeBlocks();
        return 0;
    }

    if(
        (targetBlockIndex + 1) < pool.memoryBlockCount &&
        pool.memoryBlocks[targetBlockIndex + 1].isAllocated == false &&
        pool.memoryBlocks[targetBlockIndex + 1].size >= requiredSize
    ) {
        pool.memoryBlocks[targetBlockIndex].size = newSize;
        pool.memoryBlocks[targetBlockIndex + 1].startFrom += requiredSize;
        pool.memoryBlocks[targetBlockIndex + 1].size -= requiredSize;
        pool.freeMemory -= requiredSize;

        mergePoolFreeBlocks();
        return 0;
    }

    MemoryBlock targetBlock = pool.memoryBlocks[targetBlockIndex];
    int newBlockIndex = addNewMemoryBlockInPool(
        generateMemoryBlockId(), 
        newSize
    );
    if(newBlockIndex == -1) {
        printf("Error: failed to allocate new memory block for resizing with ID %d.\n", blockId);
        return 1;
    }

    memmove(
        pool.memoryBuffer + pool.memoryBlocks[newBlockIndex].startFrom,
        pool.memoryBuffer + targetBlock.startFrom,
        targetBlock.size
    );

    if(!releaseMemoryBlockFromPool(targetBlock.id)) {
        printf("Error: failed to release old block\n");
        return 1;
    }

    mergePoolFreeBlocks();
    return 0;
}


void handleMemoryAllocation() {
    int size;
    if(readMemoryBlockSize("Enter memory block size: ", &size) != 0) {
        printf("Error: failed to read memory block size.\n");
        return;
    }

    if(size > pool.freeMemory) {
        printf("Error: not enough memory available in the pool.\n");
        return;
    }

    int newBlockIndex = addNewMemoryBlockInPool(generateMemoryBlockId(), size);
    if(newBlockIndex == -1) {
        printf("Error: failed to allocate memory block of size %d.\n", size);
        return;
    }

    printf(
        "Allocation successful. ID: %d, Start: %d, Size: %d\n", 
        pool.memoryBlocks[newBlockIndex].id, 
        pool.memoryBlocks[newBlockIndex].startFrom, 
        pool.memoryBlocks[newBlockIndex].size
    );
}


void handleMemoryRelease() {
    int id;
    if(readMemoryBlockId("Enter memory block ID to release: ", &id) != 0) {
        printf("Error: failed to read memory block ID.\n");
        return;
    }

    if(!releaseMemoryBlockFromPool(id)) {
        printf("Error: failed to release memory block.\n");
        return;
    }

    printf("Allocation with ID %d released successfully.\n", id);
}


void handleMemoryResize() {
    int id;
    if(readMemoryBlockId("Enter memory block ID to resize: ", &id) != 0) {
        printf("Error: failed to read memory block ID.\n");
        return;
    }

    int targetBlockIndex = searchBlockById(id);
    if(targetBlockIndex == -1) {
        printf("Error: no memory block available with ID %d\n", id);
        return;
    }

    int size;
    if(readMemoryBlockSize("Memory Block new size: ", &size) != 0) {
        printf("Error: failed to read block new size\n");
        return;
    }

    if(resizeMemoryBlockInPool(id, size) != 0) {
        printf("Error: failed to resize memory block with ID %d.\n", id);
        return;
    }

    targetBlockIndex = searchBlockById(id);

    printf(
        "Resize successful. ID: %d, Start: %d, New Size: %d\n", 
        pool.memoryBlocks[targetBlockIndex].id, 
        pool.memoryBlocks[targetBlockIndex].startFrom, 
        size
    );
}


void handleMemoryAllocationMethodChange() {
    printf("\nSelect Allocation Method:\n");
    printf("1. First Fit\n");
    printf("2. Best Fit\n");
    printf("3. Worst Fit\n");
    printf("Enter your choice: ");

    int methodChoice;
    while(
        scanf("%d", &methodChoice) != 1 ||
        (
            methodChoice != FIRST_FIT && 
            methodChoice != BEST_FIT && 
            methodChoice != WORST_FIT
        )
    ) {
        if(feof(stdin)) {
            printf("Error: Input ended unexpectedly\n");
            return;
        }

        clearInputBuffer();
        printf("Invalid input. Please select valid option: ");
    }

    clearInputBuffer();

    pool.activeAllocationMethod = (MemoryAllocationMethod)methodChoice;
    printf("Allocation method set to %s successfully.\n", 
        getAllocationMethodName((MemoryAllocationMethod)methodChoice)
    );
}


void handleMemoryPoolStatusDisplay() {
    int freeBlocks = 0, largestFreeBlockSize = 0;
   
    for(int index = 0; index < pool.memoryBlockCount; index++) {
        MemoryBlock block = pool.memoryBlocks[index];
        if(!block.isAllocated) freeBlocks++;

        if(!block.isAllocated && block.size > largestFreeBlockSize) {
            largestFreeBlockSize = block.size;
        }
    }

    printf("\n-----Memory Pool Status-----\n");
    printf("Total Memory       : %10d bytes\n", pool.totalMemory);
    printf("Allocated Memory   : %10d bytes\n", pool.totalMemory - pool.freeMemory);
    printf("Free Memory        : %10d bytes\n", pool.freeMemory);
    printf("Free Blocks        : %10d\n", freeBlocks);
    printf("Largest Free Block : %10d bytes\n", largestFreeBlockSize);

    if(pool.freeMemory > 0 && largestFreeBlockSize < pool.freeMemory) {
        printf("\nNote: The request for %d bytes failed even though %d bytes are free, because the free memory is fragmented.\n", 
            largestFreeBlockSize + 1, 
            pool.freeMemory
        );
    }
}


void handleMemoryMapDisplay() {
    printf("\n----- Memory Map ( %d bytes ) -----\n", pool.totalMemory);

    for(int index = 0; index < pool.memoryBlockCount; index++) {
        MemoryBlock block = pool.memoryBlocks[index];

        if(!block.isAllocated) {
            printf(
                "[ Free         | %10d bytes | Start from %10d ]\n", 
                block.size, 
                block.startFrom
            );
        } else {   
            printf(
                "[ Allocated #%d | %10d bytes | Start from %10d ]\n",
                block.id, 
                block.size,
                block.startFrom
            );
        }
    }

    printf("\n");
}


void handleExit() {
    printf("\n---Exiting Memory Manager---\n");
    free(pool.memoryBuffer);
    free(pool.memoryBlocks);
}


int initializeMemoryPool() {
    printf("Enter memory pool size (%d-%d bytes): ", POOL_MIN_SIZE, POOL_MAX_SIZE);
    
    while(
        readMemoryBlockSize(NULL, &pool.totalMemory) != 0 || 
        pool.totalMemory < POOL_MIN_SIZE || 
        pool.totalMemory > POOL_MAX_SIZE
    ) {
        if(feof(stdin)) {
            printf("Error: Input ends unexpectedly\n");
            return 1;
        }

        printf("Invalid input. Please enter a valid memory pool size (%d-%d bytes): ", POOL_MIN_SIZE, POOL_MAX_SIZE);
    }
    
    handleMemoryAllocationMethodChange();

    pool.memoryBuffer = (char*)malloc(pool.totalMemory * sizeof(char));
    if(pool.memoryBuffer == NULL) {
        printf("Error: Memory allocation for pool failed\n");
        return 1;
    }

    pool.memoryBlocks = (MemoryBlock*)malloc(sizeof(MemoryBlock));
    if(pool.memoryBlocks == NULL) {
        printf("Error: Memory allocation for memory blocks failed\n");
        free(pool.memoryBuffer);
        return 1;
    }

    pool.memoryBlocks[0] = initializeMemoryBlock(-1, pool.totalMemory, 0, false);
    pool.freeMemory = pool.totalMemory;
    pool.memoryBlockCount = 1;

    return 0;
}


int main() {
    if(initializeMemoryPool() != 0) {
        printf("Error: failed to initialized memory pool");
        return 1;
    }

    int choice;
    do {
        printf("\n-----Memory Management-----\n");
        printf("1. Allocate Memory\n");
        printf("2. Deallocate Memory\n");
        printf("3. Resize Memory Block\n");
        printf("4. Display Memory Status\n");
        printf("5. Display Memory Map\n");
        printf("6. Change Allocation Method\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");

        while(scanf("%d", &choice) != 1) {
            if(feof(stdin)) {
                printf("Error: Input ended unexpectedly\n");
                handleExit();
            }

            printf("Invalid input. Please select a valid option: ");
            clearInputBuffer();
        }

        clearInputBuffer();

        switch(choice) {
            case 1:
                handleMemoryAllocation();
                break;
            case 2:
                handleMemoryRelease();
                break;
            case 3:
                handleMemoryResize();
                break;
            case 4:
                handleMemoryPoolStatusDisplay();
                break;
            case 5:
                handleMemoryMapDisplay();
                break;
            case 6:
                handleMemoryAllocationMethodChange();
                break;
            case 7:
                handleExit();
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while(choice != 7);
}