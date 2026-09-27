#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define USER_DATA_FILE "users.txt"


typedef struct {
    unsigned id;
    char name[50];
    int age;
} User;


void cleanInputBuffer() {
    int ch;
    while((ch = getchar()) != '\n' && ch != EOF);
}

void takeUserId(unsigned* id) {
    printf("Enter user ID: ");
    while(scanf("%u", id) != 1) {
        printf("Invalid input. Please enter a valid user ID: ");
        cleanInputBuffer();
    }
    cleanInputBuffer();
}


void takeUserName(char* name) {
    printf("Enter user name: ");
    while(scanf("%49[^\n]", name) != 1) {
        printf("Invalid input. Please enter a valid user name: ");
        cleanInputBuffer();
    }
    cleanInputBuffer();
}


void takeUserAge(int* age) {
    printf("Enter user age: ");
    while(scanf("%d", age) != 1) {
        printf("Invalid input. Please enter a valid user age: ");
        cleanInputBuffer();
    }
    cleanInputBuffer();
}


bool isFileExists() {
    FILE* file = fopen(USER_DATA_FILE, "r");
    if(file == NULL) return false;

    fclose(file);
    return true;
}


int openFile(char* mode, FILE** file) {
    if(isFileExists() == false) {
        *file = fopen(USER_DATA_FILE, "w");
        if(*file == NULL) {
            printf("Error: Unable to create file %s\n", USER_DATA_FILE);
            return 1;
        }

        fclose(*file);
    }

    *file = fopen(USER_DATA_FILE, mode);
    if(*file == NULL) {
        printf("Error: Unable to open file %s with mode %s \n", USER_DATA_FILE, mode);
        return 1;
    }

    return 0;
}


int getFileLineCount(FILE** file) {
    if(*file == NULL) return 0;

    int ch, count = 0;

    do {
        ch = fgetc(*file);
        if(ch == '\n') count++;
    } while(ch != EOF);

    return count;
}


int loadUsers(User** users, int* userCount) {
    FILE* file;
    int error = openFile("r", &file);
    if(error != 0) {
        printf("Error: Unable to open file %s for reading\n", USER_DATA_FILE);
        return 1;
    }
    
    int size = getFileLineCount(&file);
    fseek(file, 0, SEEK_SET);
    
    *userCount = size;
    
    if(size == 0) {
        *users = NULL;
        fclose(file);
        return 0;
    }

    *users = (User*)malloc(size * sizeof(User));

    for(int i=0; i<size; i++) {
        fscanf(
            file,
            "%u, %49[^,], %d\n",
            &(*users)[i].id,
            (*users)[i].name,
            &(*users)[i].age
        );
    }

    fclose(file);
    return 0;
}


void displayUsers() {
    User* users;
    int userCount;
    int error = loadUsers(&users, &userCount);
    if(error != 0) return;

    printf("User List:\n");
    printf("%-12s %-50s %-10s\n", "ID", "Name", "Age");

    for(int i=0; i<72; i++) printf("-");
    printf("\n");
    
    if(userCount == 0) {
        printf("No users found.\n");
        free(users);
        return;
    }
    
    for(int i=0; i<userCount; i++) {
        printf("%-12u %-50s %-10d\n", users[i].id, users[i].name, users[i].age);
    }
    
    for(int i=0; i<72; i++) printf("-");
    printf("\n");

    free(users);
}


unsigned generateUserId() {
    User* users;
    int userCount;
    
    int error = loadUsers(&users, &userCount);
    if(error != 0) return 1;
    
    unsigned id = 1 + (userCount > 0 ? users[userCount - 1].id : 0);

    free(users);
    return id;
}


void createUser() {
    User user;

    user.id = generateUserId();
    takeUserName(user.name);
    takeUserAge(&user.age);

    FILE* file;
    int error = openFile("a", &file);
    if(error != 0) return;

    fprintf(file, "%u, %s, %d\n", user.id, user.name, user.age);
    printf("User with ID %u created successfully.\n", user.id);

    fclose(file);
}


void updateUser() {
    unsigned id;
    takeUserId(&id);

    User* users;
    int userCount;
    int error = loadUsers(&users, &userCount);
    if(error != 0) return;

    bool userFound = false;
    for(int i=0; i<userCount; i++) {
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
        free(users);
        return;
    }

    FILE* file;
    error = openFile("w", &file);
    if(error != 0) return;

    for(int i=0; i<userCount; i++) {
        fprintf(file, "%u, %s, %d\n", users[i].id, users[i].name, users[i].age);
    }

    printf("User with ID %u updated successfully.\n", id);

    fclose(file);
    free(users);
}


void deleteUser() {
    unsigned id;
    takeUserId(&id);

    User* users;
    int userCount;
    int error = loadUsers(&users, &userCount);
    if(error != 0) return;

    if(userCount == 0) {
        printf("No users found.\n");
        free(users);
        return;
    }

    FILE* file;
    error = openFile("w", &file);
    if(error != 0) return;

    bool userFound = false;
    for(int i=0; i<userCount; i++) {
        if(users[i].id == id) {
            userFound = true;
            continue;
        }

        fprintf(file, "%u, %s, %d\n", users[i].id, users[i].name, users[i].age);
    }

    if(userFound) {
        printf("User with ID %u deleted successfully.\n", id);
    } else {
        printf("User with ID %u not found.\n", id);
    }

    fclose(file);
    free(users);
}


int main() {
    int choice;

    do {
        printf("\nUser Management System\n");
        printf("1. Display Users\n");
        printf("2. Create User\n");
        printf("3. Update User\n");
        printf("4. Delete User\n");
        printf("5. Exit\n");
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
                printf("Exiting the program.\n");
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while(choice != 5);

    return 0;
}