#include <iostream>

using namespace std;

class Greeter{
public:
    explicit Greeter(const string& text) 
    : text_(text) 
    {
        cout << "Hello "s << text_ << "!\n"s;
    }

    ~Greeter() {
        cout << "Buy "s << text_ << "!\n"s;
    }
private:
    string text_;
};

Greeter glob("global"s);

int main() {
    Greeter mn("main"s);
    return 0;
}

Greeter glob2("global2"s);