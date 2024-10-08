#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <cmath>

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
    double relevance;
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
        vector<string> words = SplitIntoWordsNoStop(document);
        int count_words = words.size();
        for (const string& word : words) {
            word_to_documents_[word].insert(document_id);
            if (word_to_documents_freqs_[word].count(document_id) == 0) {
                word_to_documents_freqs_[word][document_id] = count(words.begin(), words.end(), word) / double(count_words);
            }
        }
        ++document_count_;
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
    map<string, map<int, double> > word_to_documents_freqs_;
    int document_count_ = 0;
    
    Query ParseQuery(const string& query) const{
        Query result;
        for (const string& word: SplitIntoWordsNoStop(query)) {
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
        map<int, double> document_to_relevance;
        for (const string& word : parse_query.plus_words) {
            if (word_to_documents_freqs_.find(word) != word_to_documents_freqs_.end()){
                for (const auto id_documets : word_to_documents_.at(word)) {
                    document_to_relevance[id_documets] += word_to_documents_freqs_.at(word).at(id_documets) * log(double(document_count_) / word_to_documents_freqs_.at(word).size());
                }
            }
        }
        for (const string& word : parse_query.minus_words) {
            if (word_to_documents_freqs_.find(word) != word_to_documents_freqs_.end()){
                for (const auto id_documets : word_to_documents_.at(word)) {
                    document_to_relevance.erase(id_documets);
                }
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
