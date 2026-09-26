#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* input(char* prompt) {
    printf("%s", prompt);
    char* string = NULL;
    scanf("%m[^\n]s", &string);
    return string;
}

int isDigit(char c) {
    return c >= '0' && c <= '9';
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

int applyOperator(double a, double b, char op, double* result) {
    switch(op) {
        case '+':
            *result = a + b;
            return 0;
        case '-':
            *result = a - b;
            return 0;
        case '*':
            *result = a * b;
            return 0;
        case '/':
            if (b == 0) return 1;
            *result = a / b;
            return 0;
        default:
            return 1;
    }
}


int evaluateExpression(char* expression, double* result) {
    const int size = strlen(expression);

    double* values = (double*)malloc(sizeof(double) * size);
    char* operators = (char*)malloc(sizeof(char) * size);
    int valuesTop = -1, operatorsTop = -1;

    for(int i=0; i<size; i++) {
        char ch = expression[i];

        if(ch == ' ') continue;

        if(isDigit(ch)) {
            double num = 0;
            while(i < size && isDigit(expression[i])) {
                num = num * 10 + (expression[i] - '0');
                i += 1;
            }

            values[++valuesTop] = num;
            i -= 1;
            continue;
        }

        if(operatorPrecedence(ch) == 0) {
            printf("Error: Invalid expression.\n");
            return 1;
        }

        while(
            operatorsTop >= 0 && 
            operatorPrecedence(ch) <= operatorPrecedence(operators[operatorsTop])
        ) {
            double b = values[valuesTop--];
            double a = values[valuesTop--];
            char operator = operators[operatorsTop--];

            int error = applyOperator(a, b, operator, &values[++valuesTop]);
            if(error == 1) return 1;
        }

        operators[++operatorsTop] = ch;
    }
    
    while(operatorsTop >= 0) {
        double b = values[valuesTop--];
        double a = values[valuesTop--];
        char operator = operators[operatorsTop--];
        
        int error = applyOperator(a, b, operator, &values[++valuesTop]);
        if(error == 1) return 1;
    }
    
    *result = values[0];

    free(values);
    free(operators);

    return 0;
}


int main() {
    char* expression = input("Enter you expression: ");

    double result = 0;
    int error = evaluateExpression(expression, &result);

    if(error == 1) {
        printf("Error: Expression evaluation was failed.\n");
        return 0;
    }

    printf("Result: %.2lf\n", result);

    return 0;
}