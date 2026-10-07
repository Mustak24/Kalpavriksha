#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Student {
    unsigned int rollNumber;
    char name[50];
    float marks[3];
} Student;


void cleanInputBuffer() {
    int ch;
    while((ch = getchar()) != '\n' && ch != EOF);
}


float getTotalMarks(const Student* student) {
    return student->marks[0] + student->marks[1] + student->marks[2];
}

float getAverageMarks(const Student* student) {
    return getTotalMarks(student) / 3.0f;
}

char getGrade(float average) {
    if(average >= 85.0f) return 'A';
    if(average >= 70.0f) return 'B';
    if(average >= 50.0f) return 'C';
    if(average >= 35.0f) return 'D';
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

void printStudentDetails(const Student* student) {
    printf("Roll: %u\n", student->rollNumber);
    printf("Name: %s\n", student->name);
    printf("Total: %.2f\n", getTotalMarks(student));

    const float average = getAverageMarks(student);
    printf("Average: %.2f\n", average);

    const char grade = getGrade(average);
    printf("Grade: %c\n", grade);

    if(grade != 'F') {   
        printf("Performance: %s\n", getPerformanceStarts(grade));
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
    if(scanf("%zu", &studentCount) != 1 || studentCount == 0) {
        printf("Invalid input. Number of students must be a positive integer.\n");
        return 1;
    }
    cleanInputBuffer();


    Student* students = (Student*)malloc(studentCount * sizeof(Student));
    if(students == NULL) {
        printf("Error: Memory allocation failed\n");
        return 1;
    }

    printf("Enter %zu students details:\n", studentCount);
    for(size_t i = 0; i < studentCount; i++) {
        char buffer[128];
        if(!fgets(buffer, sizeof(buffer), stdin)) {
            printf("Error reading input.\n");
            free(students);
            return 1;
        }

        if(
            sscanf(
                buffer, 
                "%u %49s %f %f %f", 
                &students[i].rollNumber, 
                students[i].name, 
                &students[i].marks[0], &students[i].marks[1], &students[i].marks[2]
            ) != 5
        ) {
            printf("Invalid input format for student %zu. Please enter: rollNumber name mark1 mark2 mark3\n", i + 1);
            free(students);
            return 1;
        }
    }

    printf("\nStudent Details:\n");
    for(size_t i=0; i<studentCount; i++) {
        printStudentDetails(&students[i]);
        printf("\n");
    }

    printf("List of Roll Numbers (via recursion): ");
    printRollNumbers(students, studentCount);
    printf("\n");

    free(students);
    return 0;
}