#include "log_duration.h"

#include <iostream>

using namespace std;

class StreamUntier {
public:
    // добавьте конструктор, деструктор
    // и дополнительные поля класса при необходимости
    StreamUntier(istream &in) 
    : in_(in)
    {
        tied_before_ = in_.tie(nullptr);
    }
    ~StreamUntier() {
         in_.tie(tied_before_);
    }

private:
    ostream* tied_before_;
    istream& in_;
};

int main() {
    LOG_DURATION("\\n with tie"s);

    StreamUntier guard(cin);
    int i;
    while (cin >> i) {
        cout << i * i << "\n"s;
    }

    return 0;
}
