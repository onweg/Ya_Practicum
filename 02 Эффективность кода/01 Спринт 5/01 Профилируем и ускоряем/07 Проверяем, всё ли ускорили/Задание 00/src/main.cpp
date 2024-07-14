#include <chrono>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "log_duration.h"

using namespace std;

vector<int> ReverseVector(const vector<int>& source_vector) {
    vector<int> res;
    
    for (int i : source_vector) {
        res.insert(res.begin(), i);
    }

    return res;
}

int CountPops(const vector<int>& source_vector, int begin, int end) {
    int res = 0;

    for (int i = begin; i < end; ++i) {
        if (source_vector[i]) {
            ++res;
        }
    }

    return res;
}

void FillRandom(vector<int>& v, int n) {
    for (int i = 0; i < n; ++i) {
        // получаем случайное число с помощью функции rand.
        // в C++ имеются более современные генераторы случайных чисел,
        // но в данном уроке не будем их касаться
        v.push_back(rand() % 2);
    }
}

void Operate() {
    LOG_DURATION("Total"s);

    vector<int> random_bits;

    // операции << для целых чисел это сдвиг всех бит в двоичной
    // записи числа. Запишем с её помощью число 2 в степени 17 (131072)
    static const int N = 1 << 17;

    // заполним вектор случайными числами 0 и 1
    
    {
        LOG_DURATION("Fill random"s);
        FillRandom(random_bits, N);
    }

    // перевернём вектор задом наперёд
    vector<int> reversed_bits;
    {
        LOG_DURATION("Reverse"s);

        reversed_bits = ReverseVector(random_bits);
    }

    {
        LOG_DURATION("Counting"s);
        // посчитаем процент единиц на начальных отрезках вектора
        for (int i = 1, step = 1; i <= N; i += step, step *= 2) {
            double rate = CountPops(reversed_bits, 0, i) * 100. / i;
            cout << "After "s << i << " bits we found "s << rate << "% pops"s << endl;
        }
    }
}

int main() {
    Operate();
    return 0;
}