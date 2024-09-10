#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
using namespace std;


template <typename Term>
// вычислеяет TF-IDF
auto ComputeTfIdfs(const vector<vector<Term> >& documents, const Term term) {
    int document_freq = 0;
    for (const auto& document : documents) 
        if (find(document.begin(), document.end(), term) != document.end()) 
            ++document_freq;
    double idf = document_freq > 0 ? log(double(documents.size()) / document_freq) : 0;
    vector<double> freqs;
    for (const auto& document : documents) {
        if (document.empty()) {
            freqs.push_back(0.0);
        } else {
            freqs.push_back(1.0 * count(document.begin(), document.end(), term) / document.size() * idf);
        }
    }
    return freqs;
}

int main() {
    const vector<vector<string>> documents = {
        {"белый"s, "кот"s, "и"s, "модный"s, "ошейник"s},
        {"пушистый"s, "кот"s, "пушистый"s, "хвост"s},
        {"ухоженный"s, "пёс"s, "выразительные"s, "глаза"s},
    };
    const auto& tf_idfs = ComputeTfIdfs(documents, "кот"s);
    for (const double tf_idf : tf_idfs) {
        cout << tf_idf << " "s;
    }
    cout << endl;
    return 0;
}