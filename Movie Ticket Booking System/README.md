# Assignment

## C Programming Fundamentals

## 1. Movie Ticket Booking System

Design a console-based program in C for a small cinema ticket counter. The program should accept customer and booking details, calculate the ticket cost, and apply a discount when applicable. This assignment practises variables, data types, constants, input/output, arithmetic operators, and decision-making using if-else and switch-case.

### Requirements

- Read the customer name, number of tickets, and ticket category from the user.
- Support three ticket categories: Standard, Premium, and Recliner. The user enters 1, 2, or 3 to select a category.
- Use suitable data types for the customer name, ticket count, category choice, ticket price, discount, and final bill.
- Declare ticket prices as constants: Standard = 150, Premium = 250, and Recliner = 400.
- Use `switch-case` to select the ticket price based on the chosen category.
- Calculate the subtotal using arithmetic operators.
- Use `if-else` to apply a 10% discount when the customer books 5 or more tickets. Otherwise, no discount is applied.
- Display the selected category, number of tickets, subtotal, discount amount, and final bill. Format monetary values to two decimal places.
- Display an appropriate error message if the category choice or ticket count is invalid.

### Ticket Categories

| Choice | Category | Price per Ticket |
|---:|---|---:|
| 1 | Standard | 150 |
| 2 | Premium | 250 |
| 3 | Recliner | 400 |

### Discount Rule

If the customer books 5 or more tickets, apply a 10% discount to the subtotal. For fewer than 5 tickets, the discount is 0.

### Constraints

1. The program must be written in C.
2. Number of tickets must be between 1 and 20, inclusive.
3. Ticket category must be 1, 2, or 3.
4. Use `switch-case` for ticket category selection and `if-else` for the discount rule.
5. Use arithmetic operators to calculate subtotal, discount, and final bill.

### Input Format

Read the following inputs in order:

1. Customer name (a single name without spaces)
2. Number of tickets
3. Ticket category choice (1 = Standard, 2 = Premium, 3 = Recliner)

### Output Format

For valid input, display:

- `Customer: <customer_name>`
- `Ticket Category: <category>`
- `Tickets: <number_of_tickets>`
- `Subtotal: <subtotal>`
- `Discount: <discount_amount>`
- `Final Bill: <final_bill>`

### Sample Test Case 1

**Input:**

```text
Asha
5
2
```

**Output:**

```text
Customer: Asha
Ticket Category: Premium
Tickets: 5
Subtotal: 1250.00
Discount: 125.00
Final Bill: 1125.00
```

### Sample Test Case 2

**Input:**

```text
Ravi
2
1
```

**Output:**

```text
Customer: Ravi
Ticket Category: Standard
Tickets: 2
Subtotal: 300.00
Discount: 0.00
Final Bill: 300.00
```

---

## Assignment Submission

### GitHub Repository Setup

- Create a public GitHub repository (should be accessible by mentors) for submitting Kalpavriksha assignments throughout the program.
- Create a separate branch for assignment submission and raise a Pull Request (PR) for this branch.

### Submission Deadline

- The assignments should be done in C language.
- The final submission deadline is **15th October, 12:00 PM**.
- Late submissions will not be accepted under any circumstances.
- Any commits made to the repository after 15th October, 12:00 PM will be disregarded.

### Prohibited AI Usage

- The use of AI or automated tools to complete the assignment is strictly prohibited.
- Assignments should be completed independently.

### Assignment Review and Compliance

- Mentors will thoroughly review each assignment submission.
