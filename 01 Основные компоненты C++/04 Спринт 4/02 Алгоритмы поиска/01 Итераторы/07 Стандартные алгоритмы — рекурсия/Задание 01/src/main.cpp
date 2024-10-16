#include <iostream>

using namespace std;

bool IsPowOfTwo(int n) {
    if (n == 1) return true;
    if (n < 1 || n % 2 != 0) return false;
    return IsPowOfTwo(n / 2);
}

int main() {
    cout << boolalpha << IsPowOfTwo(0) << endl;
    cout << boolalpha << IsPowOfTwo(1) << endl;
    cout << boolalpha << IsPowOfTwo(1024) << endl;
    cout << boolalpha << IsPowOfTwo(7) << endl;
    return 0;
}