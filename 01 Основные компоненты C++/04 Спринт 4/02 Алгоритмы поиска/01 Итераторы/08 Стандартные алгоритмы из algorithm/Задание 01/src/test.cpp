#include <iostream>
#include <vector>
#include <sstream>
#include <numeric>
#include <algorithm>
#include <random> 

using namespace std;

template<typename It>
string PrintRangeToString(It range_begin, It range_end) {
	  // удобный тип ostringstream -> https://ru.cppreference.com/w/cpp/io/basic_ostringstream
    ostringstream out;
    for (auto it = range_begin; it != range_end; ++it) {
        out << *it << " "s;
    }
    out << endl;
	  // получаем доступ к строке с помощью метода str для ostringstream
    return out.str();
}

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

void MergeSort(vector<int>& a) {
    if (a.size() == 1) {
        return ;
    }
    auto left = vector<int>(a.begin(), a.begin() + a.size() / 2);
    auto right = vector<int>(a.begin() + a.size() / 2, a.end());

    MergeSort(left);
    MergeSort(right);

    size_t i = 0;
    size_t l_i = 0, l_size = left.size();
    size_t r_i = 0, r_size = right.size();
    while (l_i < l_size && r_i < r_size) {
        if (left[l_i] < right[r_i]) {
            a[i++] = left[l_i++];
        } else {
            a[i++] = right[r_i++];
        }
    }
    while (l_i < l_size) {
        a[i++] = left[l_i++];
    }
    while (r_i < r_size) {
        a[i++] = right[r_i++];
    }
    return ;
}

template<typename Item>
bool operator==(const vector<Item>& lhs, const vector<Item>& rhs) {
    if (lhs.size() != rhs.size()) return false;
    for (size_t i = 0; i < lhs.size(); ++i) {
        if (lhs[i] != rhs[i]) return false;
    }
    return true;
}

void TestMergeSort() {
    int count_successful = 0;
    for (int i = 0; i < 1000; i++) {
        vector<int> test_vector1(100);
        iota(test_vector1.begin(), test_vector1.end(), 1);
        std::random_device rd;   // Источник случайных чисел
        std::mt19937 g(rd());    // Генератор на основе Mersenne Twister
        std::shuffle(test_vector1.begin(), test_vector1.end(), g);
        vector<int> test_vector2 = vector<int>(test_vector1.begin(), test_vector1.end());
        MergeSort(test_vector1.begin(), test_vector1.end());
        sort(test_vector2.begin(), test_vector2.end());
        if (test_vector1 != test_vector2) {
            cout << "Неверная сортровка!\n";
            cout << "Первый массив: " << PrintRangeToString(test_vector1.begin(), test_vector1.end());
            cout << "Второй массив: " << PrintRangeToString(test_vector2.begin(), test_vector2.end());
        } else {
            ++count_successful;
        }
    }
    cout << count_successful << endl;
}

int main() {
    TestMergeSort();

    /*
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
    */
    return 0;
}

