#include <algorithm>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

string ReadLine() {
    string s;
    getline(cin, s);
    return s;
}

int ReadLineWithNumber() {
    int result;
    cin >> result;
    ReadLine();
    return result;
}

int main() {
    int queryCount = ReadLineWithNumber();

    vector<string> queries(queryCount);
    for (string& query : queries) {
        query = ReadLine();
    }
    string buzzword = ReadLine();

    cout << count_if(queries.begin(), queries.end(), [buzzword](const string& query) {
        // Реализуйте эту лямбда-функцию
        return query.find(buzzword) != string::npos;
    });
    cout << endl; 
}

// не рассматриваю случа если стоп слово это a а мое слово abc то оно считает как запратное слово, хотя  слова разные
