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

int main() {
    set<int> test = {1, 1, 1, 2, 3, 4, 5, 5};
    PrintRange(test.begin(), test.end());
}