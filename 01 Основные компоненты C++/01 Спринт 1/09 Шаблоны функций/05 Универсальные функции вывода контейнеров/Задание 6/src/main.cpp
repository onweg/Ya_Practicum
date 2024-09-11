#include <iostream>
#include <vector>
#include <set>
#include <map>

using namespace std;

template<typename T>
void Out_element(ostream& out, const T& element) {
    out << element;
}

template<typename Key, typename Val>
void Out_element(ostream& out, const pair<Key, Val>& element) {
    out << "("s << element.first << ", "s << element.second << ")"s;
}

template<typename Container>
void Print(ostream& out, const Container& container) {
    size_t i = 0, size = container.size();
    for (const auto& element : container) {
        Out_element(out, element);
        if (i + 1 < size) {
            out << ", ";
        }
        ++i;
    }
}

template<typename T>
ostream& operator<<(ostream& out, const set<T>& container) {
    out << "{"s;
    Print(out, container);
    out << "}"s;
    return out;
}

template<typename T>
ostream& operator<<(ostream& out, const vector<T>& container) {
    out << "["s;
    Print(out, container);
    out << "]"s;
    return out;
}

template <typename Key, typename Val>
ostream& operator<<(ostream& out, const map<Key, Val>& container) {
    out << "<"s;
    Print(out, container);
    out << ">"s;
    return out;
}

int main() {
    const set<string> cats = {"Мурка"s, "Белка"s, "Георгий"s, "Рюрик"s};
    cout << cats << endl;

    const vector<int> ages = {10, 5, 2, 12};
    cout << ages << endl;

    const map<string, int> cat_ages = {
        {"Мурка"s, 10}, 
        {"Белка"s, 5},
        {"Георгий"s, 2}, 
        {"Рюрик"s, 12}
    };
    cout << cat_ages << endl;
}