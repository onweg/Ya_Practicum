#include <iostream>
#include <vector>
#include <set>

using namespace std;

template <typename It>
void PrintRange(It range_begin, It range_end) {
    for (auto it = range_begin; it != range_end; ++it) {
        cout << *it << " "s;
    }
    cout << endl;
}

template <typename Container>
void EraseAndPrint(Container& container, int n) {
    PrintRange(container.begin(), container.begin() + n);
    PrintRange(container.begin() + 1 + n, container.end());
    container.erase(container.begin() + n);
}

template <typename Container>
void EraseAndPrint(Container& container, int n, int lhs, int rhs) {
    container.erase(container.begin() + n);
    PrintRange(container.begin(), container.end());

    container.erase(container.begin() + lhs, container.begin() + rhs);
    PrintRange(container.begin(), container.end());
}

int main() {
    vector<string> langs = {"Python"s, "Java"s, "C#"s, "Ruby"s, "C++"s};
    EraseAndPrint(langs, 0, 0, 2);
}