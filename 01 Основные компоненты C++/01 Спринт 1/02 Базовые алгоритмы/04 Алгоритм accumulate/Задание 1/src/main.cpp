#include <iostream>
#include <vector>
#include <numeric>
#include <cstddef>

using namespace std;

double Average(const vector<int>& xs) {
    // посчитайте среднее арифметическое: поделите сумму на размер
    // суммирование начните с 0.0 - тогда результат выражения будет вещественным числом
    size_t size = xs.size();
    if (size == 0) {
        return 0;
    }
    return accumulate(xs.begin(), xs.end(), 0.0) / size;
}

int main() {
    // не меняйте код main
    cout << Average({1, 2, 2}) << endl;
    return 0;
}
