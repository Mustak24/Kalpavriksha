#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_EXPRESSION_LENGTH 512


typedef enum {
    SUCCESS = 0,
    ERROR_MEMORY_ALLOCATION_FAILED,
    ERROR_FAILED_TO_READ_INPUT,
    ERROR_EXPRESSION_MAX_LENGTH_EXCEEDED,
    ERROR_INVALID_EXPRESSION,
    ERROR_OVERFLOW_INTEGER,
    ERROR_DIVISION_BY_ZERO,
} StatusCode;

void printStatusMessage(StatusCode code) {
    switch(code) {
        case SUCCESS:
            printf("Success\n");
            break;
        case ERROR_MEMORY_ALLOCATION_FAILED:
            printf("Error: Memory allocation failed\n");
            break;
        case ERROR_FAILED_TO_READ_INPUT:
            printf("Error: Failed to read input\n");
            break;
        case ERROR_EXPRESSION_MAX_LENGTH_EXCEEDED:
            printf("Error: Expression max length exceeded, allow max is %d\n", MAX_EXPRESSION_LENGTH - 2);
            break;
        case ERROR_INVALID_EXPRESSION:
            printf("Error: Invalid expression\n");
            break;
        case ERROR_OVERFLOW_INTEGER:
            printf("Error: Integer overflow\n");
            break;
        case ERROR_DIVISION_BY_ZERO:
            printf("Error: Division by zero\n");
            break;
        default:
            printf("Error: Unknown error code\n");
    }
}


char* input(char* prompt) {
    if(prompt != NULL) {
        printf("%s", prompt);
    }

    char* string = (char*)malloc(sizeof(char) * MAX_EXPRESSION_LENGTH);
    if(string == NULL) {
        printStatusMessage(ERROR_MEMORY_ALLOCATION_FAILED);
        exit(1);
    }
    
    if(fgets(string, MAX_EXPRESSION_LENGTH, stdin) == NULL) {
        printStatusMessage(ERROR_FAILED_TO_READ_INPUT);
        free(string);
        exit(1);
    }

    size_t len = strlen(string);
    
    if(len == MAX_EXPRESSION_LENGTH-1 && string[len - 1] != '\n') {
        free(string);
        printStatusMessage(ERROR_EXPRESSION_MAX_LENGTH_EXCEEDED);
        exit(1);
    }

    if(len > 0 && string[len - 1] == '\n') {
        string[len - 1] = '\0';
    }

    return string;
}


int operatorPrecedence(char op) {
    switch (op) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        default:
            return 0;
    }
}

StatusCode applyOperator(long long a, long long b, char op, int* result) {
    long long tempResult;

    switch(op) {
        case '+':
            tempResult = a + b;
            break;
        case '-':
            tempResult = a - b;
            break;
        case '*':
            tempResult = a * b;
            break;
        case '/':
            if (b == 0) return ERROR_DIVISION_BY_ZERO;
            tempResult = a / b;
            break;
        default:
            return ERROR_INVALID_EXPRESSION;
    }

    if(tempResult > INT_MAX || tempResult < INT_MIN) {
        return ERROR_OVERFLOW_INTEGER;
    }

    *result = (int)tempResult;
    return SUCCESS;
}



StatusCode evaluateExpression(char* expression, int* result) {
    const int size = strlen(expression);

    int* values = (int*)malloc(sizeof(int) * size);
    if(values == NULL) {
        free(values);
        return ERROR_MEMORY_ALLOCATION_FAILED;
    }

    char* operators = (char*)malloc(sizeof(char) * size);
    if(operators == NULL) {
        free(values);
        free(operators);
        return ERROR_MEMORY_ALLOCATION_FAILED;
    }

    int valuesTop = -1, operatorsTop = -1;

    for(int i=0; i<size; i++) {
        char ch = expression[i];

        if(ch == ' ') continue;

        if(isdigit(ch)) {
            long long num = 0;
            while(i < size && isdigit(expression[i])) {
                num = num * 10 + (expression[i] - '0');
                i += 1;

                if(num > INT_MAX) {
                    free(values);
                    free(operators);
                    return ERROR_OVERFLOW_INTEGER;
                }
            }

            values[++valuesTop] = (int)num;
            i -= 1;
            continue;
        }

        if(operatorPrecedence(ch) == 0) {
            free(values);
            free(operators);
            return ERROR_INVALID_EXPRESSION;
        }

        while(
            operatorsTop >= 0 && 
            operatorPrecedence(ch) <= operatorPrecedence(operators[operatorsTop])
        ) {
            if(valuesTop < 1) {
                free(values);
                free(operators);
                return ERROR_INVALID_EXPRESSION;
            }

            int b = values[valuesTop--];
            int a = values[valuesTop--];
            char operator = operators[operatorsTop--];

            StatusCode status = applyOperator(a, b, operator, &values[++valuesTop]);

            if(status != SUCCESS) {
                free(values);
                free(operators);
                return status;
            }
        }

        operators[++operatorsTop] = ch;
    }
    
    while(operatorsTop >= 0) {
        if(valuesTop < 1) {
            free(values);
            free(operators);
            return ERROR_INVALID_EXPRESSION;
        }

        int b = values[valuesTop--];
        int a = values[valuesTop--];
        char operator = operators[operatorsTop--];
        
        StatusCode status = applyOperator(a, b, operator, &values[++valuesTop]);
        if(status != SUCCESS) {
            free(values);
            free(operators);
            return status;
        }
    }
    
    if(valuesTop != 0) {
        free(values);
        free(operators);
        return ERROR_INVALID_EXPRESSION;
    }

    *result = values[0];

    free(values);
    free(operators);

    return SUCCESS;
}


int main() {
    char* expression = input("Enter you expression: ");

    int result = 0;
    StatusCode status = evaluateExpression(expression, &result);

    if(status == SUCCESS) {
        printf("Result: %d\n", result);
    } else {
        printStatusMessage(status);
    }

    free(expression);
    return 0;
}