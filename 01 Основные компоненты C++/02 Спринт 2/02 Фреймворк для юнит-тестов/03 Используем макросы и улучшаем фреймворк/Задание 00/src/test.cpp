#include <iostream>

using namespace std;

// Директивой #define объявляем макрос GREETING параметрами s1 и s2
#define GREETING(s1, s2) cout << "Hello, "s << (s1) << " and "s << (s2) << endl

int main() {
    // Будет заменено на:
    // cout << "Hello, "s << ("Ivan"s) << " and "s << ("Maria"s) << endl;
    GREETING("Ivan"s, "Maria"s);

    // Будет заменено на:
    // cout << "Hello, "s << (5 + 5) << " and "s << (42) << endl;
    GREETING(5 + 5, 42);
}