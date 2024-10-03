#include <iostream>
#include <numeric>
#include <vector> 

using namespace std;

class Rational {
public:

    Rational() {
        numerator_ = 0;
        denominator_ = 1;
    }

    Rational(int numerator) {
        numerator_ = numerator;
        denominator_ = 1;
    }

    Rational (int numerator, int denominator) {
        normal_form(numerator, denominator);
        numerator_ = numerator;
        denominator_ = denominator;
    }

    void normal_form(int& numerator, int& denominator) {
        int sign = ((numerator > 0 && denominator > 0) || (numerator < 0 && denominator < 0)) ? 1 : -1;
        numerator = abs(numerator);
        denominator = abs(denominator);
        int gcd_ = gcd(numerator, denominator);
        numerator = numerator / gcd_ * sign;
        denominator = denominator / gcd_;
    }

    int Numerator() const {
        return numerator_;
    }

    int Denominator() const {
        return denominator_;
    }

private:
    int numerator_;
    int denominator_;
};

Rational Add(Rational a, Rational b) {
    int lcm = a.Denominator() / gcd(a.Denominator(), b.Denominator()) * b.Denominator();
    return Rational(a.Numerator() * (lcm / a.Denominator()) + b.Numerator() * (lcm / b.Denominator()), lcm);
}

int main() {
    Rational zero;     // Дробь 0/1 = 0
    const Rational seven(7); // Дробь 7/1 = 7
    const Rational one_third(1, 3); // Дробь 1/3

    vector<Rational> numbers;
    numbers.push_back(Rational{7, 8});

    // Следующие 2 строки эквивалентны - добавляют в numbers дробь 3/1
    numbers.push_back(Rational{3});
    numbers.push_back(3);

    Rational sum = Add(Rational{1,6}, one_third);
    // Выведет 1/2
    cout << sum.Numerator() << "/" << sum.Denominator();
}