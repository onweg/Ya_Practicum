CC = g++
FLAGS = -std=c++14 -Wall -Wextra -Werror

all: static_analysis
	$(CC) $(FLAGS) main.cpp

static_analysis:
	cppcheck --enable=all --suppress=missingIncludeSystem *.cpp
	cppcheck -v *.cpp

