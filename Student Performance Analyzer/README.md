
# Assignment

## 1. Student Performance Analyzer

Design a console-based Student Performance Analyzer program in C that accepts student
details as input and evaluates their academic performance based on marks obtained in
three subjects.

The program should calculate total marks, average marks, assign a grade, and display a
performance pattern using stars (`*`) based on the grade. It should also demonstrate
programming concepts like arithmetic operators, control flow, loops, functions,
recursion, structures, and variable scope.

### Requirements

- The program should use a structure to store student details: Roll Number, Name, Marks
  in three subjects.
- The program should calculate Total Marks and Average Marks using arithmetic operators.
- Assign a Grade using if-else or switch-case logic.

| Average | Grade |
|----------|-------|
| ≥ 85 | A |
| ≥ 70 | B |
| ≥ 50 | C |
| ≥ 35 | D |
| < 35 | F |

- Display a Performance Pattern of stars based on the grade (A: 5, B: 4, C: 3, D: 2).
- If a student’s average is below 35, skip printing the star pattern using `continue`
  statement.
- Include loops to iterate over multiple students, functions to compute
  total/average/grade, recursion to print roll numbers, and demonstrate variable scope.
- Use proper input/output formatting to clearly show results for each student.

### Constraints

- Number of students: 1 ≤ N ≤ 100
- Marks in each subject: 0 ≤ Marks ≤ 100

### Input Format

First line: An integer N (number of students)

Next N lines: (Each line contains the following data separated by spaces)

```text
Roll_Number1 Name1 Marks1 Marks2 Marks3
Roll_Number2 Name2 Marks1 Marks2 Marks3
````

### Output Format

For each student, print the following details:

```text
Roll: <roll_number>
Name: <name>
Total: <total_marks>
Average: <average_marks>
Grade: <grade>
Performance: <pattern of * based on grade>
```

At the end, print:

```text
List of Roll Numbers (via recursion): 1 2 3 ... N
```

### Test Cases

#### Input

```text
2
1 Arti 78 82 90
2 Meena 32 28 35
```

#### Output

```text
Roll: 1
Name: Arti
Total: 250
Average: 83.33
Grade: B
Performance: ****
Roll: 2
Name: Meena
Total: 95
Average: 31.67
Grade: F
List of Roll Numbers (via recursion): 1 2
```

