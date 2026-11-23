#!/bin/bash

gcc -Wall -Wextra \
    -Ilib \
    -Ilib/analyzer \
    -Ilib/commands \
    -Ilib/data \
    -Ilib/utils \
    -Ilib/utils/common \
    -Ilib/utils/heap \
    -Ilib/utils/sortedList \
    main.c \
    lib/analyzer/analyzer.c \
    lib/commands/commands.c \
    lib/utils/common/common.c \
    lib/utils/heap/heap.c \
    lib/utils/sortedList/sortedList.c \
    -o iccAnalyzer && ./iccAnalyzer "$@"

rm -f ./iccAnalyzer
