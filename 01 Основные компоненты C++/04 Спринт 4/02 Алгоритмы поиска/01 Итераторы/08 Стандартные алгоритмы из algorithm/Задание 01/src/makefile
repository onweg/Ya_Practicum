CC = g++
FLAGS = -std=c++14 -Wall -Wextra -Werror
NAME = main

all: static_analysis
	$(CC) $(FLAGS) $(NAME).cpp

static_analysis:
	cppcheck --enable=all --suppress=missingIncludeSystem $(NAME).cpp
	cppcheck -v $(NAME).cpp

