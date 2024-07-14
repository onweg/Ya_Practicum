#include <iostream>
#include <string>
#include <set>
#include <numeric>

using namespace std;

string AddStopWords(const string& query, const set<string>& stop_words) {
    return accumulate(stop_words.begin(), stop_words.end(), query, [](const string& a, const string& b){return a + " -" + b;});
}

int main() {
    set<string> stop_words = {"of"s, "in"s};
    string query = "some tasty oranges"s;
    cout << AddStopWords(query, stop_words) << endl;
}
