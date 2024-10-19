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

bool PrefixIsBeginningWord(const string& prefix, const string& word) {
    if (prefix.size() > word.size()) return false;
    if (prefix.empty()) return true;

    for (size_t i = 0; i < prefix.size(); ++i) {
        if (prefix[i] != word[i]) return false;
    }

    return true;
}

void NextPrefix(string& prefix) {
    int i = static_cast<int>(prefix.size() - 1);
    while (i >= 0 && prefix[i] == 'z') {
        i--;
    }
    if (i >= 0) ++prefix[i];
}

template <typename RandomIt>
pair<RandomIt, RandomIt> FindStartsWith(RandomIt range_begin, RandomIt range_end, string prefix) {
    // напишите реализацию
    auto start_it = find_if(range_begin, range_end, [prefix](const string& word){
        return PrefixIsBeginningWord(prefix, word);
    });

    if (start_it == range_end) { // слов с таким префиксом нет
        while (start_it == range_end && prefix != string(prefix.size(), 'z')) { // ищу следующий префикс после начального prefix с которой начнется хотя бы одно слово 
            NextPrefix(prefix);
            start_it = find_if(range_begin, range_end, [prefix](const string& word){
                return PrefixIsBeginningWord(prefix, word);
                });
        }
        return make_pair(start_it, start_it);
    } else { // слова с таким префиксом есть, надо найти конец этого диапазона
        auto end_it = start_it;
        while(end_it != range_end && PrefixIsBeginningWord(prefix, *end_it)) {
            end_it++;
        }
        return make_pair(start_it, end_it);
    }
}

int main() {
    const vector<string> sorted_strings = {"moscow", "motovilikha", "murmansk"};

    const auto mo_result = FindStartsWith(begin(sorted_strings), end(sorted_strings), "mo");
    for (auto it = mo_result.first; it != mo_result.second; ++it) {
        cout << *it << " ";
    }
    cout << endl;

    const auto mt_result = FindStartsWith(begin(sorted_strings), end(sorted_strings), "mt");
    cout << (mt_result.first - begin(sorted_strings)) << " " << (mt_result.second - begin(sorted_strings)) << endl;

    const auto na_result = FindStartsWith(begin(sorted_strings), end(sorted_strings), "na");
    cout << (na_result.first - begin(sorted_strings)) << " " << (na_result.second - begin(sorted_strings)) << endl;

    const auto ma_result = FindStartsWith(begin(sorted_strings), end(sorted_strings), "ma");
    cout << (ma_result.first - begin(sorted_strings)) << " " << (ma_result.second - begin(sorted_strings)) << endl;

    const auto zz_result = FindStartsWith(begin(sorted_strings), end(sorted_strings), "");
    cout << (zz_result.first - begin(sorted_strings)) << " " << (zz_result.second - begin(sorted_strings)) << endl;

    return 0;
}