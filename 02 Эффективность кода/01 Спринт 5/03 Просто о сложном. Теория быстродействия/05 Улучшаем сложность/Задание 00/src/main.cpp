#include <algorithm>
#include <iostream>
#include <vector>
#include <random>

using namespace std;

// функция возвращает true, если векторы из одинаковых элементов
// перепишите эту функцию, улучшив её асимптотическую сложность
bool TestPermut(const vector<int>& v1, const vector<int>& v2) {
    // если они разной длины, элементы заведомо разные
    if (v1.size() != v2.size()) {
        return false;
    }
    auto v1_copy = v1;
    auto v2_copy = v2;
    sort(v1_copy.begin(), v1_copy.end());
    sort(v2_copy.begin(), v2_copy.end());
    for (size_t i = 0; i < v1_copy.size(); i++) {
        // проверяем, что каждый элемент первого вектора
        // содержится одинаковое количество раз в обоих векторах
        if (v1_copy[i] != v2_copy[i]){
            return false;
        }
    }

    return true;
}

int main() {
    std::mt19937 g;

    int n;
    cin >> n;
    vector<int> v1, v2;
    v1.reserve(n);
    v2.reserve(n);

    for (int i = 0; i < n; ++i) {
        v1.push_back(rand());
        v2.push_back(rand());
    }

    // оба вектора случайны, вряд ли они совпадут
    cout << "Random vectors match? "s << flush;
    cout << (TestPermut(v1, v2) ? "Yes"s : "No"s) << endl;

    // делаем один перестановкой другого явным образом
    v2 = v1;
    shuffle(v2.begin(), v2.end(), g);
    cout << "Permuted vectors match? "s << flush;
    cout << (TestPermut(v1, v2) ? "Yes"s : "No"s) << endl;
}