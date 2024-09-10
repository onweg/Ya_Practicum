#include <iostream>
#include <vector>

using namespace std;

template<typename T>
ostream& operator<<(ostream& out, const vector<T>& container) {
    int size =  static_cast<int>(container.size());
    for (int i = 0; i <size; ++i) {
        out << container[i];
        if (i + 1 < size) {
            out << ", "s;
        }
    }
    return out;
}

int main() {
    const vector<int> ages = {10, 5, 2, 12};
    cout << ages << endl;
}