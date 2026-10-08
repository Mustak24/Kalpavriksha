#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define BUFFER_MAX_SIZE 128

#define STUDENT_MIN_COUNT 1
#define STUDENT_MAX_COUNT 100

#define NUMBER_OF_MARKS 3
#define STUDENT_MIN_MARKS 0.0f
#define STUDENT_MAX_MARKS 100.0f

#define GRADE_A_MIN_MARKS 85.0f
#define GRADE_B_MIN_MARKS 70.0f
#define GRADE_C_MIN_MARKS 50.0f
#define GRADE_D_MIN_MARKS 35.0f


typedef struct Student {
    unsigned int rollNumber;
    char name[50];
    float marks[NUMBER_OF_MARKS];
} Student;


bool isValidMarks(float marks[NUMBER_OF_MARKS]) {
    for(int index=0; index<NUMBER_OF_MARKS; index++) {
        if(
            !isfinite(marks[index]) || 
            marks[index] < STUDENT_MIN_MARKS ||
            marks[index] > STUDENT_MAX_MARKS
        ) {
            return false;
        }
    }
    return true;
}


void cleanInputBuffer() {
    int bufferChar;
    while((bufferChar = getchar()) != '\n' && bufferChar != EOF);
}


float getTotalMarks(const Student* student) {
    float total = 0.0f;
    for(int index=0; index<NUMBER_OF_MARKS; index++) {
        total += student->marks[index];
    }
    return total;
}

float getAverageMarks(const Student* student) {
    return getTotalMarks(student) / (float)NUMBER_OF_MARKS;
}

char getGrade(float average) {
    if(average >= GRADE_A_MIN_MARKS) return 'A';
    if(average >= GRADE_B_MIN_MARKS) return 'B';
    if(average >= GRADE_C_MIN_MARKS) return 'C';
    if(average >= GRADE_D_MIN_MARKS) return 'D';
    return 'F';
}

const char* getPerformanceStars(char grade) {
    switch(grade) {
        case 'A': return "*****";
        case 'B': return "****";
        case 'C': return "***";
        case 'D': return "**";
        default: return "*";
    }
}

void printStudents(const Student* students, size_t count) {
    printf("\nStudent Details:\n");

    for(size_t index=0; index<count; index++) {
        printf("\n");

        printf("Roll: %u\n", students[index].rollNumber);
        printf("Name: %s\n", students[index].name);
        printf("Total: %.2f\n", getTotalMarks(&students[index]));
    
        const float average = getAverageMarks(&students[index]);
        printf("Average: %.2f\n", average);
    
        const char grade = getGrade(average);
        printf("Grade: %c\n", grade);
    
        if(grade == 'F') continue;
        printf("Performance: %s\n", getPerformanceStars(grade));
    }
}

void printRollNumbers(const Student* students, size_t count) {
    if(count == 0) return;
    printf("%u ", students->rollNumber);
    printRollNumbers(students + 1, count - 1);
}


int main() {
    size_t studentCount;
    printf("Enter the number of students: ");
    if(
        scanf("%zu", &studentCount) != 1 || 
        studentCount < STUDENT_MIN_COUNT || 
        studentCount > STUDENT_MAX_COUNT
    ) {
        printf("Invalid input. Number of students must be a positive integer (%d-%d).\n", STUDENT_MIN_COUNT, STUDENT_MAX_COUNT);
        return 1;
    }
    cleanInputBuffer();

    Student* students = (Student*)malloc(studentCount * sizeof(Student));
    if(students == NULL) {
        printf("Error: Memory allocation failed\n");
        return 1;
    }

    printf("Enter %zu students details:\n", studentCount);
    for(size_t index = 0; index < studentCount; index++) {
        char buffer[BUFFER_MAX_SIZE];
        if(!fgets(buffer, sizeof(buffer), stdin)) {
            printf("Error reading input.\n");
            free(students);
            return 1;
        }

        if(
            sscanf(
                buffer, 
                "%u %49s %f %f %f", 
                &students[index].rollNumber, 
                students[index].name, 
                &students[index].marks[0], &students[index].marks[1], &students[index].marks[2]
            ) != 5
        ) {
            printf("Invalid input format for student %zu. Please enter: rollNumber name mark1 mark2 mark3\n", index + 1);
            free(students);
            return 1;
        }

        if(!isValidMarks(students[index].marks)) {
            printf("Invalid marks for student %zu. Marks must be between %.1f and %.1f.\n", index + 1, STUDENT_MIN_MARKS, STUDENT_MAX_MARKS);
            free(students);
            return 1;
        }
    }

    printStudents(students, studentCount);

    printf("\nList of Roll Numbers (via recursion): ");
    printRollNumbers(students, studentCount);
    printf("\n");

    free(students);
    return 0;
}