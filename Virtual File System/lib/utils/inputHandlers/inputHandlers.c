#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "filesystem.h"
#include "commands.h"
#include "common.h"



static void handleFsCreateDirectory(FileSystemContext* fsContext, const char* argString) {
    displayFunctionStatus(
        fsCreateDirectory(fsContext, argString)
    );
}


static void handleFsCreateFile(FileSystemContext* fsContext, const char* argString) {
    displayFunctionStatus(
        fsCreateFile(fsContext, argString)
    );

}


static void handleFsWriteFile(FileSystemContext* fsContext, const char* argString) {
    size_t argStringLength = strlen(argString);

    char fileName[FILE_NAME_MAX_LEN + 1];
    char* content = (char*)malloc(argStringLength * sizeof(char));
    if(content == NULL) {
        printf("ERROR: Failed to read input data.\n");
        return;
    }

    sscanf(argString, "%s", fileName);
    strncpy(content, argString + strlen(fileName), argStringLength);

    trimString(fileName);
    trimString(content);

    size_t contentLength = strlen(content);

    if(
        content[0] != '"' || 
        content[0] != content[contentLength-1]
    ) {
        printf("Invalid data format data must be in \"<data>\" format\n");
        free(content);
        return;
    }

    content[contentLength-1] = '\0';
    for(size_t index = 1; index < contentLength; index++) {
        content[index - 1] = content[index];
    }

    displayFunctionStatus(
        fsWriteFile(fsContext, fileName, content)
    );

    free(content);
}


static void handleFsReadFile(FileSystemContext* fsContext, const char* argString) {
    char* fileData;
    FunctionStatus status = fsReadFile(fsContext, argString, &fileData);
    if(status.success == false) {
        puts(status.message);
        return;
    }


    puts(fileData);
    free(fileData);
}


static void handleFsDeleteFile(FileSystemContext* fsContext, const char* argString) {
    displayFunctionStatus(
        fsDeleteFile(fsContext, argString)
    );
}


static void handleFsDeleteDirectory(FileSystemContext* fsContext, const char* argString) {
    displayFunctionStatus(
        fsDeleteDirectory(fsContext, argString)
    );
}


static void handleFsDisplayFileAndFolder(FileSystemContext* fsContext) {
    FunctionStatus status = dsDisplayFileAndFolder(fsContext);
    if(status.success == false) {
        puts(status.message);
    }
}


static void handleFsChangeCurrentDirectory(FileSystemContext* fsContext, const char* argString) {
    FunctionStatus status = fsChangeCurrentDirectory(fsContext, argString);
    if(status.success == false) {
        puts(status.message);
        return;
    }

    printf("Move to %s\n", argString);
}


static void handleFsDisplayPresentWorkingDirectory(FileSystemContext* fsContext) {
    FunctionStatus status = fsDisplayPresentWorkingDirectory(fsContext);
    if(status.success == false) {
        puts(status.message);
        return;
    }
}


static void handleFsDisplayDiskUsage(FileSystemContext* fsContext) {
    FunctionStatus status = fsDisplayDiskUsage(fsContext);
    if(status.success == false) {
        puts(status.message);
    }
}


static void handleFsExit(FileSystemContext** fsContext) {
    FunctionStatus status = fsExit(fsContext);
    if(status.success == false) {
        puts(status.message);
    }
}



void handleTerminalInput(FileSystemContext** fsContext) {
    char* command = readLine();
    if(command == NULL) {
        handleFsExit(fsContext);
        return;    
    }

    size_t commandLength = strlen(command);
    
    char commandName[COMMAND_NAME_MAX_LEN + 1];
    char* argString = (char*)malloc(commandLength * sizeof(char));
    
    if(
        sscanf(
            command, 
            "%" STRINGIFY(COMMAND_NAME_MAX_LEN) "s", 
            commandName
        ) != 1
    ) {
        free(command);
        printf("ERROR: command not found!!\n");
        return;
    }

    strncpy(argString, command + strlen(commandName), commandLength);
    
    trimString(commandName);
    trimString(argString);

    COMMAND_CODE commandCode = getCommandCode(commandName);

    switch (commandCode) {
        case COMMAND_cd:
            handleFsChangeCurrentDirectory(*fsContext, argString);
            break;

        case COMMAND_rmdir:
            handleFsDeleteDirectory(*fsContext, argString);
            break;

        case COMMAND_mkdir:
            handleFsCreateDirectory(*fsContext, argString);
            break;

        case COMMAND_create:
            handleFsCreateFile(*fsContext, argString);
            break;

        case COMMAND_delete:
            handleFsDeleteFile(*fsContext, argString);
            break;

        case COMMAND_read:
            handleFsReadFile(*fsContext, argString);
            break;

        case COMMAND_write:
            handleFsWriteFile(*fsContext, argString);
            break;

        case COMMAND_pwd:
            handleFsDisplayPresentWorkingDirectory(*fsContext);
            break;

        case COMMAND_ls:
            handleFsDisplayFileAndFolder(*fsContext);
            break;

        case COMMAND_df:
            handleFsDisplayDiskUsage(*fsContext);
            break;

        case COMMAND_exit:
            handleFsExit(fsContext);
            break;

        case COMMAND_INVALID:
        default:
            printf("ERROR: Invalid command!!\n");
    }   

    free(argString);
    free(command);
}