#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define NAME_MAX_LEN 50
#define DESIGNATION_MAX_LEN 50


typedef struct Employee {
    unsigned int id;
    char* name;
    char* designation;

    struct Employee** subordinates;
    size_t subordinatesCount;
} Employee;


Employee* EMPLOYEE_TREE = NULL;


Employee* initializeNewEmployee() {
    Employee* employee = (Employee*)malloc(sizeof(Employee));
    if(employee == NULL) {
        printf("Error: Failed to allocate memory");
        exit(1);
    }

    employee->id = 0;
    employee->name = NULL;
    employee->designation = NULL;
    employee->subordinates = NULL;
    employee->subordinatesCount = 0;

    return employee;
}


void destroyEmployeeTree(Employee* employee) {
    if(employee == NULL) return;

    for(size_t i = 0; i < employee->subordinatesCount; i++) {
        destroyEmployeeTree(employee->subordinates[i]);
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



Employee* findManagerOfSearchFunction(Employee* employee, unsigned int targetEmployeeId) {
    if(employee == NULL) return NULL;
    for(size_t index = 0; index < employee->subordinatesCount; index++) {
        if(employee->subordinates[index]->id == targetEmployeeId) {
            return employee;
        }
    }
    return NULL;
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


void addEmployeeInEmployeeTree(Employee* employeeTree, Employee* employee, unsigned int managerId) {
    if(employeeTree == NULL) return;

    Employee* manager = searchInEmployeeTree(employeeTree, managerId, findEmployeeByIdSearchFunction);

    Employee** temp = realloc(
        manager->subordinates, 
        (manager->subordinatesCount + 1) * sizeof(Employee*)
    );

    if(temp == NULL) {
        printf("Error: failed to reallocate memory\n");
        exit(1);
    }

    manager->subordinates = temp;
    manager->subordinates[manager->subordinatesCount++] = employee;
}


void removeEmployeeFromEmployeeTree(Employee* employeeTree, unsigned int employeeId) {
    if(employeeTree == NULL) return;

    Employee* manager = searchInEmployeeTree(employeeTree, employeeId, findManagerOfSearchFunction);
    if(manager == NULL) {
        printf("Error: Cannot remove top hierarchy employee\n");
        return;
    }

    Employee* employeeToRemove = searchInEmployeeTree(employeeTree, employeeId, findEmployeeByIdSearchFunction);
    if(employeeToRemove == NULL) {
        printf("Error: Employee with id %u does not exist\n", employeeId);
        return;
    }

    for (size_t index = 0; index < manager->subordinatesCount; index++) {
        if (manager->subordinates[index]->id == employeeId) {
            manager->subordinates[index] = manager->subordinates[manager->subordinatesCount - 1];
            manager->subordinatesCount--;
            break;
        }
    }
    

    if(employeeToRemove->subordinatesCount == 0) {
        goto REMOVE_EMPLOYEE_CLEANUP;
    }

    Employee* minSubordinatesEmployee = NULL;
    for(size_t i = 0; i < manager->subordinatesCount; i++) {
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
        exit(1);
    }

    minSubordinatesEmployee->subordinates = temp;

    for(size_t i = 0; i < employeeToRemove->subordinatesCount; i++) {
        minSubordinatesEmployee->subordinates[minSubordinatesEmployee->subordinatesCount++] = employeeToRemove->subordinates[i];
    }

    REMOVE_EMPLOYEE_CLEANUP:
        if(manager->subordinatesCount == 0) {
            free(manager->subordinates);
            manager->subordinates = NULL;
        }

        free(employeeToRemove->name);
        free(employeeToRemove->designation);
        free(employeeToRemove->subordinates);
        free(employeeToRemove);
}


void clearInputBuffer() {
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}


char* readStringInput(const char* prompt, int maxBufferSize) {
    if(prompt != NULL) {
        printf("%s", prompt);
    }

    char* inputBuffer = (char*)malloc(maxBufferSize * sizeof(char));
    if(inputBuffer == NULL) {
        printf("Error: failed to allocate memory");
        exit(1);
    }

    if(fgets(inputBuffer, maxBufferSize, stdin) == NULL) {
        printf("Error: Unable to read input string\n");
        exit(1);
    }

    size_t inputLength = strlen(inputBuffer);

    if(inputLength > 0 && inputBuffer[inputLength-1] == '\n') {
        inputBuffer[inputLength-1] = '\0';
    } else {   
        clearInputBuffer();
    }

    return inputBuffer;
}


unsigned int readEmployeeId(const char* prompt) {
    if(prompt != NULL) {
        printf("%s", prompt);
    }

    int id;
    while((scanf("%d", &id) != 1) || id < 0) {
        printf("Warning, please enter a valid id: ");
        clearInputBuffer();
    }

    clearInputBuffer();
    return (unsigned int)id;
}


void readEmployeeDetails(
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
    *designation = readStringInput("DESIGNATION: ", DESIGNATION_MAX_LEN);

    if(managerId != NULL) {
        *managerId = readEmployeeId("MANAGER ID: ");
        while(searchInEmployeeTree(EMPLOYEE_TREE, *managerId, findEmployeeByIdSearchFunction) == NULL) {
            printf("Warning, enter a valid manager id, %u not exist: ", *managerId);
            *managerId = readEmployeeId(NULL);
        }
    }
}


void handleAddEmployee() {
    Employee* employee = initializeNewEmployee();

    bool isTopHierarchyFlow = EMPLOYEE_TREE == NULL;
    if(isTopHierarchyFlow) {
        printf("\n----- Adding Top Hierarchy Employee -----\n");
    } else {
        printf("\n----- Adding Employee -----\n");
    }

    unsigned int managerId;
    readEmployeeDetails(
        &employee->id,
        &employee->name,
        &employee->designation,
        isTopHierarchyFlow ? NULL : &managerId
    );

    if(isTopHierarchyFlow) {
        EMPLOYEE_TREE = employee;
    } else {
        addEmployeeInEmployeeTree(EMPLOYEE_TREE, employee, managerId);
    }
}


void handleRemoveEmployee() {
    printf("\n----- Removing Employee -----\n");
    unsigned int employeeId = readEmployeeId("Enter employee id to remove: ");
    if(searchInEmployeeTree(EMPLOYEE_TREE, employeeId, findEmployeeByIdSearchFunction) == NULL) {
        printf("Error: Employee with id %u does not exist\n", employeeId);
        return;
    }
    
    removeEmployeeFromEmployeeTree(EMPLOYEE_TREE, employeeId);
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
        destroyEmployeeTree(EMPLOYEE_TREE);
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
