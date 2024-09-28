#include <iostream>
#include <string>
#include <vector>
    
using namespace std;

vector<string> SplitIntoWords(const string& text) {
    vector<string> words;
    for (size_t i = 0; i < text.size(); ++i) {
        if (text[i] == ' ') {
            continue;
        }
        const size_t space_pos = text.find(' ', i);
        if (space_pos == text.npos) {
            words.push_back(text.substr(i));
            break;
        } else {
            words.push_back(text.substr(i, space_pos - i));
            i = space_pos;
        }
    }
    return words;
}

int main() {
    const string query = "hello kitty"s;
    for (const auto& word : SplitIntoWords(query)) {
        cout << '[' << word << ']' << endl;
    }
}