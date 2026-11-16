#ifndef COMMANDS
#define COMMANDS


#include "filesystem.h"
#include "common.h"

#define COMMAND_NAME_MAX_LEN 50

typedef enum COMMAND_CODE {
    COMMAND_mkdir = 0,
    COMMAND_create,
    COMMAND_write,
    COMMAND_read,
    COMMAND_delete,
    COMMAND_rmdir,
    COMMAND_ls,
    COMMAND_cd,
    COMMAND_pwd,
    COMMAND_df,
    COMMAND_exit,

    COMMAND_INVALID = -1
} COMMAND_CODE;

COMMAND_CODE getCommandCode(const char* string);

FunctionStatus fsCreateDirectory(FileSystemContext* fsContext, const char* directoryName);
FunctionStatus fsCreateFile(FileSystemContext* fsContext, const char* fileName);
FunctionStatus fsWriteFile(FileSystemContext* fsContext, const char* fileName, const char* data);
FunctionStatus fsReadFile(FileSystemContext* fsContext, const char* fileName, char** resultFileData);
FunctionStatus fsDeleteFile(FileSystemContext* fsContext, const char* fileName);
FunctionStatus fsDeleteDirectory(FileSystemContext* fsContext, const char* directoryName);
FunctionStatus dsDisplayFileAndFolder(FileSystemContext* fsContext);
FunctionStatus fsChangeCurrentDirectory(FileSystemContext* fsContext, const char* directoryName);
FunctionStatus fsDisplayPresentWorkingDirectory(FileSystemContext* fsContext);
FunctionStatus fsDisplayDiskUsage(FileSystemContext* fsContext);
FunctionStatus fsExit(FileSystemContext** fsContext);


#endif