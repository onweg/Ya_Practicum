#!/bin/bash

# Содержимое makefile
MAKEFILE_CONTENT="CC = g++
FLAGS = -std=c++14 -Wall -Wextra -Werror

all: static_analysis
	\$(CC) \$(FLAGS) main.cpp

static_analysis:
	cppcheck --enable=all --suppress=missingIncludeSystem *.cpp
	cppcheck -v *.cpp
"

# Находим все директории с именем src и добавляем makefile
find . -type d -name "src" | while read -r dir; do
    # Добавляем makefile в найденную директорию
    echo "$MAKEFILE_CONTENT" > "$dir/makefile"
done

echo "makefile добавлен во все директории с названием src"

