**# Assignment 1: Calculator**

**### Problem Statement**

Design a console-based calculator program in C that accepts a mathematical expression as input in the form of a string. The program should evaluate the expression and return the result.

The calculator should support the following operations:

* Addition (`+`)
* Subtraction (`-`)
* Multiplication (`*`)
* Division (`/`)

The program should follow the order of operations (DMAS), where multiplication and division are performed before addition and subtraction. Integer operations should be used, and the result should be displayed as an integer even when division has a remainder.

**### Requirements**

* The input should be a mathematical expression in the form of a string.
* The expression should contain integers and the operators `+`, `-`, `*`, and `/`.
* Whitespace between numbers and operators should be ignored.
* The program should output the result as an integer.
* Division by zero should display:
  `Error: Division by zero.`
* Invalid expressions or invalid characters should display:
  `Error: Invalid expression.`
* The program should follow the order of operations (DMAS).
* Multiplication and division should be performed before addition and subtraction.
* The program should correctly handle operator precedence and associativity.

**### Input Format**

The input consists of a single line containing a mathematical expression as a string.

**### Output Format**

The output should be either:

* The calculated result as an integer, or
* `Error: Division by zero.` if division by zero occurs, or
* `Error: Invalid expression.` if the expression is invalid.

**### Test Cases**

**Test Case 1**

Input:

```text
3 + 5 * 2
```

Output:

```text
13
```

**Test Case 2**

Input:

```text
3 + a * 2
```

Output:

```text
Error: Invalid expression.
```
