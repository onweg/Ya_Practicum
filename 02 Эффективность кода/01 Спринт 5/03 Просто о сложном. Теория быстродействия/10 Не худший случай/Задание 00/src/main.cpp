#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

bool is_find_if = true;

int EffectiveCount(const vector<int>& v, int n, int i) {
    // место для вашего решения
    int64_t expected_count = static_cast<int64_t>(v.size()) * (i + 1) / (n + 1);

    if (expected_count < log2(v.size())) {
        auto iter = find_if(v.begin(), v.end(), [i](int item){ return item > i;});
        return iter - v.begin();
    }
    is_find_if = false;
    auto iter = upper_bound(v.begin(), v.end(), i);
    if (iter == v.end()) {
        return 0;
    }
    return iter - v.begin();
}

int main() {
    static const int NUMBERS = 1'000'000;
    static const int MAX = 1'000'000'000;

    mt19937 r;
    uniform_int_distribution<int> uniform_dist(0, MAX);

    vector<int> nums;
    for (int i = 0; i < NUMBERS; ++i) {
        int random_number = uniform_dist(r);
        nums.push_back(random_number);
    }
    sort(nums.begin(), nums.end());

    int i;
    cin >> i;
    int result = EffectiveCount(nums, MAX, i);
    if (is_find_if) cout << "Using find_if: ";
    else cout << "Using upper_bound: ";
    cout << "Total numbers before "s << i << ": "s << result << endl;
}