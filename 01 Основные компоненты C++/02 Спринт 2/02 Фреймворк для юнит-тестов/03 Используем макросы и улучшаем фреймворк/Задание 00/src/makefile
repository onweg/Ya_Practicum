CC = g++
FLAGS = -std=c++14 -Wall -Wextra -Werror
NAME = main.cpp

all: static_analysis
	$(CC) $(FLAGS) $(NAME)

static_analysis:
	cppcheck --enable=all --suppress=missingIncludeSystem $(NAME)
	cppcheck -v $(NAME)

