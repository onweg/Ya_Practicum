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

template <typename Container, typename Iterator>
void EraseAndPrint(Container& container, Iterator iterator) {
    auto new_it = container.erase(iterator);
    PrintRange(container.begin(), new_it);
    PrintRange(new_it, container.end());
    
}

int main() {
    vector<string> langs = {"Python"s, "Java"s, "C#"s, "Ruby"s, "C++"s};
    EraseAndPrint(langs, langs.begin() + 1);
}