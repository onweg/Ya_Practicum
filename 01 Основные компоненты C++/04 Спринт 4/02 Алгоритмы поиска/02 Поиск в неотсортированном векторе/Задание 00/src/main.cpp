#include <iostream>
#include <algorithm>

using namespace std;

void PrintSpacesPositions(const string& str) {
    // напишите реализацию
    char space = ' ';
    auto it = find(str.begin(), str.end(), space);
    while (it != str.end()) {
        cout << distance(str.begin(), it) << endl;
        it = find(next(it), str.end(), space);
    }
}

int main() {
    string str = "He said: one and one and one is three"s;
    PrintSpacesPositions(str);
    return 0;
}