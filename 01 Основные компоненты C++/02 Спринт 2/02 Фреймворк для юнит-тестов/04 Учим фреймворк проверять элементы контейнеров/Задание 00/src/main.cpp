#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

template<typename T>
void PrintContainer(ostream& out, T container) {
    size_t i = 0, size = container.size();
    for (const auto& item : container) {
        out << item;
        if (i + 1 < size) out << ", "s;
        i++;
    }
}

template<typename T>
ostream& operator<< (ostream& out, const vector<T>& container) {
    out << "["s;
    PrintContainer(out, container);
    out << "]"s;
    return out;
}

template<typename T>
ostream& operator<< (ostream& out, const set<T>& container) {
    out << "{"s;
    PrintContainer(out, container);
    out << "}"s;
    return out;
}

template<typename T, typename U>
ostream& operator<< (ostream& out, const map<T, U>& container) {
    size_t i = 0, size = container.size();
    out << "{"s;
    for (const auto& [key, value] : container) {
        out << key << ": "s << value;
        if (i + 1 < size) out << ", "s;
        i++;
    }
    out << "}"s << endl;
    return out;
}

template<typename T>
bool operator==(const vector<T>& lhs, const vector<T>& rhs) {
    if (lhs.size() != rhs.size()) return false;
    size_t size = lhs.size();
    for (size_t i = 0; i < size; ++i) {
        if (lhs[i] != rhs[i]) return false;
    }
    return true;
}

template<typename T>
bool operator!=(const vector<T>& lhs, const vector<T>& rhs) {
    return !(lhs == rhs);
}

template<typename T>
bool operator==(const set<T>& lhs, const set<T>& rhs){
    if (lhs.size() != rhs.size()) return false;
    return equal(lhs.begin(), lhs.end(), rhs.begin());
}

template<typename T>
bool operator!=(const set<T>& lhs, const set<T>& rhs) {
    return !(lhs == rhs);
}

template<typename T, typename U>
bool operator==(const map<T, U>& lhs, const map<T, U>& rhs) {
    if (lhs.size() != rhs.size()) return false;
    return equal(lhs.begin(), lhs.end(), rhs.begin());
}

template<typename T, typename U>
bool operator!=(const map<T, U>& lhs, const map<T, U>& rhs) {
    return !(lhs == rhs);
}
template <typename T, typename U>
void AssertEqualImpl(const T& t, const U& u, const string& t_str, const string& u_str, const string& file,
                     const string& func, unsigned line, const string& hint) {
    if (t != u) {
        cout << boolalpha;
        cout << file << "("s << line << "): "s << func << ": "s;
        cout << "ASSERT_EQUAL("s << t_str << ", "s << u_str << ") failed: "s;
        cout << t << " != "s << u << "."s;
        if (!hint.empty()) {
            cout << " Hint: "s << hint;
        }
        cout << endl;
        abort();
    }
}

#define ASSERT_EQUAL(a, b) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, ""s)

#define ASSERT_EQUAL_HINT(a, b, hint) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, (hint))

void AssertImpl(bool value, const string& expr_str, const string& file, const string& func, unsigned line,
                const string& hint) {
    if (!value) {
        cout << file << "("s << line << "): "s << func << ": "s;
        cout << "ASSERT("s << expr_str << ") failed."s;
        if (!hint.empty()) {
            cout << " Hint: "s << hint;
        }
        cout << endl;
        abort();
    }
}

#define ASSERT(expr) AssertImpl(!!(expr), #expr, __FILE__, __FUNCTION__, __LINE__, ""s)

#define ASSERT_HINT(expr, hint) AssertImpl(!!(expr), #expr, __FILE__, __FUNCTION__, __LINE__, (hint))

vector<int> TakeEvens(const vector<int>& numbers) {
    vector<int> evens;
    for (int x : numbers) {
        if (x % 2 == 0) {
            evens.push_back(x);
        }
    }
    return evens;
}

map<string, int> TakeAdults(const map<string, int>& people) {
    map<string, int> adults;
    for (const auto& [name, age] : people) {
        if (age >= 18) {
            adults[name] = age;
        }
    }
    return adults;
}

bool IsPrime(int n) {
    if (n < 2) {
        return false;
    }
    int i = 2;
    while (i * i <= n) {
        if (n % i == 0) {
            return false;
        }
        ++i;
    }
    return true;
}

set<int> TakePrimes(const set<int>& numbers) {
    set<int> primes;
    for (int number : numbers) {
        if (IsPrime(number)) {
            primes.insert(number);
        }
    }
    return primes;
}

int main() {
    {
        const set<int> numbers = {-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
        const set<int> expected_primes = {2, 3, 5, 7, 11, 13};
        ASSERT_EQUAL(TakePrimes(numbers), expected_primes);
    }

    {
        const map<string, int> people = {{"Ivan"s, 19}, {"Sergey"s, 16}, {"Alexey"s, 18}};
        const map<string, int> expected_adults = {{"Alexey"s, 18}, {"Ivan"s, 19}};
        ASSERT_EQUAL(TakeAdults(people), expected_adults);
    }

    {
        const vector<int> numbers = {3, 2, 1, 0, 3, 6};
        const vector<int> expected_evens = {2, 0, 6};
        ASSERT_EQUAL(TakeEvens(numbers), expected_evens);
    }
}