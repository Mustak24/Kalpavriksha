# Virtual File System (VFS) – Assignment & Explanation

## Problem Statement

You are tasked with building a Virtual File System (VFS) that simulates an in-memory file storage environment. The system should allow users to create, read, write, and manage files and directories using circular and doubly linked lists as the core data structures. The purpose is to simulate how an actual file system manages memory, directory structures, and disk block allocations.

## Core Initialization and Design Overview

At program startup, the virtual disk and root directory must be initialized. The virtual disk is represented as a two-dimensional byte array, while the free blocks are organized in a doubly linked list for easy allocation and deallocation. The root directory `/` is created during initialization and serves as the starting point of the filesystem hierarchy.

## Free Block Management

The virtual disk is divided into fixed-size blocks (e.g., 512 bytes). Each block represents a unit of storage, and all unallocated blocks are maintained in a doubly linked list of `FreeBlock` nodes.

When the system starts, all blocks are free and inserted into the free block list. Each `FreeBlock` node contains an index (indicating its position in the disk array) and `next`/`prev` pointers.

- When a file is written: required number of blocks are removed from the head of the free list.
- When a file is deleted: its blocks are reinserted at the tail of the free list.
- When disk is full: file write operations must fail with an appropriate message.

## Directory and File Structure

Each directory and file is represented by a `FileNode` structure. Directories maintain a circular linked list of their child entries, which may include files or other subdirectories. Files, on the other hand, use `blockPointers[]` to reference actual disk blocks used to store their content.

Directories do not consume disk blocks. They exist purely in memory as linked lists. Files consume disk space based on their data size, calculated as:

```text
contentSize / BLOCK_SIZE
```

## Current Working Directory (CWD)

The current working directory (`cwd`) pointer indicates where the user currently is in the filesystem. At startup, `cwd` points to the root directory (`/`). When `mkdir` or `create` is executed, the new node is added to the circular list of `cwd`. The `cwd` only changes when the user executes a `cd` command.

- On startup → `cwd = '/'`
- `mkdir folder1` → `cwd` still `/`
- `cd folder1` → `cwd = '/folder1'`
- `create notes.txt` → file created under `/folder1`, `cwd` unchanged
- `cd ..` → `cwd` moves to parent directory

## Operation Explanations

- **`mkdir`**: Creates a new directory under the current directory. Inserts a new node into the circular linked list.
- **`create`**: Creates a new file node under the current directory. Initially has zero allocated blocks.
- **`write`**: Allocates one or more blocks from the free list, writes content, updates file `blockPointers[]`.
- **`read`**: Reads file content by traversing the `blockPointers[]` array and printing data from `virtualDisk[]`.
- **`delete`**: Deletes a file, removes it from the directory’s circular list, and returns its blocks to the free list.
- **`rmdir`**: Removes an empty directory. Fails if it still contains files or subdirectories.
- **`ls`**: Lists all entries (files and directories) under the current directory by traversing its circular list.
- **`cd`**: Changes the current working directory by updating the `cwd` pointer.
- **`pwd`**: Displays the current directory path by traversing parent pointers up to root.
- **`df`**: Displays disk usage (total blocks, used blocks, free blocks).
- **`exit`**: Releases all memory, clears linked lists, and terminates the program.

## Linked List Behavior

### Inserting a `FileNode`

- If the list is empty, the new node points to itself (circular behavior).
- Otherwise, the node is inserted at the end, maintaining circular linkage.

### Deleting a `FileNode`

- Neighbor pointers are adjusted to bypass the node.
- If it was the only node, the list becomes empty.

### Removing a `FreeBlock`

- The head block is removed and reassigned to a file.
- The new head becomes the next node in the list.

### Freeing a Block

- A new `FreeBlock` node is appended at the tail of the list.

## Constraints

- `BLOCK_SIZE = 512` bytes
- `1 ≤ NUM_BLOCKS ≤ 5000`
- File and directory names ≤ 50 characters
- Commands are case-sensitive
- Directories must be empty before deletion

## Test Cases

| Command | Expected behavior |
|---|---|
| `/ > mkdir docs` | Creates directory `docs` under root. `cwd = '/'`. Verifies insertion in circular list. |
| `/ > cd docs` | Updates `cwd` to `/docs`. |
| `/docs > create notes.txt` | Creates file under `cwd`. No blocks allocated yet. |
| `/docs > write notes.txt 'Hello'` | Allocates 1 block from free list, updates file’s `blockPointers`. |
| `/docs > read notes.txt` | Reads and prints content from allocated blocks. |
| `/docs > delete notes.txt` | Removes file node and returns its blocks to free list. |
| `/ > rmdir docs` | Removes empty directory `docs`. |
| `/ > df` | Displays total, used, and free blocks. |
| `/ > exit` | Frees all nodes and gracefully exits. |

## Console Representation of All Operations

### 🏁 System Startup

```console
$ ./vfs

Compact VFS - ready. Type 'exit' to quit.

/ >
```

### `mkdir` (Create Directory)

```console
/ > mkdir docs
Directory 'docs' created successfully.
```

```console
/ > ls docs/
```

### 📂 `cd` (Change Directory)

```console
/ > cd docs
Moved to /docs

docs >
```

### 📄 `create` (Create File)

```console
docs > create notes.txt
File 'notes.txt' created successfully.

docs > ls
notes.txt
```

### ✍️ `write` (Write to File)

```console
docs > write notes.txt "This is my first note."
Data written successfully (size=24 bytes).

docs > df
Total Blocks: 1024
Used Blocks: 1
Free Blocks: 1023
Disk Usage: 0.10%
```

### 📖 `read` (Read File)

```console
docs > read notes.txt
This is my first note.
```

### 🗑️ `delete` (Delete File)

```console
docs > delete notes.txt
File deleted successfully.

docs > ls
(empty)

docs > df
Total Blocks: 1024
Used Blocks: 0
Free Blocks: 1024
Disk Usage: 0.00%
```

### 📂 `rmdir` (Remove Directory)

```console
docs > cd ..
Moved to /

/ > rmdir docs
Directory removed successfully.

/ > ls
(empty)
```

### 🧭 `pwd` (Show Current Path)

```console
/ > mkdir projects
/ > cd projects
Moved to /projects

projects > pwd
/projects
```

### 📊 `df` (Show Disk Usage)

```console
/ > df
Total Blocks: 1024
Used Blocks: 0
Free Blocks: 1024
Disk Usage: 0.00%
```

### 🚪 `exit` (Terminate Program)

```console
/ > exit
Memory released. Exiting program...
```

## Guidelines

### GitHub Repository Setup

- Create a public GitHub repository (accessible by mentors) for submitting Kalpavriksha assignments throughout the program.
- Create a separate branch for assignment submission and raise a Pull Request (PR) for this branch.

### Submission Deadline

- The assignments should be done in C language.
- The final submission deadline is **17th November, 12:00 PM**.
- Late submissions will not be accepted under any circumstances.
- Any commits made to the repository after **17th November, 12:00 PM** will be disregarded.

### Prohibited AI Usage

- The use of AI or automated tools to complete the assignment is strictly prohibited.
- Assignments should be completed independently.

### Assignment Review and Compliance

- Mentors will thoroughly review each assignment submission.