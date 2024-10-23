#include <cstdlib>
#include <iostream>
#include <vector>
#include <chrono>

using namespace std;
using namespace chrono;

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

void AppendRandom(vector<int>& v, int n) {
    for (int i = 0; i < n; ++i) {
        // получаем случайное число с помощью функции rand.
        // с помощью (rand() % 2) получим целое число в диапазоне 0..1.
        // в C++ имеются более современные генераторы случайных чисел,
        // но в данном уроке не будем их касаться
        v.push_back(rand() % 2);
    }
}

void Operate() {
    vector<int> random_bits;

    // операция << для целых чисел это сдвиг всех бит в двоичной
    // записи числа. Запишем с её помощью число 2 в степени 17 (131072)
    static const int N = 1 << 17;

    // заполним вектор случайными числами 0 и 1
    const steady_clock::time_point start_time_AR = steady_clock::now();
    AppendRandom(random_bits, N);
    const steady_clock::time_point finish_time_AR = steady_clock::now();
    const steady_clock::duration duration_AR = finish_time_AR - start_time_AR;

    // перевернём вектор задом наперёд
    const steady_clock::time_point start_time_revers = steady_clock::now();
    vector<int> reversed_bits = ReverseVector(random_bits);
    const steady_clock::time_point finish_time_reverse = steady_clock::now();
    const steady_clock::duration duration_reverse = finish_time_reverse - start_time_revers;

    const steady_clock::time_point start_time_CP = steady_clock::now();
    // посчитаем процент единиц на начальных отрезках вектора
    for (int i = 1, step = 1; i <= N; i += step, step *= 2) {
        // чтобы вычислить проценты, мы умножаем на литерал 100. типа double;
        // целочисленное значение функции CountPops при этом автоматически
        // преобразуется к double, как и i
        double rate = CountPops(reversed_bits, 0, i) * 100. / i;
        cout << "After "s << i << " bits we found "s << rate << "% pops"s
             << endl;
    }
    const steady_clock::time_point finish_time_CP = steady_clock::now();
    const steady_clock::duration duration_CP = finish_time_CP - start_time_CP;


    cerr << "Append random: "s << duration_cast<milliseconds>(duration_AR).count() << "ms"s << endl;
    cerr << "Reverse: "s << duration_cast<milliseconds>(duration_reverse).count() << "ms"s << endl;
    cerr << "Counting: " << duration_cast<milliseconds>(duration_CP).count() <<  "ms"s << endl;

}

int main() {
    Operate();
    return 0;
}