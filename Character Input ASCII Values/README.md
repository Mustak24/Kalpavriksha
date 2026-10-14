# Character Input and ASCII Values

## Problem Statement

Write a C program that reads an integer `n`, followed by `n` characters. For each character, print the character, its ASCII value, and the next character in sequence, all on one line separated by spaces. After processing all characters, print the sum of all the ASCII values.

## Constraints

- `1 ≤ n ≤ 20`
- Each character is a lowercase letter from `a` to `y` (so the next character is always a letter).
- The characters may be entered separated by spaces (`a b c d e f`) or together (`abcdef`). Your program must work for both.
- Use a loop that runs exactly `n` times, with one character read per iteration.

## Input Format

The first line contains the integer `n`. The second line contains `n` characters.

## Output Format

- `n` lines, each in the form: `<char> <ASCII value> <next char>`
- The last line is: `Sum = <total>`

## Sample Input 1

```text
6
a b c d e f
```

## Sample Output 1

```text
a 97 b
b 98 c
c 99 d
d 100 e
e 101 f
f 102 g
Sum = 597
```

## Sample Input 2

```text
3
abc
```

## Sample Output 2

```text
a 97 b
b 98 c
c 99 d
Sum = 294
```