#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

using namespace std;

int main() {
    // считайте входные данные и сформируйте вывод программы
    // ...
    int n;
    cin >> n;
    vector<string> v;
    string s;
    for (int i = 0; i < n; i++) {
        cin >> s;
        v.push_back(s);
    }
    sort(v.begin(), v.end(),
        [](const string& lhs_s, const string& rhs_s){
        return lexicographical_compare(lhs_s.begin(), lhs_s.end(), rhs_s.begin(), rhs_s.end(),
                                       [](char a, char b){return tolower(a) < tolower(b);});
            /*
            int i = 0;
            int size_lhs_s = int(lhs_s.size()), size_rhs_s = int(rhs_s.size());
            while (i < size_lhs_s && i < size_rhs_s) {
                char lhs_ch = tolower(lhs_s[i]);
                char rhs_ch = tolower(rhs_s[i]);
                if (lhs_ch != rhs_ch) return lhs_ch < rhs_ch;
                i++;
            }
            if (i == size_lhs_s) return true;
            return false;
            */
        }
        );
    for (const string& i : v) {
        cout << i << ' ';
    }
    cout << endl;
    return 0;
}
