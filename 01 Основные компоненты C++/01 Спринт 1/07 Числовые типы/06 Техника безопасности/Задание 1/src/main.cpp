#include <algorithm>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Document {
    int id;
    int rating;
}; 

void PrintDocuments(vector<Document> documents, size_t skip_start, size_t skip_finish) {
		// отсортировать документы и вывести не все
    if (documents.empty()) return 0;
    sort(documents.begin(), documents.end(), [](const Document& a, const Document& b){return a.rating > b.rating;});
    const auto size = documents.size();
    if (skip_finish > size) skip_finish = size;
    for (auto i = skip_start; i < size - skip_finish; i++) {
        cout << "{ id = "s << documents[i].id << ", rating = "s << documents[i].rating << " }" << endl;
    }
}

int main() {
    PrintDocuments(
        {
            {100, 5},
            {101, 7},
            {102, -4},
            {103, 9},
            {104, 1}
        },
        1,
        2
    );
}  
