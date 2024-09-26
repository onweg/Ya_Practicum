#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

void AssertImpl(bool value, const string& str, const string& file, const int& line, const string& function, const string& hint = ""s) {
    if (!value){
        cout << file << " ("s << line << "): "s << function << ": ASSERT("s << str << ") failed."s;
        if (!hint.empty()) {
            cout << " Hint: "s << hint;
        }
        cout << endl;
    }
}

#define ASSERT(expr) AssertImpl((expr), #expr, __FILE__, __LINE__, __FUNCTION__, ""s);

#define ASSERT_HINT(expr, hint) AssertImpl((expr), #expr, __FILE__, __LINE__, __FUNCTION__, hint);

int main() {
    string hello = "hello"s;
    ASSERT(!hello.empty());
    ASSERT_HINT(2 + 2 == 5, "This will fail"s);
}