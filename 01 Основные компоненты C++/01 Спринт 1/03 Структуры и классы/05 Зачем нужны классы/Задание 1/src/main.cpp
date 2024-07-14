#include <map>
#include <set>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

class SearchServer {
    map<string, set<int>> word_to_documents_;
    set<string> stop_words_;
public:
    int GetStopWordsSize() const {
        // ...
    }
	
};

int GetStopWordsSize(const SearchServer& server) {
    //...
}

int main() {
		// ваш код
}