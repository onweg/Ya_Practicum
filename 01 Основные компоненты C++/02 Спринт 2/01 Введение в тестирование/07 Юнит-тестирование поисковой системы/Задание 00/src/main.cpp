#include <algorithm>
#include <cassert>
#include <cmath>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <iostream>

using namespace std;

/* Подставьте вашу реализацию класса SearchServer сюда */

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
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        } else {
            word += c;
        }
    }
    if (!word.empty()) {
        words.push_back(word);
    }

    return words;
}
    
struct Document {
    int id;
    double relevance;
    int rating;
};

enum class DocumentStatus {
    ACTUAL,
    IRRELEVANT,
    BANNED,
    REMOVED,
};

class SearchServer {
public:
    void SetStopWords(const string& text) {
        for (const string& word : SplitIntoWords(text)) {
            stop_words_.insert(word);
        }
    }    
    
    void AddDocument(int document_id, const string& document, DocumentStatus status, const vector<int>& ratings) {
        const vector<string> words = SplitIntoWordsNoStop(document);
        const double inv_word_count = 1.0 / words.size();
        for (const string& word : words) {
            word_to_document_freqs_[word][document_id] += inv_word_count;
        }
        documents_.emplace(document_id, 
            DocumentData{
                ComputeAverageRating(ratings), 
                status
            });
    }
      
    vector<Document> FindTopDocuments(const string& raw_query, DocumentStatus filter_status = DocumentStatus::ACTUAL) const {
        return FindTopDocuments(raw_query, [filter_status](int document_id, DocumentStatus status, int rating){return status == filter_status;});
    }

    template<typename Filter>
    vector<Document> FindTopDocuments(const string& raw_query, Filter filter) const {            
        const Query query = ParseQuery(raw_query);
        auto matched_documents = FindAllDocuments(query);
        
        sort(matched_documents.begin(), matched_documents.end(),
             [](const Document& lhs, const Document& rhs) {
                if (abs(lhs.relevance - rhs.relevance) < 1e-6) {
                    return lhs.rating > rhs.rating;
                } else {
                    return lhs.relevance > rhs.relevance;
                }
             });

        for (int i = 0; i < static_cast<int>(matched_documents.size()); ++i) {
            if (!filter(matched_documents.at(i).id, documents_.at(matched_documents.at(i).id).status, documents_.at(matched_documents.at(i).id).rating)) {
                matched_documents.erase(matched_documents.begin() + i);
                i--;
            }
        }
        if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }
        return matched_documents;
    }

    int GetDocumentCount() const {
        return documents_.size();
    }
    
    tuple<vector<string>, DocumentStatus> MatchDocument(const string& raw_query, int document_id) const {
        vector<string> matched_words;
        if (!documents_.count(document_id)) {
            return make_tuple(matched_words, DocumentStatus::REMOVED);
        }
        const Query query = ParseQuery(raw_query);
        for (const string& word : query.plus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            if (word_to_document_freqs_.at(word).count(document_id)) {
                matched_words.push_back(word);
            }
        }
        for (const string& word : query.minus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            if (word_to_document_freqs_.at(word).count(document_id)) {
                matched_words.clear();
                break;
            }
        }
        return {matched_words, documents_.at(document_id).status};
    }
    
private:
    struct DocumentData {
        int rating;
        DocumentStatus status;
    };

    set<string> stop_words_;
    map<string, map<int, double>> word_to_document_freqs_;
    map<int, DocumentData> documents_;
    
    bool IsStopWord(const string& word) const {
        return stop_words_.count(word) > 0;
    }
    
    vector<string> SplitIntoWordsNoStop(const string& text) const {
        vector<string> words;
        for (const string& word : SplitIntoWords(text)) {
            if (!IsStopWord(word)) {
                words.push_back(word);
            }
        }
        return words;
    }
    
    static int ComputeAverageRating(const vector<int>& ratings) {
        if (ratings.empty()) {
            return 0;
        }
        int rating_sum = 0;
        for (const int rating : ratings) {
            rating_sum += rating;
        }
        return rating_sum / static_cast<int>(ratings.size());
    }
    
    struct QueryWord {
        string data;
        bool is_minus;
        bool is_stop;
    };
    
    QueryWord ParseQueryWord(string text) const {
        bool is_minus = false;
        // Word shouldn't be empty
        if (text[0] == '-') {
            is_minus = true;
            text = text.substr(1);
        }
        return {
            text,
            is_minus,
            IsStopWord(text)
        };
    }
    
    struct Query {
        set<string> plus_words;
        set<string> minus_words;
    };
    
    Query ParseQuery(const string& text) const {
        Query query;
        for (const string& word : SplitIntoWords(text)) {
            const QueryWord query_word = ParseQueryWord(word);
            if (!query_word.is_stop) {
                if (query_word.is_minus) {
                    query.minus_words.insert(query_word.data);
                } else {
                    query.plus_words.insert(query_word.data);
                }
            }
        }
        return query;
    }
    
    // Existence required
    double ComputeWordInverseDocumentFreq(const string& word) const {
        return log(GetDocumentCount() * 1.0 / word_to_document_freqs_.at(word).size());
    }

    vector<Document> FindAllDocuments(const Query& query) const {
        map<int, double> document_to_relevance;
        for (const string& word : query.plus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            const double inverse_document_freq = ComputeWordInverseDocumentFreq(word);
            for (const auto [document_id, term_freq] : word_to_document_freqs_.at(word)) {
                document_to_relevance[document_id] += term_freq * inverse_document_freq;
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
            matched_documents.push_back({
                document_id,
                relevance,
                documents_.at(document_id).rating
            });
        }
        return matched_documents;
    }
};

// -------- Начало модульных тестов поисковой системы ----------

// Тест проверяет, что поисковая система исключает стоп-слова при добавлении документов
void TestExcludeStopWordsFromAddedDocumentContent() {
    const int doc_id = 42;
    const string content = "cat in the city"s;
    const vector<int> ratings = {1, 2, 3};
    // Сначала убеждаемся, что поиск слова, не входящего в список стоп-слов,
    // находит нужный документ
    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        const auto found_docs = server.FindTopDocuments("in"s);
        assert(found_docs.size() == 1);
        const Document& doc0 = found_docs[0];
        assert(doc0.id == doc_id);
    }

    // Затем убеждаемся, что поиск этого же слова, входящего в список стоп-слов,
    // возвращает пустой результат
    {
        SearchServer server;
        server.SetStopWords("in the"s);
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        assert(server.FindTopDocuments("in"s).empty());
    }
    cout << "TestExcludeStopWordsFromAddedDocumentContent SUCCESSFUL"s << endl;
}

/*
Разместите код остальных тестов здесь
*/

//Поддержка минус-слов. Документы, содержащие минус-слова поискового запроса, не должны включаться в результаты поиска.
void TestMinusWords() {
    const int doc_id = 42;
    const string content = "cat in the city"s;
    const vector<int> ratings = {1, 2, 3};

    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        const auto found_docs = server.FindTopDocuments("in"s);
        assert(found_docs.size() == 1);
        const Document& doc0 = found_docs[0];
        assert(doc0.id == doc_id);
    }

    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        assert(server.FindTopDocuments("doc -cat"s).empty());
        assert(!server.FindTopDocuments("cat -cit"s).empty());
    }

    {
        SearchServer server;
        server.SetStopWords("in the"s);
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        assert(server.FindTopDocuments("doc -in"s).empty());
    }

    cout << "TestMinusWords SUCCESFUL"s << endl;
}

//Матчинг документов. При матчинге документа по поисковому запросу должны быть возвращены все слова из поискового запроса, присутствующие в документе. Если есть соответствие хотя бы по одному минус-слову, должен возвращаться пустой список слов.
void TestMatchWords() {
    const int doc_id = 42;
    const string content = "cat in the city"s;
    const vector<int> ratings = {1, 2, 3};

    {   // найти слова в пустом сервере
        SearchServer server;
        const auto [found_words, status] = server.MatchDocument("in the", 100);
        assert(found_words.size() == 0 && status == DocumentStatus::REMOVED);
    }
    {   // найти слова с несуществующем id
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        const auto [found_words, status] = server.MatchDocument("in the", 100);
        assert(found_words.size() == 0 && status == DocumentStatus::REMOVED);
    }
    
    {   // найти слова в существующем id
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        const auto [found_words, status] = server.MatchDocument("in the", 42);
        assert(found_words.size() == 2 && found_words[0] == "in"s && found_words[1] == "the" && status == DocumentStatus::ACTUAL);
    }

    {   // найти слова с запросом с минус словами 
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        const auto [found_words, status] = server.MatchDocument("dog in the city -cat", 42);
        assert(found_words.size() == 0 && status == DocumentStatus::ACTUAL);
    }

    {   // найти слова с запросом с стоп словами
        SearchServer server;
        server.SetStopWords("in the"s);
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        const auto [found_words, status] = server.MatchDocument("white cat in the city"s, 42);
        assert(found_words.size() == 2 && found_words[0] == "cat"s && found_words[1] == "city"s && status == DocumentStatus::ACTUAL);
    }

    cout << "TestMatchWords SUCCESSFUL"s << endl;
}

//Сортировка найденных документов по релевантности. Возвращаемые при поиске документов результаты должны быть отсортированы в порядке убывания релевантности.
void TestSortDocuments() {    
    {   // проверить норм ли сортирует сначала отсортированы по релевантности,
        // потом по рейтенгу, только определенного статуса
        SearchServer server;
        server.SetStopWords("in the and is"s);
        server.AddDocument(1, "cat in the city"s, DocumentStatus::ACTUAL, {1, 2, 3});
        server.AddDocument(2, "my city my love"s, DocumentStatus::ACTUAL, {2, 2, 3});
        server.AddDocument(3, "my cat is brown"s, DocumentStatus::ACTUAL, {5, 5, 5});
        server.AddDocument(4, "cat and dog friend forever"s, DocumentStatus::REMOVED, {1, 2, 3, 3});
        server.AddDocument(5, "cat cat cat"s, DocumentStatus::ACTUAL, {3, 4, 3});
        server.AddDocument(6, "our city our love"s, DocumentStatus::ACTUAL, {5, 5, 5});
        const auto found_docs = server.FindTopDocuments("cat in the city"s);
        assert(found_docs.size() == 5);
        assert(found_docs[0].relevance >= found_docs[1].relevance && found_docs[1].relevance >= found_docs[2].relevance && found_docs[2].relevance >= found_docs[3].relevance && found_docs[3].relevance >= found_docs[4].relevance);
        assert(make_tuple(found_docs[0].id, found_docs[1].id, found_docs[2].id, found_docs[3].id, found_docs[4].id) == make_tuple(1, 5, 6, 2, 3));
    }
    cout << "TestSortDocuments SUCCESSFUL"s << endl;
}

//Вычисление рейтинга документов. Рейтинг добавленного документа равен среднему арифметическому оценок документа.
void TestRating() {
    {
        SearchServer server;
        server.AddDocument(1, "cat in the city"s, DocumentStatus::ACTUAL, {1, 2, 3});
        server.AddDocument(2, "my city my love"s, DocumentStatus::ACTUAL, {-2, 2});
        server.AddDocument(3, "my cat in brown"s, DocumentStatus::ACTUAL, {5, 5, 6});
        const auto found_docs = server.FindTopDocuments("cat in the city");
        assert(found_docs.size() == 3 && found_docs[0].rating == 2 && found_docs[1].rating == 5 && found_docs[2].rating == 0);

    }
    {
        SearchServer server;
        server.AddDocument(1, "cat in the city"s, DocumentStatus::ACTUAL, {0, 0, 0});
        const auto found_docs = server.FindTopDocuments("cat in the city");
        assert(found_docs.size() == 1 && found_docs[0].rating == 0);
    }
    cout << "TestRating SUCCESSFUL" << endl;
}

//Фильтрация результатов поиска с использованием предиката, задаваемого пользователем.
//Поиск документов, имеющих заданный статус.
void TestSortDocumentsWithPredicate() {
    {
         SearchServer server;
        server.SetStopWords("in the and is"s);
        server.AddDocument(1, "cat in the city"s, DocumentStatus::ACTUAL, {1, 2, 3});
        server.AddDocument(2, "my city my love"s, DocumentStatus::ACTUAL, {2, 2, 3});
        server.AddDocument(3, "my cat is brown"s, DocumentStatus::ACTUAL, {5, 5, 5});
        server.AddDocument(4, "cat and dog friend forever"s, DocumentStatus::REMOVED, {1, 2, 3, 3});
        server.AddDocument(5, "cat cat cat"s, DocumentStatus::ACTUAL, {3, 4, 3});
        server.AddDocument(6, "our city our love"s, DocumentStatus::ACTUAL, {5, 5, 5});
        {
            const auto found_docs = server.FindTopDocuments("cat in the city", [](int id, DocumentStatus status, int rating){return id % 2 == 0;});
            assert(found_docs.size() == 3 && make_tuple(found_docs[0].id, found_docs[1].id) == make_tuple(6, 2));
        }
        {
            const auto found_docs = server.FindTopDocuments("cat in the city", DocumentStatus::REMOVED);
            assert(found_docs.size() == 1 && found_docs[0].id == 4);
        }
        {
            const auto found_docs = server.FindTopDocuments("cat brown city", [](int id, DocumentStatus status, int rating){return status == DocumentStatus::ACTUAL && rating > 4;});
            assert(found_docs.size() == 2 && make_tuple(found_docs[0].id, found_docs[1].id) == make_tuple(3, 6));
        }
        cout << "TestSortDocumentsWithPredicate SUCCESSFUL" << endl;
    }
}

//Корректное вычисление релевантности найденных документов.
void TestRelevanceCorrect() {

    auto comparison = [](double a, double b){return abs(a - b) < 1e-6;};

    {
        SearchServer server;
        server.SetStopWords("in the and is"s);
        server.AddDocument(1, "my cat is brown"s, DocumentStatus::ACTUAL, {1, 2, 3});
        server.AddDocument(2, "cat and dog friend"s, DocumentStatus::ACTUAL, {2, 2, 3});
        server.AddDocument(3, "Moscow white city"s, DocumentStatus::ACTUAL, {5, 5, 5});
        server.AddDocument(4, "one cat and two cat"s, DocumentStatus::ACTUAL, {1, 2, 3, 3});
        server.AddDocument(5, "cat city"s, DocumentStatus::ACTUAL, {3, 4, 3});
        const auto found_docs = server.FindTopDocuments("white cat in the city"s);
        assert(found_docs.size() == 5);
        assert(comparison(found_docs[0].relevance, 0.84191));
        assert(comparison(found_docs[1].relevance, 0.569717));
        assert(comparison(found_docs[2].relevance, 0.111572));
        assert(comparison(found_docs[3].relevance, 0.074381));
        assert(comparison(found_docs[4].relevance, 0.074381));
    }
    cout << "TestRelevanceCorrect SUCCESSFUL" << endl;
}

void TestEmptyQuery() {
    
    {
        SearchServer server;
        server.AddDocument(1, "cat in the city"s, DocumentStatus::ACTUAL, {1, 2, 3});
        assert(server.FindTopDocuments(""s).empty());  // Пустой запрос должен возвращать пустой результат
    }
    {
        SearchServer server;
        server.AddDocument(1, "     "s, DocumentStatus::ACTUAL, {1, 2, 3});
        assert(server.FindTopDocuments("cat"s).empty());
    }
    cout << "TestEmptyQuery SUCCESSFUL"s << endl;
}

// Функция TestSearchServer является точкой входа для запуска тестов
void TestSearchServer() {
    TestExcludeStopWordsFromAddedDocumentContent();
    // Не забудьте вызывать остальные тесты здесь
    TestMinusWords();
    TestMatchWords();
    TestSortDocuments();
    TestRating();
    TestSortDocumentsWithPredicate();
    TestRelevanceCorrect();
    TestEmptyQuery();
}

// --------- Окончание модульных тестов поисковой системы -----------

int main() {
    TestSearchServer();
    // Если вы видите эту строку, значит все тесты прошли успешно
    cout << "Search server testing finished"s << endl;
}