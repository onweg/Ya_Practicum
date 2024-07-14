#include <algorithm>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

pair<bool, double> CalcMedian(vector<double> samples) {
    // верните {true, медиана}, если она существует,
    // то есть вектор непустой,
    // иначе - {false, 0}
    // обратите внимание - вектор принимаем по значению,
    // так как его придётся немного подпортить, чтобы вернуть ответ
    int size = samples.size();
    if (size == 0) {
        return {false, 0};
    }
    double result;
    if (size % 2 == 0) {
        auto median_right = samples.begin() + size / 2;
        nth_element(samples.begin(), median_right, samples.end());
        result = *median_right;
        
        auto median_left = samples.begin() + (size - 1) / 2;
        nth_element(samples.begin(), median_left, samples.end());
        result = (result + *median_left) / 2;
    } else {
        auto median = samples.begin() + size / 2;
        nth_element(samples.begin(), median, samples.end());
        result = *median;
    }
    return {true, result};
}

int main() {
    int size;
    cin >> size;

    vector<double> samples;
    for (int i = 0; i < size; ++i) {
        double sample;
        cin >> sample;
        samples.push_back(sample);
    }

    pair<bool, double> result = CalcMedian(samples);
    if (result.first) {
        cout << result.second << endl;
    } else {
        cout << "Empty vector"s << endl;
    }
}
