# Sonar Image Processing

## Problem Description

You are working as a software engineer at a marine research lab analyzing sonar images to track whale movements.

Each sonar image is represented by an `N × N` matrix of integer intensity values ranging from 0 to 255.

Due to limited onboard memory, your task is to:

1. Generate a random `N × N` matrix of sonar intensity values.
2. Rotate the matrix 90° clockwise in-place by swapping pointers (addresses) instead of values, using only pointer arithmetic.
3. Apply a 3×3 smoothing filter in-place on the rotated matrix using only pointers.

All operations must be done in-place, without using any extra `N × N` matrices or array indexing.

## Tasks

### 1. Matrix Generation

- Take an integer `n` as input (`2 ≤ n ≤ 10`).
- Generate a random `N × N` matrix where each cell has a random integer between 0 and 255.
- Display the generated matrix.

### 2. Rotate Matrix 90° Clockwise (In-Place)

- Must be performed in-place using only pointer arithmetic (no `matrix[i][j]`).
- Rotation should be done by swapping addresses (pointers), not by swapping or copying the actual values.

### 3. Apply 3×3 Smoothing Filter (In-Place)

- For each cell, replace its value with the average of itself and its valid neighbors within a 3×3 window.
- For edge and corner cells, include only existing valid neighbors (no out-of-bound access).
- Perform this using only pointer access and without creating any additional `N × N` array.

## Input Format

```text
Enter matrix size (2-10): <n>
```

## Output Format

```text
Original Randomly Generated Matrix:
<Matrix>

Matrix after 90° Clockwise Rotation:
<Matrix>

Matrix after Applying 3×3 Smoothing Filter:
<Matrix>
```

## Example Output: Rotation and Smoothing

### Original Matrix

```text
12   60   90
200  150  30
10   220  180
```

### Rotated Matrix

```text
10   200  12
220  150  60
180  30   90
```

### Final Output

```text
145  115  80
125  105  78
145  90   67
```

## Constraints

- `2 ≤ N ≤ 10`
- `0 ≤ cell value ≤ 255`
- Only pointer-based access allowed; no array indexing permitted.
- No extra `N × N` matrix allowed.
- Only a few temporary variables for swapping, summing, and counting are allowed.
- Extra space: `O(N)` at most.

## 3×3 Smoothing Filter Example

### Original 5×5 Matrix

```text
10  20  30  40  50
15  25  35  45  55
20  30  40  50  60
25  35  45  55  65
30  40  50  60  70
```

### Neighbors and Smoothed Values

| Cell | Neighbors | Smoothed Value |
|---:|---|---:|
| 10 | 10, 20, 15, 25 | 17 |
| 20 | 10, 20, 30, 15, 25, 35 | 22 |
| 30 | 20, 30, 40, 25, 35, 45 | 30 |
| 40 | 30, 40, 50, 35, 45, 55 | 40 |
| 50 | 40, 50, 45, 55 | 48 |
| 15 | 10, 20, 15, 25, 20, 30 | 20 |
| 25 | 10, 20, 30, 15, 25, 35, 20, 30, 40 | 27 |
| 35 | 20, 30, 40, 25, 35, 45, 30, 40, 50 | 35 |
| 45 | 30, 40, 50, 35, 45, 55, 40, 50, 60 | 45 |
| 55 | 40, 50, 45, 55, 50, 60 | 50 |
| 20 | 15, 25, 20, 30, 25, 35 | 25 |
| 30 | 15, 25, 35, 20, 30, 40, 25, 35, 45 | 32 |
| 40 | 25, 35, 45, 30, 40, 50, 35, 45, 55 | 40 |
| 50 | 35, 45, 55, 40, 50, 60, 45, 55, 65 | 50 |
| 60 | 45, 55, 50, 60, 55, 65 | 55 |
| 25 | 20, 30, 25, 35, 30, 40 | 30 |
| 35 | 20, 30, 40, 25, 35, 45, 30, 40, 50 | 37 |
| 45 | 30, 40, 50, 35, 45, 55, 40, 50, 60 | 45 |
| 55 | 35, 45, 55, 40, 50, 60, 45, 55, 65 | 55 |
| 65 | 45, 55, 50, 60, 55, 65 | 60 |
| 30 | 25, 35, 30, 40 | 32 |
| 40 | 25, 35, 45, 30, 40, 50 | 40 |
| 50 | 30, 40, 50, 35, 45, 55 | 50 |
| 60 | 35, 45, 55, 40, 50, 60 | 57 |
| 70 | 45, 55, 60, 65, 70 | 63 |

### Smoothed Output Matrix

```text
17  22  30  40  48
20  27  35  45  50
25  32  40  50  55
30  37  45  55  60
32  40  50  57  63
```

## Assignment Submission

### GitHub Repository Setup

- Create a public GitHub repository (should be accessible by mentors) for submitting Kalpavriksha assignments throughout the program.
- Create a separate branch for assignment submission and raise a Pull Request (PR) for this branch.

### Submission Deadline

- The assignments should be done in C language.
- The final submission deadline is **29th October, 5:00 PM**.
- Late submissions will not be accepted under any circumstances.
- Any commits made to the repository after 29th October, 5:00 PM will be disregarded.

### Prohibited AI Usage

- The use of AI or automated tools to complete the assignment is strictly prohibited.
- Assignments should be completed independently.

### Assignment Review and Compliance

- Mentors will thoroughly review each assignment submission.