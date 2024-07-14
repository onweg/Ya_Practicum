#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

#define ASSERT(expr) /* реализовать самостоятельно */

#define ASSERT_HINT(expr, hint) /* реализовать самостоятельно */

int main() {
    string hello = "hello"s;
    ASSERT(!hello.empty());
    ASSERT_HINT(2 + 2 == 5, "This will fail"s);
}