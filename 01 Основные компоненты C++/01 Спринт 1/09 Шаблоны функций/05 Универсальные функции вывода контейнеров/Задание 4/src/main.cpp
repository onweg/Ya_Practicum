#include <iostream>
#include <vector>
#include <set>

using namespace std;

template<typename T>
ostream& operator<<(ostream& out, const set<T>& container) {
    int i = 0, size = static_cast<int>(container.size());
    for (const auto& element : container) {
        out << element;
        if (i + 1 < size) {
            out << ", ";
        }
        ++i;
    }
    return out;
}

template<typename T>
ostream& operator<<(ostream& out, const vector<T>& container) {
    int i = 0, size =  static_cast<int>(container.size());
    for (const auto& element : container) {
        out << element;
        if (i + 1 < size) {
            out << ", "s;
        }
        ++i;
    }
    return out;
}

int main() {
    const vector<int> ages = {10, 5, 2, 12};
    cout << ages << endl;
    const set<string> cats = {"Мурка"s, "Белка"s, "Георгий"s, "Рюрик"s};
    cout << cats << endl;
}