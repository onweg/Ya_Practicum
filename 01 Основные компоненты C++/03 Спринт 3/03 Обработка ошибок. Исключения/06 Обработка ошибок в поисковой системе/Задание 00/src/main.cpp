#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

using namespace std;

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
    Document() = default;

    Document(int id, double relevance, int rating)
            : id(id)
            , relevance(relevance)
            , rating(rating) {
    }

    int id = 0;
    double relevance = 0.0;
    int rating = 0;
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

enum class DocumentStatus {
    ACTUAL,
    IRRELEVANT,
    BANNED,
    REMOVED,
};

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
            if (!IsValidWord(word)) {
                throw invalid_argument("Некорректные данные для стоп слов!"s);
            }
        }

    }

    explicit SearchServer(const string& stop_words_text) //Уточнить что это?! Можно ли так и корректно ли так?
    //Походу это конструктор класса через первый конструктор 
        : SearchServer(SplitIntoWords(stop_words_text))
    {}

    static bool IsValidWord(const string& word) {
        // Валидное слово не должно иметь специальных символов
        return none_of(word.begin(), word.end(), [](char c) {
            return c >= '\0' && c < ' ';
        });
    }

    
    void AddDocument(int document_id, const string& document, DocumentStatus status, const vector<int>& ratings) {
        /*
        Метод должен выбрасывать исключение invalid_argument в следующих ситуациях:
        Попытка добавить документ с отрицательным id;
        Попытка добавить документ c id ранее добавленного документа;
        Наличие недопустимых символов (с кодами от 0 до 31) в тексте добавляемого документа.
        */
        if ((document_id < 0) || (documents_.count(document_id) > 0)) {
            throw invalid_argument("Некорректный id"s);
        }
    }

    /*
    template <typename DocumentPredicate>
    vector<Document> FindTopDocuments(const string& raw_query, DocumentPredicate document_predicate) const {
        ...
    }

    vector<Document> FindTopDocuments(const string& raw_query, DocumentStatus status) const {
        ...
    }

    vector<Document> FindTopDocuments(const string& raw_query) const {
        ...
    }

    int GetDocumentCount() const {
        ...
    }

    int GetDocumentId(int index) const {
        ...
    }

    tuple<vector<string>, DocumentStatus> MatchDocument(const string& raw_query, int document_id) const {
        ...
    }
    */
private:
    struct DocumentData {
        int rating;
        DocumentStatus status;
    };

    const set<string> stop_words_; //стоп слова
    map<string, map<int, double>> word_to_document_freqs_; //слово и в каком id документе встречается и сколько весомости имеет
    map<int, DocumentData> documents_; //id документа и какие данные имеет 
    vector<int> document_ids_; //id документа до индексу(id документа по добавлению в этот вектор) (странная тема но пусть)


};

// ------------ Пример использования ----------------

/*
void PrintDocument(const Document& document) {
    cout << "{ "s
         << "document_id = "s << document.id << ", "s
         << "relevance = "s << document.relevance << ", "s
         << "rating = "s << document.rating << " }"s << endl;
}

void PrintMatchDocumentResult(int document_id, const vector<string>& words, DocumentStatus status) {
    cout << "{ "s
         << "document_id = "s << document_id << ", "s
         << "status = "s << static_cast<int>(status) << ", "s
         << "words ="s;
    for (const string& word : words) {
        cout << ' ' << word;
    }
    cout << "}"s << endl;
}

void AddDocument(SearchServer& search_server, int document_id, const string& document, DocumentStatus status,
                 const vector<int>& ratings) {
    try {
        search_server.AddDocument(document_id, document, status, ratings);
    } catch (const exception& e) {
        cout << "Ошибка добавления документа "s << document_id << ": "s << e.what() << endl;
    }
}

void FindTopDocuments(const SearchServer& search_server, const string& raw_query) {
    cout << "Результаты поиска по запросу: "s << raw_query << endl;
    try {
        for (const Document& document : search_server.FindTopDocuments(raw_query)) {
            PrintDocument(document);
        }
    } catch (const exception& e) {
        cout << "Ошибка поиска: "s << e.what() << endl;
    }
}

void MatchDocuments(const SearchServer& search_server, const string& query) {
    try {
        cout << "Матчинг документов по запросу: "s << query << endl;
        const int document_count = search_server.GetDocumentCount();
        for (int index = 0; index < document_count; ++index) {
            const int document_id = search_server.GetDocumentId(index);
            const auto [words, status] = search_server.MatchDocument(query, document_id);
            PrintMatchDocumentResult(document_id, words, status);
        }
    } catch (const exception& e) {
        cout << "Ошибка матчинга документов на запрос "s << query << ": "s << e.what() << endl;
    }
}

*/
void TestSearchServer() {
    try {
        SearchServer server("и в на"s);
    } catch(const invalid_argument& e) {
        cout << "Ошибка: "s << e.what() << endl;
    }

    try {
        SearchServer server("и в н\x12а"s);
    } catch(const invalid_argument& e) {
        cout << "Ошибка: "s << e.what() << endl;
    }
}

int main() {
    TestSearchServer();

    /*
    SearchServer search_server("и в на"s);
    AddDocument(search_server, 1, "пушистый кот пушистый хвост"s, DocumentStatus::ACTUAL, {7, 2, 7});
    AddDocument(search_server, 1, "пушистый пёс и модный ошейник"s, DocumentStatus::ACTUAL, {1, 2});
    AddDocument(search_server, -1, "пушистый пёс и модный ошейник"s, DocumentStatus::ACTUAL, {1, 2});
    AddDocument(search_server, 3, "большой пёс скво\x12рец евгений"s, DocumentStatus::ACTUAL, {1, 3, 2});
    AddDocument(search_server, 4, "большой пёс скворец евгений"s, DocumentStatus::ACTUAL, {1, 1, 1});

    FindTopDocuments(search_server, "пушистый -пёс"s);
    FindTopDocuments(search_server, "пушистый --кот"s);
    FindTopDocuments(search_server, "пушистый -"s);

    MatchDocuments(search_server, "пушистый пёс"s);
    MatchDocuments(search_server, "модный -кот"s);
    MatchDocuments(search_server, "модный --пёс"s);
    MatchDocuments(search_server, "пушистый - хвост"s);
    */
    return 0;
}