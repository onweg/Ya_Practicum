#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

bool comp(string s) {
    if (s.size())
        return s[0] == 'a' || s[0] == 'A';
    return false;
}

int CountStartsWithA(const vector<string>& xs) {
    // посчитайте число строк, начинающихся на букву A
    return count_if(xs.begin(), xs.end(), comp);
//    int count = 0;
//    for (string s : xs) {
//        if (s.size()) {
//            count += (s[0] == 'a' || s[0] == 'A');
//        }
//    }
//    return count;
}

int main() {
    // не меняйте тело main
    cout << CountStartsWithA({"And", "another", "one", "gone", "another", "one", "bites", "the", "dust", ""});
		return 0;
} 
