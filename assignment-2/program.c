#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

#define USER_DATA_FILE "users.txt"


typedef struct {
    unsigned id;
    char name[50];
    unsigned short age;
} User;

User* users = NULL;
FILE* file = NULL;
size_t usersCount = 0;


void cleanInputBuffer() {
    int ch;
    while((ch = getchar()) != '\n' && ch != EOF);
}

void takeUserId(unsigned* id) {
    printf("Enter user ID: ");
    while(scanf("%u", id) != 1 || *id < 1) {
        printf("Invalid input. Please enter a valid user ID: ");
        cleanInputBuffer();
    }
    cleanInputBuffer();
}

bool isValidUserName(const char* name) {
    if(name == NULL || name[0] == '\0') return false;

    for(int i=0; name[i] != '\0'; i++) {
        if(name[i] == ',') return false;
    }

    return true;
}

void takeUserName(char* name) {
    printf("Enter user name: ");

    while(true) {
        if(fgets(name, 50, stdin) == NULL) {
            printf("Error: Failed to read user name.\n");
            exit(1);
        } 
    
        size_t len = strlen(name);
        if(len > 0 && name[len - 1] == '\n') {
            name[len - 1] = '\0';
        } else {
            cleanInputBuffer();
        }

        if(isValidUserName(name)) return;
        printf("Invalid input. Please enter a valid user name (no commas): ");
    }
}



void takeUserAge(unsigned short* age) {
    printf("Enter user age: ");
    while(scanf("%hu", age) != 1 || *age < 1 || *age > 120) {
        printf("Invalid input. Please enter a valid user age: ");
        cleanInputBuffer();
    }
    cleanInputBuffer();
}

void closeFile() {
    if(file != NULL) {
        fclose(file);
        file = NULL;
    }
}

bool isFileExists() {
    file = fopen(USER_DATA_FILE, "r");
    if(file == NULL) return false;

    closeFile();
    return true;
}


int openFile(const char* mode) {
    if(file != NULL) closeFile();

    if(isFileExists() == false) {
        file = fopen(USER_DATA_FILE, "w");
        if(file == NULL) {
            printf("Error: Unable to create file %s.\n", USER_DATA_FILE);
            return 1;
        }

        closeFile();
    }

    file = fopen(USER_DATA_FILE, mode);
    if(file == NULL) {
        printf("Error: Unable to open file %s.\n", USER_DATA_FILE);
        return 1;
    }

    return 0;
}

int getFileLineCount() {
    if(file == NULL) return 0;

    int ch, count = 0;
    fseek(file, 0, SEEK_SET);
    
    do {
        ch = fgetc(file);
        if(ch == '\n') count++;
    } while(ch != EOF);

    fseek(file, 0, SEEK_SET);
    return count;
}


int loadUsers() {
    int error = openFile("r");
    if(error != 0) return 1;
    
    usersCount = getFileLineCount();
    
    if(usersCount == 0) {
        closeFile();
        return 0;
    }
    
    users = (User*)malloc(usersCount * sizeof(User));
    if(users == NULL) {
        closeFile();
        return 1;
    }

    for(int i=0; i<(int)usersCount; i++) {
        if(fscanf(
            file,
            "%u, %49[^,], %hu\n",
            &users[i].id,
            users[i].name,
            &users[i].age
        ) != 3) {
            printf("Error: Unable to read user data from file.\n");
            closeFile();
            free(users);
            users = NULL;
            return 1;
        }
    }

    closeFile();
    return 0;
}


void displayUsers() {
    printf("User List:\n");
    printf("%-12s %-50s %-10s\n", "ID", "Name", "Age");

    for(int i=0; i<72; i++) printf("-");
    printf("\n");
    
    if(usersCount == 0) {
        printf("No users found.\n");
        return;
    }
    
    for(int i=0; i<usersCount; i++) {
        printf("%-12u %-50s %-10hu\n", users[i].id, users[i].name, users[i].age);
    }
    
    for(int i=0; i<72; i++) printf("-");
    printf("\n");
}


unsigned generateUserId() {
    unsigned maxId = 0;

    for(int i=0; i<(int)usersCount; i++) {
        if(users[i].id >= maxId) {
            maxId = users[i].id;
        }
    }

    if(maxId == UINT_MAX) {
        return 0;
    }

    return maxId + 1;
}


void createUser() {
    unsigned id = generateUserId();
    if(id == 0) {
        printf("Error: Maximum user ID reached. Cannot create new user.\n");
        return;
    }
    
    User* temp = (User*)realloc(users, (usersCount + 1) * sizeof(User));
    if(temp == NULL) {
        printf("Error: Memory allocation failed for new user.\n");
        return;
    }

    users = temp;

    users[usersCount].id = id;
    takeUserName(users[usersCount].name);
    takeUserAge(&users[usersCount].age);

    usersCount++;
}


void updateUser() {
    unsigned id;
    takeUserId(&id);

    bool userFound = false;
    for(int i=0; i<usersCount; i++) {
        if(users[i].id == id) {
            userFound = true;
            printf("Updating user with ID %u:\n", id);
            takeUserName(users[i].name);
            takeUserAge(&users[i].age);
            break;
        }
    }

    if(!userFound) {
        printf("User with ID %u not found.\n", id);
        return;
    }

    printf("User with ID %u updated successfully.\n", id);
    return;
}


void deleteUser() {
    unsigned id;
    takeUserId(&id);

    if(usersCount == 0) {
        printf("No users found.\n");
        return;
    }

    bool userFound = false;
    for(int i=0; i<usersCount; i++) {
        if(users[i].id == id) {
            userFound = true;
            
            for(int j=i; j<usersCount-1; j++) {
                users[j] = users[j+1];
            }
            
            usersCount--;
            break;
        }
    }

    if(userFound) {
        printf("User with ID %u deleted successfully.\n", id);
    } else {
        printf("User with ID %u not found.\n", id);
    }
}


void handleSave() {
    int error = openFile("w");
    if(error != 0) {
        printf("Error: Unable to open file for writing.\n");
        return;
    }

    for(int i=0; i<usersCount; i++) {
        if(fprintf(file, "%u, %s, %hu\n", users[i].id, users[i].name, users[i].age) < 0) {
            printf("Error: Unable to write user data to file.\n");
            closeFile();
            return;
        }
    }

    closeFile();
    printf("Users saved successfully.\n");
}


void handleExit() {
    printf("Exiting the program.\n");
    handleSave();
    if(users) {
        free(users);
        users = NULL;
    }
}


int main() {
    int error = loadUsers();
    if(error != 0) {
        printf("Error: Unable to load users.\n");
        return 1;
    }

    int choice;

    do {
        printf("\nUser Management System\n");
        printf("1. Display Users\n");
        printf("2. Create User\n");
        printf("3. Update User\n");
        printf("4. Delete User\n");
        printf("5. Save & Exit\n");
        printf("Enter your choice: ");
        
        while(scanf("%d", &choice) != 1 || choice < 1 || choice > 5) {
            printf("Invalid input. Please enter a valid choice: ");
            cleanInputBuffer();
        }
        cleanInputBuffer();
        
        printf("\n");

        switch(choice) {
            case 1:
                displayUsers();
                break;
            case 2:
                createUser();
                break;
            case 3:
                updateUser();
                break;
            case 4:
                deleteUser();
                break;
            case 5:
                handleExit();
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while(choice != 5);

    return 0;
}