#include <iostream>
#include <vector>
#include <numeric>

using namespace std;

class Rational {
public:
    Rational() = default;

    Rational(int numerator) 
    :numerator_(numerator), denominator_(1)
    {}

    Rational(int numerator, int denominator)
    :numerator_(numerator), denominator_(denominator)
    {
        Normalize();
    }

    int Numerator() const {
        return numerator_;
    }

    int Denominator() const {
        return denominator_;
    }

private:
    void Normalize() {
        if (denominator_ < 0) {
            numerator_ = -numerator_;
            denominator_ = -denominator_;
        }
        const int divisor = gcd(numerator_, denominator_);
        numerator_ /= divisor;
        denominator_ /= divisor;
    }

    int numerator_ = 0;
    int denominator_ = 1;
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

    Rational sum = Add(Rational{1,6}, Add(one_third, zero));
    // Выведет 1/2
    cout << sum.Numerator() << "/" << sum.Denominator();
}