#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

struct Document {
    int id;
    int rating;
}; 

void PrintDocuments(vector<Document> documents, size_t skip_start, size_t skip_finish) {
		// отсортировать документы и вывести не все
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
