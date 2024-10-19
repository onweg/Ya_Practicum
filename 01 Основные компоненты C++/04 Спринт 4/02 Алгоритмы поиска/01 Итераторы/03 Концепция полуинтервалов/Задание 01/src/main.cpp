//Заготовки кода здесь нет — вы легко справитесь без неё. А вот решение не забудьте сохранить — оно вам понадобится в следующем задании.
#include <iostream>
#include <set>
using namespace std;

template <typename iterator>
void PrintRange(iterator it_begin, iterator it_end) {
    while (it_begin != it_end) {
        cout << *it_begin;
        it_begin++;
        if (it_begin != it_end) cout << " "s;
    }
    cout << endl;
} 

template <typename Iterator, typename Element>
void FindAndPrint(Iterator it_begin, Iterator it_end, Element element) {
    auto it_element = find_if(it_begin, it_end, [element](Element find_elemnt){return find_elemnt == element;});
    PrintRange(it_begin, it_element);
    PrintRange(it_element, it_end);
}

int main() {
    set<int> test = {1, 1, 1, 2, 3, 4, 5, 5};
    FindAndPrint(test.begin(), test.end(), 3);
}