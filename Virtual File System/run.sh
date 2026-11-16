#!/bin/bash

gcc -Wall -Wextra \
    -Ilib \
    -Ilib/commands \
    -Ilib/filesystem \
    -Ilib/utils \
    -Ilib/utils/common \
    -Ilib/utils/inputHandlers \
    -Ilib/utils/stack \
    main.c \
    lib/commands/commands.c \
    lib/filesystem/filesystem.c \
    lib/utils/common/common.c \
    lib/utils/inputHandlers/inputHandlers.c \
    lib/utils/stack/stack.c \
    -o vfs && ./vfs
    
rm ./vfs
