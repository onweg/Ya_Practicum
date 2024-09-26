#include <iostream>
#include <string>

using namespace std;

template <typename function>
void RunTestImpl(function func, const string& func_str) {
    func();
    cerr << func_str << " OK"s << endl;
}

#define RUN_TEST(func) RunTestImpl(func, #func)

void Test1() {
}

int main() {
    RUN_TEST(Test1);
}