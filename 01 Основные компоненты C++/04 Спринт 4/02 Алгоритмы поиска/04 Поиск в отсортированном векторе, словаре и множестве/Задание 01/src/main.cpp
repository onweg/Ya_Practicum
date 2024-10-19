#include <iostream>
#include <vector>
using namespace std;


template <typename RandomIt>
pair<RandomIt, RandomIt> FindStartsWith(RandomIt range_begin, RandomIt range_end, char prefix) {
    // напишите реализацию
    auto start_it = find_if(range_begin, range_end, [prefix](const string& word){
        if (!word.empty()) return word[0] == prefix;
        return false;
    });
    if (start_it == range_end) { // слов с такой буквой нет
        while (start_it == range_end && prefix < 'z') { // ищу первую букву после prefix с котороц начинается хотя бы одно слово 
            ++prefix;
            start_it = find_if(range_begin, range_end, [prefix](const string& word){
                if (!word.empty()) return word[0] == prefix;
                return false;
            });
        }
        return make_pair(start_it, start_it);
    } else { // слов с такой буквой есть, надо найти конец этого диапазона
        auto end_it = start_it;
        while(end_it != range_end && (*end_it)[0] == prefix) {
            end_it++;
        }
        return make_pair(start_it, end_it);
    }

}

int main() {
    const vector<string> sorted_strings = {"moscow", "murmansk", "vologda"};

    const auto m_result = FindStartsWith(begin(sorted_strings), end(sorted_strings), 'm');
    for (auto it = m_result.first; it != m_result.second; ++it) {
        cout << *it << " ";
    }
    cout << endl;

    const auto p_result = FindStartsWith(begin(sorted_strings), end(sorted_strings), 'p');
    cout << (p_result.first - begin(sorted_strings)) << " " << (p_result.second - begin(sorted_strings)) << endl;

    const auto z_result = FindStartsWith(begin(sorted_strings), end(sorted_strings), 'z');
    cout << (z_result.first - begin(sorted_strings)) << " " << (z_result.second - begin(sorted_strings)) << endl;

    const auto a_result = FindStartsWith(begin(sorted_strings), end(sorted_strings), 'a');
    cout << (a_result.first - begin(sorted_strings)) << " " << (a_result.second - begin(sorted_strings)) << endl;

    const auto v_result = FindStartsWith(begin(sorted_strings), end(sorted_strings), 'v');
    cout << (v_result.first - begin(sorted_strings)) << " " << (v_result.second - begin(sorted_strings)) << endl;

    return 0;
}