CC = g++
FLAGS = -std=c++17 -Wall -Wextra #-Werror

all: static_analysis
	$(CC) $(FLAGS) *.cpp

static_analysis:
	cppcheck --enable=all --suppress=missingIncludeSystem *.cpp
	cppcheck -v *.cpp

