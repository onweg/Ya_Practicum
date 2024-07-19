#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Document {
	int id;
	int relevance;
};

bool HasDocumentGreaterRelevance(Document lhs, Document rhs) {
		return lhs.relevance < rhs.relevance;
}

int main() {
    vector<Document> document = {{1, 12}, {2, 10}, {3, 11}};
    sort(document.begin(), document.end(), HasDocumentGreaterRelevance);
    for (auto i : document) {
        cout << i.id << ' ' << i.relevance << endl;
    }
    return 0;
}
