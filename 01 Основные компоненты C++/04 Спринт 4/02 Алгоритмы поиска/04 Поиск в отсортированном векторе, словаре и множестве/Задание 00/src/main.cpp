#include <iostream>
#include <set>
using namespace std;

set<int>::const_iterator FindNearestElement(const set<int>& numbers, int border) {
    // set<int>::const_iterator — тип итераторов для константного множества целых чисел
    auto lower = numbers.lower_bound(border);
    if (lower == numbers.end()){
        if (lower == numbers.begin()) return lower;
        else return --lower;
    }
    if (*lower == border) return lower;
    if (lower == numbers.begin()) return lower;
    auto upper = lower;
    --lower;
    if (border - *lower <= *upper - border) return lower;
    return upper;
}

int main() {
    set<int> numbers = {1, 4, 6};
    cout << *FindNearestElement(numbers, 0) << " " << *FindNearestElement(numbers, 3) << " "
         << *FindNearestElement(numbers, 5) << " " << *FindNearestElement(numbers, 6) << " "
         << *FindNearestElement(numbers, 100) << endl;

    set<int> empty_set;

    cout << (FindNearestElement(empty_set, 8) == end(empty_set)) << endl;

    set<int> num = {1};
    cout << *FindNearestElement(numbers, 0) << " " << *FindNearestElement(numbers, 1) << " " << *FindNearestElement(numbers, 2) << endl;
    return 0;
}