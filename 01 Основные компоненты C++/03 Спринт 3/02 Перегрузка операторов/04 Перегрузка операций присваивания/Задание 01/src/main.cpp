#include <iostream>
#include <numeric>
#include <string>

using namespace std;

class Rational {
public:
    Rational() = default;

    Rational(int value)
        : numerator_(value)
        , denominator_(1)
    {
    }

    Rational(int numerator, int denominator)
        : numerator_(numerator)
        , denominator_(denominator)
    {
        Normalize();
    }

    Rational& operator+=(Rational rational) {
        numerator_ = numerator_ * rational.Denominator() + denominator_ * rational.Numerator();
        denominator_ *= rational.Denominator();
        Normalize();
        return *this;
    }

    Rational& operator-=(Rational rational) {
        numerator_ = numerator_ * rational.Denominator() - denominator_ * rational.Numerator();
        denominator_ *= rational.Denominator();
        Normalize();
        return *this;
    }

    Rational& operator*=(Rational rational) {
        numerator_ *= rational.Numerator();
        denominator_ *= rational.Denominator();
        Normalize();
        return *this;
    }

    Rational& operator/=(Rational rational) {
        numerator_ *= rational.Denominator();
        denominator_ *= rational.Numerator();
        Normalize();
        return *this;
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
        int n = gcd(numerator_, denominator_);
        numerator_ /= n;
        denominator_ /= n;
    }

    int numerator_ = 0;
    int denominator_ = 1;
};

ostream& operator<<(ostream& output, Rational rational) {
    return output << rational.Numerator() << '/' << rational.Denominator();
}

istream& operator>>(istream& input, Rational& rational) {
    int numerator;
    int denominator;
    char slash;
    if ((input >> numerator) && (input >> slash) && (slash == '/') && (input >> denominator)) {
        rational = Rational{numerator, denominator};
    }
    return input;
}

Rational operator+(Rational left, Rational right) {
    return left += right;
}

Rational operator-(Rational left, Rational right) {
    return left -= right;
}

Rational operator+(Rational value) {
    return value;
}

Rational operator-(Rational value) {
    return {-value.Numerator(), value.Denominator()};
}

int main() {
    Rational a;
    Rational b, c;
    cin >> a;
    a += 1;
    cin >> b;
    c = a + (b += 1);
    cout << a << " " << b << " " << c << endl;
    return 0;
}