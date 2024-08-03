#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

using namespace std;

const int MAX_RESULT_DOCUMENT_COUNT = 5;

string ReadLine() {
    string s;
    getline(cin, s);
    return s;
}

int ReadLineWithNumber() {
    int result;
    cin >> result;
    ReadLine();
    return result;
}

vector<string> SplitIntoWords(const string& text) {
    vector<string> words;
    string word;
    for (const char c : text) {
        if (c == ' ') {
            words.push_back(word);
            word = "";
        } else {
            word += c;
        }
    }
    words.push_back(word);
    
    return words;
}

struct Document {
    int id;
    int relevance;
};

struct Query {
    vector<string> plus_words;
    vector<string> minus_words;
};

class SearchServer {
public:
    void SetStopWords(const string& text) {
        for (const string& word : SplitIntoWords(text)) {
            stop_words_.insert(word);
        }
    }
    
    void AddDocument(int document_id, const string& document) {
        for (const string& word : SplitIntoWordsNoStop(document)) {
            word_to_documents_[word].insert(document_id);
        }
    }
    
    vector<Document> FindTopDocuments(const string& query) const {
        auto matched_documents = FindAllDocuments(query);
        
        sort(
             matched_documents.begin(),
             matched_documents.end(),
             [](const Document& lhs, const Document& rhs) {
                 return lhs.relevance > rhs.relevance;
             }
             );
        if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }
        return matched_documents;
    }
    
private:
    map<string, set<int>> word_to_documents_;
    set<string> stop_words_;
    
    Query ParseQuery(const string& query) const{
        Query result;
        for (const string& word: SplitIntoWords(query)) {
            if (word[0] == '-') {
                result.minus_words.push_back(word.substr(1));
            } else {
                result.plus_words.push_back(word);
            }
        }
        return result;
    }
    
    vector<string> SplitIntoWordsNoStop(const string& text) const {
        vector<string> words;
        for (const string& word : SplitIntoWords(text)) {
            if (stop_words_.count(word) == 0) {
                words.push_back(word);
            }
        }
        return words;
    }
    
    vector<Document> FindAllDocuments(const string& query) const {
        Query parse_query = ParseQuery(query);
        map<int, int> document_to_relevance;
        for (const string& word : parse_query.plus_words) {
            if (word_to_documents_.count(word) == 0) {
                continue;
            }
            for (const int document_id : word_to_documents_.at(word)) {
                ++document_to_relevance[document_id];
            }
        }
        for (const string& word : parse_query.minus_words) {
            if (word_to_documents_.count(word) == 0) {
                continue;
            }
            for (const int document_id : word_to_documents_.at(word)) {
                document_to_relevance.erase(document_to_relevance.find(document_id));
            }
        }
        vector<Document> matched_documents;
        for (auto [document_id, relevance] : document_to_relevance) {
            matched_documents.push_back({document_id, relevance});
        }
        
        return matched_documents;
    }
};

SearchServer CreateSearchServer() {
    SearchServer search_server;
    search_server.SetStopWords(ReadLine());
    
    const int document_count = ReadLineWithNumber();
    for (int document_id = 0; document_id < document_count; ++document_id) {
        search_server.AddDocument(document_id, ReadLine());
    }
    
    return search_server;
}


int main() {
    const SearchServer search_server = CreateSearchServer();
    
    const string query = ReadLine();
    for (auto [document_id, relevance] : search_server.FindTopDocuments(query)) {
        cout << "{ document_id = " << document_id << ", relevance = " << relevance << " }" << endl;
    }
}


TF - как часто встречается слово в документе
отношение количества сколько встретилось в документе на количество слов в этом документе
IDF - как часто встречаеся в документах
логорифм (10) отношения количество документов в котором встречается на количество всего документов
Повторение слова в документе увеличивает TF. А сколько раз слово повторится в запросе, столько раз нужно будет перемножить TF и IDF этого слова.

и в на
3
кот TF = 1  IDF = log10(3/2)
пушистый кот пушистый
ухоженный

пушистый ухоженный кот
