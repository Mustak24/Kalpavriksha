# C Programming Assignment

# Smart Memory Manager

## 1. Problem Statement

Implement a simplified memory manager that simulates allocating and releasing regions of a fixed-size memory pool. The program must manage one dynamically allocated byte buffer and a separate dynamically allocated array of metadata records describing the regions in that buffer.

The pool is divided into contiguous blocks. Each block is either allocated to a request or available for future allocation. As requests are processed, blocks may need to be split, merged, resized, or moved. The program must keep the memory map consistent after every operation.

## 2. Objectives

- Use `malloc()`, `realloc()`, and `free()` appropriately.
- Represent memory regions using structures stored in a dynamically allocated array.
- Implement and compare First Fit, Best Fit, and Worst Fit allocation strategies.
- Split free blocks during allocation and merge adjacent free blocks after release.
- Resize existing allocations while preserving their data.
- Calculate memory usage and identify external fragmentation.
- Handle invalid requests and allocation failures without corrupting the pool.

## 3. Functional Requirements

### 3.1 Initialize the Memory Pool

- Ask the user for the total pool size in bytes.
- Allocate the pool dynamically as a byte buffer. Entire pool is initially one free block.
- Reject invalid pool sizes and handle allocation failures safely.

### 3.2 Allocate Memory

At program startup, allow the user to choose First Fit, Best Fit, or Worst Fit. Keep the selected strategy active for subsequent allocation requests until the user changes it through the menu.

Allow the user to request a block of a specified size. The strategies are:

- **First Fit:** Select the first free block large enough for the request.
- **Best Fit:** Select the smallest free block that can satisfy the request.
- **Worst Fit:** Select the largest free block that can satisfy the request.

On success, assign a unique allocation ID. If the chosen free block is larger than the request, split it into an allocated block and a remaining free block. If no suitable block exists, report that the request failed.

### 3.3 Release Memory

- Release an allocation using its allocation ID.
- Reject unknown IDs and attempts to release an already-free block.
- Mark the released region as free.
- Merge adjacent free blocks so the metadata does not contain unnecessarily separate free regions.

### 3.4 Resize an Allocation

Allow the user to increase or decrease the size of an existing allocation.

- If possible, expand into an adjacent free block without moving the allocation.
- If in-place expansion is not possible, search for another suitable free region, move the existing data, and release the old region.
- If resizing fails, the original allocation and its data must remain valid and unchanged.
- If shrinking creates a free region, update the metadata and merge adjacent free blocks where applicable.

### 3.5 Display the Memory Map

Display blocks in address order, including each block's starting offset, size, and status. Allocated blocks must show their allocation IDs.

Example (illustrative):

```text
Memory pool: 1000 bytes
[Allocated #1: 120 bytes] [Free: 80 bytes]
[Allocated #2: 250 bytes] [Free: 150 bytes]
[Allocated #3: 100 bytes] [Free: 300 bytes]
```

### 3.6 Display Memory Statistics

- Total pool size.
- Total allocated bytes and total free bytes.
- Number of allocated blocks and free blocks.
- Size of the largest contiguous free block.

### 3.7 Detect External Fragmentation

Report when a request cannot be satisfied even though total free memory is greater than or equal to the requested size. Explain the situation using the sizes of the individual free blocks.

## 4. Data Structure and Design Constraints

- Use a dynamically allocated byte buffer for the simulated memory pool.
- Use a separate dynamically allocated array of structures to describe the blocks. Do not use a linked list.
- Structure should represent the block and it should contain block's starting offset, size, allocation ID, and status.
- Use `malloc()` to allocate storage and `realloc()` to grow the metadata array when its capacity is insufficient.
- Keep the metadata array ordered by each block's starting offset.
- Use separate functions for allocation strategies, release, resize, merging, and statistics.
- Do not use a fixed-size array for the block metadata.
- Validate all sizes and IDs before modifying the memory map.

## 5. Suggested Menu

```text
1. Allocate memory
2. Release memory
3. Resize an allocation
4. Display memory map
5. Display memory statistics
6. Change allocation strategy
7. Exit
```

## 6. Example Scenario

Assume the user creates a 1000-byte pool and allocates 200 bytes, followed by 300 bytes. If the first allocation is then released, the map should show a 200-byte free region, the 300-byte allocation, and the remaining free region.

Subsequent allocation requests must use the selected strategy and must not overlap existing allocations.

## 7. Constraints

- The implementation must use a dynamic array for block metadata; linked lists are not allowed.
- If an allocation or resize cannot be completed, the existing memory map and allocated data must remain valid.
- Adjacent free blocks must be merged after a release or any operation that creates neighboring free regions.
- The sum of all block sizes must always equal the total pool size.
- The block-metadata array must remain ordered by starting offset, and blocks must cover the pool without gaps or overlaps.
- The memory pool and block-metadata array must both be dynamically allocated; fixed-size arrays must not be used for the block metadata.
- Allocation IDs must be positive and unique for every successful allocation.
- Each allocation request must be greater than 0 and no larger than the total pool size.
- The memory pool size must be between 1 and 1,000,000 bytes.

## 8. Sample Input/Output

The following example illustrates a First Fit session. The exact wording and formatting of your program's output may differ, but the results must be logically equivalent.

```text
Enter memory pool size: 1000
Select allocation strategy (1. First Fit, 2. Best Fit, 3. Worst Fit): 1
Strategy set to First Fit.

Menu:
1. Allocate memory
2. Release memory
3. Resize allocation
4. Display memory map
5. Display memory statistics
6. Change allocation strategy
7. Exit

Choice: 1
Requested size: 200
Allocation successful. ID: 1, Start: 0, Size: 200

Choice: 1
Requested size: 300
Allocation successful. ID: 2, Start: 200, Size: 300

Choice: 2
Allocation ID: 1
Allocation released.

Choice: 1
Requested size: 250
Allocation successful. ID: 3, Start: 500, Size: 250

Choice: 4
Memory Map (1000 bytes):
[Free: 200 bytes, Start: 0]
[Allocated #2: 300 bytes, Start: 200]
[Allocated #3: 250 bytes, Start: 500]
[Free: 250 bytes, Start: 750]

Choice: 1
Requested size: 300
Allocation failed: no contiguous free block is large enough.

Choice: 5
Total memory: 1000 bytes
Allocated: 550 bytes
Free: 450 bytes
Free blocks: 2
Largest free block: 250 bytes
Note: The request for 300 bytes failed even though 450 bytes are free, because the free memory is fragmented.
```

## Assignment Submission

### GitHub Repository Setup

- Create a public GitHub repository (accessible by mentors) for submitting Kalpavriksha assignments throughout the program.
- Create a separate branch for assignment submission and raise a Pull Request (PR) for this branch.

### Submission Deadline

- The assignments should be done in C language.
- The final submission deadline is **5th November, 12:00 PM**.
- Late submissions will not be accepted under any circumstances.
- Any commits made to the repository after 5th November, 12:00 PM will be disregarded.

### Prohibited AI Usage

- The use of AI or automated tools to complete the assignment is strictly prohibited.
- Assignments should be completed independently.

### Assignment Review and Compliance

- Mentors will thoroughly review each assignment submission.
"""

output_path = Path("/mnt/data/Smart_Memory_Manager.md")
output_path.write_text(content, encoding="utf-8")
print(f"Created: {output_path}")

from pathlib import Path

content = """# C Programming Assignment

# Smart Memory Manager

## 1. Problem Statement

Implement a simplified memory manager that simulates allocating and releasing regions of a fixed-size memory pool. The program must manage one dynamically allocated byte buffer and a separate dynamically allocated array of metadata records describing the regions in that buffer.

The pool is divided into contiguous blocks. Each block is either allocated to a request or available for future allocation. As requests are processed, blocks may need to be split, merged, resized, or moved. The program must keep the memory map consistent after every operation.

## 2. Objectives

- Use `malloc()`, `realloc()`, and `free()` appropriately.
- Represent memory regions using structures stored in a dynamically allocated array.
- Implement and compare First Fit, Best Fit, and Worst Fit allocation strategies.
- Split free blocks during allocation and merge adjacent free blocks after release.
- Resize existing allocations while preserving their data.
- Calculate memory usage and identify external fragmentation.
- Handle invalid requests and allocation failures without corrupting the pool.

## 3. Functional Requirements

### 3.1 Initialize the Memory Pool

- Ask the user for the total pool size in bytes.
- Allocate the pool dynamically as a byte buffer. Entire pool is initially one free block.
- Reject invalid pool sizes and handle allocation failures safely.

### 3.2 Allocate Memory

At program startup, allow the user to choose First Fit, Best Fit, or Worst Fit. Keep the selected strategy active for subsequent allocation requests until the user changes it through the menu.

Allow the user to request a block of a specified size. The strategies are:

- **First Fit:** Select the first free block large enough for the request.
- **Best Fit:** Select the smallest free block that can satisfy the request.
- **Worst Fit:** Select the largest free block that can satisfy the request.

On success, assign a unique allocation ID. If the chosen free block is larger than the request, split it into an allocated block and a remaining free block. If no suitable block exists, report that the request failed.

### 3.3 Release Memory

- Release an allocation using its allocation ID.
- Reject unknown IDs and attempts to release an already-free block.
- Mark the released region as free.
- Merge adjacent free blocks so the metadata does not contain unnecessarily separate free regions.

### 3.4 Resize an Allocation

Allow the user to increase or decrease the size of an existing allocation.

- If possible, expand into an adjacent free block without moving the allocation.
- If in-place expansion is not possible, search for another suitable free region, move the existing data, and release the old region.
- If resizing fails, the original allocation and its data must remain valid and unchanged.
- If shrinking creates a free region, update the metadata and merge adjacent free blocks where applicable.

### 3.5 Display the Memory Map

Display blocks in address order, including each block's starting offset, size, and status. Allocated blocks must show their allocation IDs.

Example (illustrative):

```text
Memory pool: 1000 bytes
[Allocated #1: 120 bytes] [Free: 80 bytes]
[Allocated #2: 250 bytes] [Free: 150 bytes]
[Allocated #3: 100 bytes] [Free: 300 bytes]
```

### 3.6 Display Memory Statistics

- Total pool size.
- Total allocated bytes and total free bytes.
- Number of allocated blocks and free blocks.
- Size of the largest contiguous free block.

### 3.7 Detect External Fragmentation

Report when a request cannot be satisfied even though total free memory is greater than or equal to the requested size. Explain the situation using the sizes of the individual free blocks.

## 4. Data Structure and Design Constraints

- Use a dynamically allocated byte buffer for the simulated memory pool.
- Use a separate dynamically allocated array of structures to describe the blocks. Do not use a linked list.
- Structure should represent the block and it should contain block's starting offset, size, allocation ID, and status.
- Use `malloc()` to allocate storage and `realloc()` to grow the metadata array when its capacity is insufficient.
- Keep the metadata array ordered by each block's starting offset.
- Use separate functions for allocation strategies, release, resize, merging, and statistics.
- Do not use a fixed-size array for the block metadata.
- Validate all sizes and IDs before modifying the memory map.

## 5. Suggested Menu

```text
1. Allocate memory
2. Release memory
3. Resize an allocation
4. Display memory map
5. Display memory statistics
6. Change allocation strategy
7. Exit
```

## 6. Example Scenario

Assume the user creates a 1000-byte pool and allocates 200 bytes, followed by 300 bytes. If the first allocation is then released, the map should show a 200-byte free region, the 300-byte allocation, and the remaining free region.

Subsequent allocation requests must use the selected strategy and must not overlap existing allocations.

## 7. Constraints

- The implementation must use a dynamic array for block metadata; linked lists are not allowed.
- If an allocation or resize cannot be completed, the existing memory map and allocated data must remain valid.
- Adjacent free blocks must be merged after a release or any operation that creates neighboring free regions.
- The sum of all block sizes must always equal the total pool size.
- The block-metadata array must remain ordered by starting offset, and blocks must cover the pool without gaps or overlaps.
- The memory pool and block-metadata array must both be dynamically allocated; fixed-size arrays must not be used for the block metadata.
- Allocation IDs must be positive and unique for every successful allocation.
- Each allocation request must be greater than 0 and no larger than the total pool size.
- The memory pool size must be between 1 and 1,000,000 bytes.

## 8. Sample Input/Output

The following example illustrates a First Fit session. The exact wording and formatting of your program's output may differ, but the results must be logically equivalent.

```text
Enter memory pool size: 1000
Select allocation strategy (1. First Fit, 2. Best Fit, 3. Worst Fit): 1
Strategy set to First Fit.

Menu:
1. Allocate memory
2. Release memory
3. Resize allocation
4. Display memory map
5. Display memory statistics
6. Change allocation strategy
7. Exit

Choice: 1
Requested size: 200
Allocation successful. ID: 1, Start: 0, Size: 200

Choice: 1
Requested size: 300
Allocation successful. ID: 2, Start: 200, Size: 300

Choice: 2
Allocation ID: 1
Allocation released.

Choice: 1
Requested size: 250
Allocation successful. ID: 3, Start: 500, Size: 250

Choice: 4
Memory Map (1000 bytes):
[Free: 200 bytes, Start: 0]
[Allocated #2: 300 bytes, Start: 200]
[Allocated #3: 250 bytes, Start: 500]
[Free: 250 bytes, Start: 750]

Choice: 1
Requested size: 300
Allocation failed: no contiguous free block is large enough.

Choice: 5
Total memory: 1000 bytes
Allocated: 550 bytes
Free: 450 bytes
Free blocks: 2
Largest free block: 250 bytes
Note: The request for 300 bytes failed even though 450 bytes are free, because the free memory is fragmented.
```

## Assignment Submission

### GitHub Repository Setup

- Create a public GitHub repository (accessible by mentors) for submitting Kalpavriksha assignments throughout the program.
- Create a separate branch for assignment submission and raise a Pull Request (PR) for this branch.

### Submission Deadline

- The assignments should be done in C language.
- The final submission deadline is **5th November, 12:00 PM**.
- Late submissions will not be accepted under any circumstances.
- Any commits made to the repository after 5th November, 12:00 PM will be disregarded.

### Prohibited AI Usage

- The use of AI or automated tools to complete the assignment is strictly prohibited.
- Assignments should be completed independently.

### Assignment Review and Compliance

- Mentors will thoroughly review each assignment submission.