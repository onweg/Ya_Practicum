#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

using namespace std;

const int MAX_RESULT_DOCUMENT_COUNT = 5;

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
            if (!IsValidString(word)) {
                throw invalid_argument("Некорректные данные для стоп слов!"s);
            }
        }

    }

    explicit SearchServer(const string& stop_words_text) //Уточнить что это?! Можно ли так и корректно ли так?
    //Походу это конструктор класса через первый конструктор 
        : SearchServer(SplitIntoWords(stop_words_text))
    {}

    static bool IsValidString(const string& word) {
        // Валидное слово не должно иметь специальных символов
        return none_of(word.begin(), word.end(), [](char c) {
            return c >= '\0' && c < ' ';
        });
    }

    static bool CorrectUseMinusSignInText(const string& text) {
        size_t i = 0, size = text.size();
        for (; i < size; i++) {
            if (text[i] == '-') {
                if (i + 1 == size) return false; // если символ последний
                if (text[i + 1] == ' ' or text[i + 1] == '-') return false; // если после символа пробел или еще один минус
            }
        }
        return true;
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
        if (!IsValidString(document)) {
            throw invalid_argument("Некоректный запрос"s);
        }
        const auto words = SplitIntoWordsNoStop(document);
        if (!words) {
            throw invalid_argument("Некоректный запрос");
        }

        const double inv_word_count = 1.0 / words->size();
        for (const string& word : *words) {
            word_to_document_freqs_[word][document_id] += inv_word_count;
        }
        documents_.emplace(document_id, DocumentData{ComputeAverageRating(ratings), status});
        document_ids_.push_back(document_id);
    }

    
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


    vector<Document> FindTopDocuments(const string& raw_query, DocumentStatus status) const {
        return FindTopDocuments(
                raw_query,
                [status](int document_id, DocumentStatus document_status, int rating) {
                    return document_status == status;
                });    }

    vector<Document> FindTopDocuments(const string& raw_query) const {
        return FindTopDocuments(raw_query, DocumentStatus::ACTUAL);
    }

    int GetDocumentCount() const {
        return documents_.size();
    }

    int GetDocumentId(int index) const {
        /*
        Метод GetDocumentId должен выбрасывать исключение out_of_range, если индекс переданного документа выходит за пределы допустимого диапазона (0; количество документов).
        */
        if (index >= 0 && index < GetDocumentCount()) {
            return document_ids_[index];
        }
        throw out_of_range("Некорректный индекс"s);
    }


    tuple<vector<string>, DocumentStatus> MatchDocument(const string& raw_query, int document_id) const {
        /*
        Метод MatchDocument должен выбрасывать исключение invalid_argument в следующих ситуациях:
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
        vector<string> matched_words;
        for (const string& word : query->plus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            if (word_to_document_freqs_.at(word).count(document_id)) {
                matched_words.push_back(word);
            }
        }
        for (const string& word : query->minus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            if (word_to_document_freqs_.at(word).count(document_id)) {
                matched_words.clear();
                break;
            }
        }

        return make_tuple(matched_words, documents_.at(document_id).status);
    }
    
private:
    struct DocumentData {
        int rating;
        DocumentStatus status;
    };

    const set<string> stop_words_; //стоп слова
    map<string, map<int, double>> word_to_document_freqs_; //слово и в каком id документе встречается и сколько весомости имеет
    map<int, DocumentData> documents_; //id документа и какие данные имеет 
    vector<int> document_ids_; //id документа до индексу(id документа по добавлению в этот вектор) (странная тема но пусть)

     bool IsStopWord(const string& word) const {
        return stop_words_.count(word) > 0;
    }

    static bool IsValidWord(const string& word) {
        // A valid word must not contain special characters
        return none_of(word.begin(), word.end(), [](char c) {
            return c >= '\0' && c < ' ';
        });
    }

    optional<vector<string>> SplitIntoWordsNoStop(const string& text) const {
        vector<string> words;
        for (const string& word : SplitIntoWords(text)) {
            if (!IsValidWord(word)) {
                return nullopt;
            }
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
    double ComputeWordInverseDocumentFreq(const string& word) const {
        return log(GetDocumentCount() * 1.0 / word_to_document_freqs_.at(word).size());
    }

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

// ------------ Пример использования ----------------


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

void TestInitSearchServer() {
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

void TestSearchServerAddDocument() {
    try {
        SearchServer server("и в на"s);
        server.AddDocument(0, "кошки мышки"s, DocumentStatus::ACTUAL, {1, 2, 3});
    } catch(const invalid_argument& e) {
        cout << "Ошибка: "s << e.what() << endl;
    }

    try {
        SearchServer server("и в на"s);
        server.AddDocument(0, "кош\x12ки мышки"s, DocumentStatus::ACTUAL, {1, 2, 3});
    } catch(const invalid_argument& e) {
        cout << "Ошибка: "s << e.what() << endl;
    }

    try {
        SearchServer server("и в на"s);
        server.AddDocument(-1, "кошки мышки"s, DocumentStatus::ACTUAL, {1, 2, 3});
    } catch(const invalid_argument& e) {
        cout << "Ошибка: "s << e.what() << endl;
    }

    try {
        SearchServer server("и в на"s);
        server.AddDocument(0, "кошки мышки"s, DocumentStatus::ACTUAL, {1, 2, 3});
        server.AddDocument(0, "кошки мышки"s, DocumentStatus::ACTUAL, {1, 2, 3});
    } catch(const invalid_argument& e) {
        cout << "Ошибка: "s << e.what() << endl;
    }
}

void TestSearchServerFindTopDocuments() {
    try {
        SearchServer server("и в на"s);
        server.AddDocument(0, "a b c"s, DocumentStatus::ACTUAL, {1, 2, 3});
        server.AddDocument(1, "a b "s, DocumentStatus::ACTUAL, {1, 2, 3});
        server.AddDocument(2, "a a"s, DocumentStatus::ACTUAL, {1, 2, 3});
        server.AddDocument(3, "d"s, DocumentStatus::ACTUAL, {1, 2, 3});

        for (const auto& doc : server.FindTopDocuments("a b"s)) {
            PrintDocument(doc);
        }
    } catch(const invalid_argument& e) {
        cout << "Ошибка: "s << e.what() << endl;
    }

    try {
        SearchServer server("и в на"s);
        server.AddDocument(0, "кошки мышки"s, DocumentStatus::ACTUAL, {1, 2, 3});
        for (const auto& doc : server.FindTopDocuments("ко\x12шки"s)) {
            PrintDocument(doc);
        }
    } catch(const invalid_argument& e) {
        cout << "Ошибка: "s << e.what() << endl;
    }

    try {
        SearchServer server("и в на"s);
        server.AddDocument(0, "кошки мышки"s, DocumentStatus::ACTUAL, {1, 2, 3});
        for (const auto& doc : server.FindTopDocuments("кошки- мышки"s)) {
            PrintDocument(doc);
        }
    } catch(const invalid_argument& e) {
        cout << "Ошибка: "s << e.what() << endl;
    }

    try {
        SearchServer server("и в на"s);
        server.AddDocument(0, "кошки мышки"s, DocumentStatus::ACTUAL, {1, 2, 3});
        for (const auto& doc : server.FindTopDocuments("--кошки мышки"s)) {
            PrintDocument(doc);
        }
    } catch(const invalid_argument& e) {
        cout << "Ошибка: "s << e.what() << endl;
    }

    try {
        SearchServer server("и в на"s);
        server.AddDocument(0, "кошки мышки"s, DocumentStatus::ACTUAL, {1, 2, 3});
        for (const auto& doc : server.FindTopDocuments("кошки мышки -"s)) {
            PrintDocument(doc);
        }
    } catch(const invalid_argument& e) {
        cout << "Ошибка: "s << e.what() << endl;
    }

    try {
        SearchServer server("и в на"s);
        server.AddDocument(0, "кошки мышки"s, DocumentStatus::ACTUAL, {1, 2, 3});
        for (const auto& doc : server.FindTopDocuments("кошки иван-чай"s)) {
            PrintDocument(doc);
        }
    } catch(const invalid_argument& e) {
        cout << "Ошибка: "s << e.what() << endl;
    }
}

void TestSearchServerMatchDocument() {
    try {
        SearchServer search_server("и в на"s);
        search_server.AddDocument(0, "a b c"s, DocumentStatus::ACTUAL, {1, 2, 3});
        search_server.AddDocument(1, "a b "s, DocumentStatus::ACTUAL, {1, 2, 3});
        search_server.AddDocument(2, "a a"s, DocumentStatus::ACTUAL, {1, 2, 3});
        search_server.AddDocument(3, "d"s, DocumentStatus::ACTUAL, {1, 2, 3});

        const string query = "b c"s;
        cout << "Матчинг документов по запросу: "s << query << endl;
        const int document_count = search_server.GetDocumentCount();
        for (int index = 0; index < document_count; ++index) {
            const int document_id = search_server.GetDocumentId(index);
            const auto [words, status] = search_server.MatchDocument(query, document_id);
            PrintMatchDocumentResult(document_id, words, status);
        }
    } catch(const invalid_argument& e) {
        cout << "Ошибка: "s << e.what() << endl;
    }

    try {
        SearchServer search_server("и в на"s);
        search_server.AddDocument(0, "кошки мышки"s, DocumentStatus::ACTUAL, {1, 2, 3});
        const string query = "b x\12c"s;
        cout << "Матчинг документов по запросу: "s << query << endl;
        const int document_count = search_server.GetDocumentCount();
        for (int index = 0; index < document_count; ++index) {
            const int document_id = search_server.GetDocumentId(index);
            const auto [words, status] = search_server.MatchDocument(query, document_id);
            PrintMatchDocumentResult(document_id, words, status);
        }
    } catch(const invalid_argument& e) {
        cout << "Ошибка: "s << e.what() << endl;
    }

    try {
        SearchServer search_server("и в на"s);
        search_server.AddDocument(0, "кошки мышки"s, DocumentStatus::ACTUAL, {1, 2, 3});
        const string query = "b- c"s;
        cout << "Матчинг документов по запросу: "s << query << endl;
        const int document_count = search_server.GetDocumentCount();
        for (int index = 0; index < document_count; ++index) {
            const int document_id = search_server.GetDocumentId(index);
            const auto [words, status] = search_server.MatchDocument(query, document_id);
            PrintMatchDocumentResult(document_id, words, status);
        }
    } catch(const invalid_argument& e) {
        cout << "Ошибка: "s << e.what() << endl;
    }

    try {
        SearchServer search_server("и в на"s);
        search_server.AddDocument(0, "кошки мышки"s, DocumentStatus::ACTUAL, {1, 2, 3});
        const string query = "b --c"s;
        cout << "Матчинг документов по запросу: "s << query << endl;
        const int document_count = search_server.GetDocumentCount();
        for (int index = 0; index < document_count; ++index) {
            const int document_id = search_server.GetDocumentId(index);
            const auto [words, status] = search_server.MatchDocument(query, document_id);
            PrintMatchDocumentResult(document_id, words, status);
        }
    } catch(const invalid_argument& e) {
        cout << "Ошибка: "s << e.what() << endl;
    }

    try {
        SearchServer search_server("и в на"s);
        search_server.AddDocument(0, "кошки мышки"s, DocumentStatus::ACTUAL, {1, 2, 3});
        const string query = "b c -"s;
        cout << "Матчинг документов по запросу: "s << query << endl;
        const int document_count = search_server.GetDocumentCount();
        for (int index = 0; index < document_count; ++index) {
            const int document_id = search_server.GetDocumentId(index);
            const auto [words, status] = search_server.MatchDocument(query, document_id);
            PrintMatchDocumentResult(document_id, words, status);
        }
    } catch(const invalid_argument& e) {
        cout << "Ошибка: "s << e.what() << endl;
    }

    try {
        SearchServer search_server("и в на"s);
        search_server.AddDocument(0, "кошки мышки"s, DocumentStatus::ACTUAL, {1, 2, 3});
        const string query = "b иван-чай"s;
        cout << "Матчинг документов по запросу: "s << query << endl;
        const int document_count = search_server.GetDocumentCount();
        for (int index = 0; index < document_count; ++index) {
            const int document_id = search_server.GetDocumentId(index);
            const auto [words, status] = search_server.MatchDocument(query, document_id);
            PrintMatchDocumentResult(document_id, words, status);
        }
    } catch(const invalid_argument& e) {
        cout << "Ошибка: "s << e.what() << endl;
    }
}

void TestSearchServerGetDocumentId() {
    try {
        SearchServer search_server("и в на"s);
        cout << search_server.GetDocumentId(0) << endl;
    } catch(const out_of_range& e) {
        cout << "Ошибка: "s << e.what() << endl;
    }

    try {
        SearchServer search_server("и в на"s);
        search_server.AddDocument(0, "a b"s, DocumentStatus::ACTUAL, {1, 3});
        cout << search_server.GetDocumentId(-1) << endl;
    } catch(const out_of_range& e) {
        cout << "Ошибка: "s << e.what() << endl;
    }

    try {
        SearchServer search_server("и в на"s);
        search_server.AddDocument(0, "a b"s, DocumentStatus::ACTUAL, {1, 3});
        cout << search_server.GetDocumentId(1) << endl;
    } catch(const out_of_range& e) {
        cout << "Ошибка: "s << e.what() << endl;
    }

    try {
        SearchServer search_server("и в на"s);
        search_server.AddDocument(0, "a b"s, DocumentStatus::ACTUAL, {1, 3});
        cout << search_server.GetDocumentId(0) << endl;
    } catch(const out_of_range& e) {
        cout << "Ошибка: "s << e.what() << endl;
    }
}

int main() {    
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
    
    return 0;
}
