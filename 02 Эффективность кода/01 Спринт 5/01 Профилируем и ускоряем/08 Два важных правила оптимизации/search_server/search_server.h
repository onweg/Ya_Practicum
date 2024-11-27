#pragma once

#include <map>
#include <vector>
#include <set>
#include <string>
#include <numeric>
#include <algorithm>


#include "document.h"
#include "string_processing.h"


using namespace std;

const int MAX_RESULT_DOCUMENT_COUNT = 5;

enum class DocumentStatus {
    ACTUAL,
    IRRELEVANT,
    BANNED,
    REMOVED,
};

template <typename StringContainer> //создает множество непустых строк из запроса (нужно при инициализации)
set<string> MakeUniqueNonEmptyStrings(const StringContainer& strings) {
    set<string> non_empty_strings;
    for (const string& str : strings) {
        if (!str.empty()) {
            non_empty_strings.insert(str);
        }
    }
    return non_empty_strings;
}

class SearchServer {
public:
    template <typename StringContainer>
    explicit SearchServer(const StringContainer& stop_words)
        : stop_words_(MakeUniqueNonEmptyStrings(stop_words))
    {
        /*
        Конструкторы класса SearchServer должны выбрасывать исключение invalid_argument, если любое из переданных стоп-слов содержит недопустимые символы, то есть символы с кодами от 0 до 31.
        */
        for (const auto& word : stop_words_) {
            if (!IsValidString(word)) {
                throw invalid_argument("Некорректные данные для стоп слов!"s);
            }
        }

    }

    explicit SearchServer(const string& stop_words_text)
    //Походу это конструктор класса через первый конструктор
        : SearchServer(SplitIntoWords(stop_words_text))
    {}

    static bool IsValidString(const string& word);

    static bool CorrectUseMinusSignInText(const string& text);
    
    void AddDocument(int document_id, const string& document, DocumentStatus status, const vector<int>& ratings);

    template <typename DocumentPredicate>
    vector<Document> FindTopDocuments(const string& raw_query, DocumentPredicate document_predicate) const {
        /*
        Методы FindDocument должны выбрасывать исключение invalid_argument в следующих ситуациях:
        В словах поискового запроса есть недопустимые символы с кодами от 0 до 31;
        Наличие более чем одного минуса перед словами, которых не должно быть в искомых документах, например, пушистый --кот. В середине слов минусы разрешаются, например: иван-чай.
        Отсутствие текста после символа «минус»: в поисковом запросе: "пушистый -".
        */
        if (!IsValidString(raw_query) or !CorrectUseMinusSignInText(raw_query)) {
            throw invalid_argument("Некорректное использование запроса"s);
        }

        const auto& query = ParseQuery(raw_query);
        if (!query) {
            throw invalid_argument("Некорректное использование запроса"s);
        }
        
        auto matched_documents = FindAllDocuments(*query, document_predicate);

        sort(matched_documents.begin(), matched_documents.end(), [](const Document& lhs, const Document& rhs) {
            if (abs(lhs.relevance - rhs.relevance) < 1e-6) {
                return lhs.rating > rhs.rating;
            } else {
                return lhs.relevance > rhs.relevance;
            }
        });
        if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }
        
        return matched_documents;
    }

    vector<Document> FindTopDocuments(const string& raw_query, DocumentStatus status) const;

    vector<Document> FindTopDocuments(const string& raw_query) const;

    int GetDocumentCount() const;

    int GetDocumentId(int index) const;

    tuple<vector<string>, DocumentStatus> MatchDocument(const string& raw_query, int document_id) const;
    
private:
    struct DocumentData {
        int rating;
        DocumentStatus status;
    };

    const set<string> stop_words_; //стоп слова
    map<string, map<int, double>> word_to_document_freqs_; //слово и в каком id документе встречается и сколько весомости имеет
    map<int, DocumentData> documents_; //id документа и какие данные имеет
    vector<int> document_ids_; //id документа до индексу(id документа по добавлению в этот вектор) (странная тема но пусть)

    bool IsStopWord(const string& word) const;

    static bool IsValidWord(const string& word);

    optional<vector<string>> SplitIntoWordsNoStop(const string& text) const;

    static int ComputeAverageRating(const vector<int>& ratings);

    struct QueryWord {
        string data;
        bool is_minus;
        bool is_stop;
    };

    optional<QueryWord> ParseQueryWord(string text) const {
        // Empty result by initializing it with default constructed QueryWord
        if (text.empty()) {
            return nullopt;
        }
        bool is_minus = false;
        if (text[0] == '-') {
            is_minus = true;
            text = text.substr(1);
        }
        if (text.empty() || text[0] == '-' || !IsValidWord(text)) {
            return nullopt;
        }
        return QueryWord{text, is_minus, IsStopWord(text)};
    }

    struct Query {
        set<string> plus_words;
        set<string> minus_words;
    };

    optional<Query> ParseQuery(const string& text) const {
        // Empty result by initializing it with default constructed Query
        Query query;
        for (const string& word : SplitIntoWords(text)) {
            const auto& query_word = ParseQueryWord(word);
            if (!query_word) {
                return nullopt;
            }
            if (!query_word->is_stop) {
                if (query_word->is_minus) {
                    query.minus_words.insert(query_word->data);
                } else {
                    query.plus_words.insert(query_word->data);
                }
            }
        }
        return query;
    }   

    // Existence required
    double ComputeWordInverseDocumentFreq(const string& word) const;

    template <typename DocumentPredicate>
    vector<Document> FindAllDocuments(const Query& query, DocumentPredicate document_predicate) const {
        map<int, double> document_to_relevance;
        for (const string& word : query.plus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            const double inverse_document_freq = ComputeWordInverseDocumentFreq(word);
            for (const auto [document_id, term_freq] : word_to_document_freqs_.at(word)) {
                const auto& document_data = documents_.at(document_id);
                if (document_predicate(document_id, document_data.status, document_data.rating)) {
                    document_to_relevance[document_id] += term_freq * inverse_document_freq;
                }
            }
        }

        for (const string& word : query.minus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            for (const auto [document_id, _] : word_to_document_freqs_.at(word)) {
                document_to_relevance.erase(document_id);
            }
        }

        vector<Document> matched_documents;
        for (const auto [document_id, relevance] : document_to_relevance) {
            matched_documents.push_back({document_id, relevance, documents_.at(document_id).rating});
        }
        return matched_documents;
    }
};

void PrintDocument(const Document& document);
void PrintMatchDocumentResult(int document_id, const vector<string>& words, DocumentStatus status);
void AddDocument(SearchServer& search_server, int document_id, const string& document, DocumentStatus status, const vector<int>& ratings);
void FindTopDocuments(const SearchServer& search_server, const string& raw_query);
void MatchDocuments(const SearchServer& search_server, const string& query);
void PaginateDocuments(const SearchServer& search_server, const string& query, const int page_size);
