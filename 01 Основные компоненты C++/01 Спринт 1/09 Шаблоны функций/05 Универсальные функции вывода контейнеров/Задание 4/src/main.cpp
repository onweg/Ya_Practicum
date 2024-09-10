#include <iostream>
#include <vector>
#include <set>

using namespace std;

template<typename Container>
void Print(ostream& out, const Container& container) {
    int i = 0, size = static_cast<int>(container.size());
    for (const auto& element : container) {
        out << element;
        if (i + 1 < size)
            out << ", "s;
    }
}

template<typename T>
ostream& operator<<(ostream& out, const set<T>& container) {
    Print(out, container);
    return out;
}

template<typename T>
ostream& operator<<(ostream& out, const vector<T>& container) {
    Print(out, container);
    return out;
}

int main() {
    const vector<int> ages = {10, 5, 2, 12};
    cout << ages << endl;
    const set<string> cats = {"Мурка"s, "Белка"s, "Георгий"s, "Рюрик"s};
    cout << cats << endl;
}