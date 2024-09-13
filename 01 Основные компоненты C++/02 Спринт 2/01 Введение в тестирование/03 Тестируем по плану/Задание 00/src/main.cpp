#include <iostream>
#include <string>

using namespace std;

// Определяет, будет ли слово палиндромом
// text может быть строкой, содержащей строчные символы английского алфавита и пробелы
// Пустые строки и строки, состоящие только из пробелов, — это не палиндромы
bool IsPalindrome(const string& text) {
    // Напишите недостающий код
    int lhs = 0, size = text.size(), rhs = size - 1;
    if (!size) return false;
    bool only_space = true;
    while (lhs <= rhs) {
        while (lhs < size && text[lhs] == ' ') lhs++;
        if (lhs == size) break;
        only_space = false;
        while (rhs >= 0 && text[rhs] == ' ') rhs --;
        if (rhs < lhs) break;
        if (text[lhs] != text[rhs]) return false;
        lhs++;
        rhs--;
    }
    if (only_space) return false;
    return true;

}

int main() {
    string text;
    getline(cin, text);

    if (IsPalindrome(text)) {
        cout << "palindrome"s << endl;
    } else {
        cout << "not a palindrome"s << endl;
    }
}