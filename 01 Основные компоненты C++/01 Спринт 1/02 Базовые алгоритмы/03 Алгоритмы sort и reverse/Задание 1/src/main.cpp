#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

bool comp(pair<int, string> a, pair<int, string> b) {
    if (a.first == b.first) {
        return a.second < b.second;
    }
    return a.first > b.first;
}

int main() {
    int count;
    cin >> count;
    vector<pair<int, string>> peoples;
    for (int i = 0; i < count; ++i) {
        string name;
        int age;
        cin >> name >> age;
        // сохраните в вектор пар
        peoples.push_back({age, name});
    }
    sort(peoples.begin(), peoples.end(), comp);
    //reverse(peoples.begin(), peoples.end());
    
    for (const pair<int, string>& p : peoples) {
        cout << p.second << endl;
    }
    // выведите только имена в порядке убывания возраста
    // Jack
    // John
    // ...
}
