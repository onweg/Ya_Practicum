#include "read_input_functions.h"

using namespace std;

std::string ReadLine() {
    std::string s;
    getline(cin, s);
    return s;
}

int ReadLineWithNumber() {
    int result;
    std::cin >> result;
    ReadLine();
    return result;
}