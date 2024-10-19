#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <random> 

using namespace std;

template <typename It>
void PrintRange(It range_begin, It range_end) {
    for (auto it = range_begin; it != range_end; ++it) {
        cout << *it << " "s;
    }
    cout << endl;
}

template<typename It>
void MergeSort(It range_begin, It range_end) {
    if (distance(range_begin, range_end) == 1) {
        return ;
    }
    auto left = vector<int>(range_begin, range_begin + distance(range_begin, range_end) / 2);
    auto right = vector<int>(range_begin + distance(range_begin, range_end) / 2, range_end);

    MergeSort(left.begin(), left.end());
    MergeSort(right.begin(), right.end());

    size_t l_i = 0, l_size = left.size();
    size_t r_i = 0, r_size = right.size();
    auto it = range_begin;
    while (l_i < l_size && r_i < r_size) {
        if (left[l_i] < right[r_i]) {
            *it = left[l_i++];
        } else {
            *it = right[r_i++];
        }
        advance(it, 1);
    }
    while (l_i < l_size) {
        *it = left[l_i++];
        advance(it, 1);
    }
    while (r_i < r_size) {
        *it = right[r_i++];
        advance(it, 1);
    }
    return ;
}
int main() {
    vector<int> test_vector(10);

    // iota             -> http://ru.cppreference.com/w/cpp/algorithm/iota
    // Заполняет диапазон последовательно возрастающими значениями
    iota(test_vector.begin(), test_vector.end(), 1);

    // random_shuffle   -> https://ru.cppreference.com/w/cpp/algorithm/random_shuffle
    // Перемешивает элементы в случайном порядке
    std::random_device rd;   // Источник случайных чисел
    std::mt19937 g(rd());    // Генератор на основе Mersenne Twister
    // Перемешиваем вектор с использованием std::shuffle
    std::shuffle(test_vector.begin(), test_vector.end(), g);

    // Выводим вектор до сортировки
    PrintRange(test_vector.begin(), test_vector.end());

    // Сортируем вектор с помощью сортировки слиянием
    MergeSort(test_vector.begin(), test_vector.end());

    // Выводим результат
    PrintRange(test_vector.begin(), test_vector.end());
    return 0;
}