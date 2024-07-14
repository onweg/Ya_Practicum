#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

/*
bool comp(const string& s) {
    if (s.size())
        return s[0] == 'a' || s[0] == 'A';
    return false;
}
*/

int CountStartsWithA(const vector<string>& xs) {
    // посчитайте число строк, начинающихся на букву A
    /*
    int count = 0;
    for (string s : xs) {
        if (s.size()) {
            count += (s[0] == 'a' || s[0] == 'A');
        }
    }
    return count;
    */
    
    //return count_if(xs.begin(), xs.end(), comp);
    
    char a = 'a', A = 'A';
    return count_if(xs.begin(), xs.end(), [a, A](const string& s) { return s.size() > 0 && (s[0] == a || s[0] == A); });
}

int main() {
    // не меняйте тело main
    cout << CountStartsWithA({"And"s, "another"s, "one"s, "gone"s, "another"s, "one"s, "bites"s, "the"s, "dust"s, ""s}) << endl;
		return 0;
} 
