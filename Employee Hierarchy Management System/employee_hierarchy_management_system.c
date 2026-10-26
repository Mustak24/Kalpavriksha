#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define NAME_MAX_LEN 50
#define DESIGNATION_MAX_LEN 50


typedef struct Employee {
    unsigned int id;
    char* name;
    char* designation;

    struct Employee* manager;
    struct Employee** subordinates;
    size_t subordinatesCount;
} Employee;


Employee* EMPLOYEE_TREE = NULL;


Employee* initializeNewEmployee() {
    Employee* employee = (Employee*)malloc(sizeof(Employee));
    if(employee == NULL) {
        printf("Error: Failed to allocate memory");
        return NULL;
    }

    employee->id = 0;
    employee->name = NULL;
    employee->designation = NULL;
    employee->manager = NULL;
    employee->subordinates = NULL;
    employee->subordinatesCount = 0;

    return employee;
}



void destroyEmployeeTree(Employee* employee, bool deepDestroy) {
    if(employee == NULL) return;

    if(deepDestroy) {
        for(size_t i = 0; i < employee->subordinatesCount; i++) {
            destroyEmployeeTree(employee->subordinates[i], deepDestroy);
        }
    }

    free(employee->name);
    free(employee->designation);
    free(employee->subordinates);
    free(employee);
}



int displayEmployeeTree(Employee* employee, int level) {
    if(employee == NULL) return 0;

    for(int tab=0; tab<level; tab++) {
        printf("  ");
    }
    printf("%u - %s - %s\n", employee->id, employee->name, employee->designation);

    int count = 1;
    for(size_t i = 0; i < employee->subordinatesCount; i++) {
        count += displayEmployeeTree(employee->subordinates[i], level + 1);
    }

    return count;
}


Employee* findEmployeeByIdSearchFunction(Employee* employee, unsigned int targetEmployeeId) {
    if(employee != NULL && employee->id == targetEmployeeId) {
        return employee;
    }
    return NULL;
}

Employee* searchInEmployeeTree(
    Employee* currentEmployee, 
    unsigned int targetEmployeeId,
    Employee* (*searchFunction)(Employee* employee, unsigned int targetEmployeeId)
) {  
    if(currentEmployee == NULL) return NULL;

    Employee* result = searchFunction(currentEmployee, targetEmployeeId);
    if(result != NULL) return result;

    for(size_t index = 0; index < currentEmployee->subordinatesCount; index++) {
        Employee* subordinate = currentEmployee->subordinates[index];

        result = searchInEmployeeTree(subordinate, targetEmployeeId, searchFunction);
        if(result != NULL) return result;
    }

    return NULL;
}


int addEmployeeInEmployeeTree(Employee* employeeTree, Employee* employee, unsigned int managerId) {
    if(employeeTree == NULL) return 1;

    Employee* manager = searchInEmployeeTree(employeeTree, managerId, findEmployeeByIdSearchFunction);
    if(manager == NULL) {
        printf("Error: Manager with id %u does not exist\n", managerId);
        return 1;
    }

    Employee** temp = realloc(
        manager->subordinates, 
        (manager->subordinatesCount + 1) * sizeof(Employee*)
    );

    if(temp == NULL) {
        printf("Error: failed to reallocate memory\n");
        return 1;
    }

    manager->subordinates = temp;
    employee->manager = manager;
    manager->subordinates[manager->subordinatesCount++] = employee;
    return 0;
}


int removeEmployeeFromEmployeeTree(Employee* employeeTree, unsigned int employeeId) {
    if(employeeTree == NULL) return 1;

    
    Employee* employeeToRemove = searchInEmployeeTree(employeeTree, employeeId, findEmployeeByIdSearchFunction);
    if(employeeToRemove == NULL) {
        printf("Error: Employee with id %u does not exist\n", employeeId);
        return 1;
    }
    
    Employee* manager = employeeToRemove->manager;
    if(manager == NULL) {
        printf("Error: Cannot remove top hierarchy employee\n");
        return 1;
    }

    if(employeeToRemove->subordinatesCount > 0) {
        Employee* minSubordinatesEmployee = NULL;
        for(size_t i = 0; i < manager->subordinatesCount; i++) {
            if(manager->subordinates[i]->id == employeeId) continue;
            
            Employee* subordinate = manager->subordinates[i];
            if(minSubordinatesEmployee == NULL || subordinate->subordinatesCount < minSubordinatesEmployee->subordinatesCount) {
                minSubordinatesEmployee = subordinate;
            }
        }

        if(minSubordinatesEmployee == NULL) {
            minSubordinatesEmployee = manager;
        }

        Employee** temp = realloc(
            minSubordinatesEmployee->subordinates, 
            (minSubordinatesEmployee->subordinatesCount + employeeToRemove->subordinatesCount) * sizeof(Employee*)
        );

        if(temp == NULL) {
            printf("Error: failed to reallocate memory\n");
            return 1;
        }

        minSubordinatesEmployee->subordinates = temp;

        for(size_t i = 0; i < employeeToRemove->subordinatesCount; i++) {
            employeeToRemove->subordinates[i]->manager = minSubordinatesEmployee;
            minSubordinatesEmployee->subordinates[minSubordinatesEmployee->subordinatesCount++] = employeeToRemove->subordinates[i];
        }
    }

    for (size_t index = 0; index < manager->subordinatesCount; index++) {
        if (manager->subordinates[index]->id == employeeId) {
            for (size_t j = index + 1; j < manager->subordinatesCount; j++) {
                manager->subordinates[j - 1] = manager->subordinates[j];
            }

            manager->subordinatesCount--;
            break;
        }
    }

    if(manager->subordinatesCount == 0) {
        free(manager->subordinates);
        manager->subordinates = NULL;
    }

    destroyEmployeeTree(employeeToRemove, false);
    return 0;
}


void clearInputBuffer() {
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}

char* trimString(char* str) {
    if(str == NULL) return NULL;

    char* start = str;
    while(isspace(*start)) start++;

    if(*start == '\0') {
        *str = '\0';
        return str;
    }

    char* end = start + strlen(start) - 1;
    while(end > start && isspace(*end)) end--;

    *(end + 1) = '\0';

    memmove(str, start, end - start + 2);
    return str;
}

char* readStringInput(const char* prompt, int maxBufferSize) {
    if(prompt != NULL) {
        printf("%s", prompt);
    }

    char* inputBuffer = (char*)malloc(maxBufferSize * sizeof(char));
    if(inputBuffer == NULL) {
        printf("Error: failed to allocate memory");
        return NULL;
    }

    if(fgets(inputBuffer, maxBufferSize, stdin) == NULL) {
        printf("Error: Unable to read input string\n");
        free(inputBuffer);
        return NULL;
    }

    if(inputBuffer[strlen(inputBuffer) - 1] != '\n') {
        clearInputBuffer();
    }

    trimString(inputBuffer);

    if(strlen(inputBuffer) == 0) {
        printf("Warning, please enter a valid string: ");
        free(inputBuffer);
        return readStringInput(NULL, maxBufferSize);
    }

    return inputBuffer;
}


unsigned int readEmployeeId(const char* prompt) {
    if(prompt != NULL) {
        printf("%s", prompt);
    }

    long int id;
    while((scanf("%ld", &id) != 1) || id < 0 || id > UINT_MAX) {
        printf("Warning, please enter a valid id: ");
        clearInputBuffer();
    }

    clearInputBuffer();
    return (unsigned int)id;
}


int readEmployeeDetails(
    unsigned int* id,
    char** name,
    char** designation,
    unsigned int* managerId
) {
    *id = readEmployeeId("ID: ");
    while(searchInEmployeeTree(EMPLOYEE_TREE, *id, findEmployeeByIdSearchFunction) != NULL) {
        printf("Warning, entre a valid id, %u is already exist: ", *id);
        *id = readEmployeeId(NULL);
    }

    *name = readStringInput("NAME: ", NAME_MAX_LEN);
    if(*name == NULL) {
        printf("Error: Unable to read employee name\n");
        return 1;
    }

    *designation = readStringInput("DESIGNATION: ", DESIGNATION_MAX_LEN);
    if(*designation == NULL) {
        printf("Error: Unable to read employee designation\n");
        return 1;
    }

    if(managerId != NULL) {
        *managerId = readEmployeeId("MANAGER ID: ");
        while(searchInEmployeeTree(EMPLOYEE_TREE, *managerId, findEmployeeByIdSearchFunction) == NULL) {
            printf("Warning, enter a valid manager id, %u not exist: ", *managerId);
            *managerId = readEmployeeId(NULL);
        }
    }

    return 0;
}


void handleAddEmployee() {
    Employee* employee = initializeNewEmployee();
    if(employee == NULL) {
        printf("Error: Failed to initialize new employee\n");
        return;
    }

    bool isTopHierarchyFlow = EMPLOYEE_TREE == NULL;
    if(isTopHierarchyFlow) {
        printf("\n----- Adding Top Hierarchy Employee -----\n");
    } else {
        printf("\n----- Adding Employee -----\n");
    }

    unsigned int managerId;
    int error = readEmployeeDetails(
        &employee->id,
        &employee->name,
        &employee->designation,
        isTopHierarchyFlow ? NULL : &managerId
    );

    if(error != 0) {
        printf("Error: Unable to read employee details\n");
        destroyEmployeeTree(employee, false);
        return;
    }

    if(isTopHierarchyFlow) {
        EMPLOYEE_TREE = employee;
    } else {
        int error = addEmployeeInEmployeeTree(EMPLOYEE_TREE, employee, managerId);
        if(error != 0) {
            printf("Error: Failed to add employee with id %u\n", employee->id);
            destroyEmployeeTree(employee, false);
            return;
        }
    }

    printf("Employee with id %u added successfully\n", employee->id);
}


void handleRemoveEmployee() {
    printf("\n----- Removing Employee -----\n");
    unsigned int employeeId = readEmployeeId("Enter employee id to remove: ");
    if(searchInEmployeeTree(EMPLOYEE_TREE, employeeId, findEmployeeByIdSearchFunction) == NULL) {
        printf("Error: Employee with id %u does not exist\n", employeeId);
        return;
    }
    
    int error = removeEmployeeFromEmployeeTree(EMPLOYEE_TREE, employeeId);
    if(error != 0) {
        printf("Error: Failed to remove employee with id %u\n", employeeId);
        return;
    }

    printf("Employee with id %u removed successfully\n", employeeId);
}


void handleDisplayEmployeeHierarchy() {
    printf("\n----- Displaying Employee Hierarchy -----\n");
    if(EMPLOYEE_TREE == NULL) {
        printf("Error: No employees in the system\n");
        return;
    }

    unsigned int employeeId = readEmployeeId("Enter employee id: ");

    Employee* employee = searchInEmployeeTree(EMPLOYEE_TREE, employeeId, findEmployeeByIdSearchFunction);
    if(employee == NULL) {
        printf("Error: Employee with id %u does not exist\n", employeeId);
        return;
    }

    int employeeCount = displayEmployeeTree(employee, 0);
    printf("\nTotal Employees under %u: %d\n", employee->id, employeeCount-1);
}


void handleDisplayCompleteHierarchy() {
    printf("\n----- Displaying Complete Hierarchy -----\n");
    if(EMPLOYEE_TREE == NULL) {
        printf("Error: No employees in the system\n");
        return;
    }

    int employeeCount = displayEmployeeTree(EMPLOYEE_TREE, 0);
    printf("\nTotal Employees: %d\n", employeeCount);
}

void handleExit() {
    printf("\n-----EXIT-----\n");

    if(EMPLOYEE_TREE != NULL) {
        destroyEmployeeTree(EMPLOYEE_TREE, true);
        EMPLOYEE_TREE = NULL;
    }
}


int main() {
    int choice;
    
    do {
        printf("\n-----EMPLOYEE HIERARCHY MANAGEMENT SYSTEM-----\n");
        printf("1. Add Employee\n");
        printf("2. Remove Employee\n");
        printf("3. Display Employee Hierarchy\n");
        printf("4. Display Complete Hierarchy\n");
        printf("5. Exit\n");
        
        printf("Select: ");
        while(scanf("%d", &choice) != 1) {
            printf("Warning, please select a valid choice: ");
            clearInputBuffer();
        }

        clearInputBuffer();
        
        switch(choice) {
            case 1:
                handleAddEmployee();
                break;
            case 2:
                handleRemoveEmployee();
                break;
            case 3:
                handleDisplayEmployeeHierarchy();
                break;
            case 4:
                handleDisplayCompleteHierarchy();
                break;
            case 5:
                handleExit();
                break;
            default:
                printf("\n!!!Invalid Option!!!\n");
        }

    } while(choice != 5);

    return 0;
}
