# Employee Hierarchy Management System

## Objective

Develop a C program to manage an organization's employee hierarchy. Each employee may report to one manager and may have multiple employees reporting to them. The program should use structures, pointers, functions and recursion build and manage the organization hierarchy.

## Employee Structure

Each employee should contain:

- Employee ID
- Name
- Designation
- Reports – employees who report directly to the current employee
- Reporting Manager – employee to whom the current employee reports

## Tasks

### 1. Add an Employee

Allow the user to add a new employee by providing Employee ID, Name, Designation, and Reporting Manager. If the employee is at the top level of the organization, they will have no reporting manager. When an employee is added under a manager, the employee should also be added to that manager's list of reports.

### 2. Remove an Employee

Allow the user to remove an employee from the organization. When removing an employee, remove them from their reporting manager's list and handle their reporting employees appropriately while maintaining a valid organization hierarchy.

### 3. Display Hierarchy of an Employee

Ask for an Employee ID and display that employee along with all employees reporting under them. Also display the total number of employees under the selected employee. (Use recursion)

### 4. Display Complete Hierarchy

Display the complete organization hierarchy starting from the top-level employee(s), along with the total number of employees. (Use recursion)

## Recursion Requirement

The employee hierarchy must be traversed using recursion. Recursive functions should be used for displaying employees under a particular employee, counting employees under an employee, and displaying the complete hierarchy. The recursive function should process the current employee and recursively process each employee reporting to them.

**Important:** Do not use a loop to replace the recursive traversal of the hierarchy.

## Constraints

1. Employee IDs must be unique.
2. An employee can report to at most one manager.
3. An employee can have zero, one, or multiple reporting employees.
4. An employee cannot report to themselves.
5. The hierarchy must not contain circular reporting relationships.
6. A top-level employee has no reporting manager.
7. Use recursion for hierarchy traversal and counting.

## Menu

```text
===== EMPLOYEE HIERARCHY MANAGEMENT =====

1. Add Employee
2. Remove Employee
3. Display Employee Hierarchy
4. Display Complete Hierarchy
5. Exit

Enter your choice:
```

## Sample Test Case

### Initial Employees

```text
101 Rahul CEO
102 Amit Engineering Manager         -> 101
103 Priya HR Manager                 -> 101
104 Neha Developer                   -> 102
105 Raj Developer                    -> 102
106 Karan HR Executive               -> 103
```

### Display Complete Hierarchy

```text
===== ORGANIZATION HIERARCHY =====

101 - Rahul - CEO
    102 - Amit - Engineering Manager
        104 - Neha - Developer
        105 - Raj - Developer
    103 - Priya - HR Manager
        106 - Karan - HR Executive

Total Employees: 6
```

### Display Hierarchy of Employee 102

```text
102 - Amit - Engineering Manager
    104 - Neha - Developer
    105 - Raj - Developer

Total Employees Under 102: 2
```

---

## Assignment Submission

### GitHub Repository Setup

- Create a public GitHub repository (should be accessible by mentors) for submitting Kalpavriksha assignments throughout the program.
- Create a separate branch for assignment submission and raise a Pull Request (PR) for this branch.

## Submission Deadline

- The assignments should be done in C language.
- The final submission deadline is **22nd October, 12:00 PM**.
- Late submissions will not be accepted under any circumstances.
- Any commits made to the repository after 22nd October, 12:00 PM will be disregarded.

## Prohibited AI Usage

- The use of AI or automated tools to complete the assignment is strictly prohibited.
- Assignments should be completed independently.

## Assignment Review and Compliance

- Mentors will thoroughly review each assignment submission.