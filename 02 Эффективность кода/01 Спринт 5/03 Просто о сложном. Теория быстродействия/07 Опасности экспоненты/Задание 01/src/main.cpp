#include <iostream>
#include <string>

using namespace std;

template <typename F>
string BruteForce(F check) {
    // верните строку str, для которой check(str) будет true
    string pass;
    for (auto ch1 = 'A'; ch1 <= 'Z'; ch1++) {
        for (auto ch2 = 'A'; ch2 <= 'Z'; ch2++) {
            for (auto ch3 = 'A'; ch3 <= 'Z'; ch3++) {
                for (auto ch4 = 'A'; ch4 <= 'Z'; ch4++) {
                    for (auto ch5 = 'A'; ch5 <= 'Z'; ch5++) {
                        pass = "";
                        pass += ch1;
                        pass += ch2;
                        pass += ch3;
                        pass += ch4;
                        pass += ch5;
                        if (check(pass)) {
                            return pass;
                        }
                    }   
                }       
            }
        }
    }
    return "";
}

int main() {
    string pass = "ARTUR"s;
    auto check = [pass](const string& s) {
        return s == pass;
    };
    cout << BruteForce(check) << endl;
}