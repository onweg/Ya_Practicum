#include "search_server.h"
#include "paginator.h"
#include "log_duration.h"


bool SearchServer::IsValidString(const string& word) {
    // Валидное слово не должно иметь специальных символов
    return none_of(word.begin(), word.end(), [](char c) {
        return c >= '\0' && c < ' ';
    });
}

bool SearchServer::CorrectUseMinusSignInText(const string& text) {
    size_t i = 0, size = text.size();
    for (; i < size; i++) {
        if (text[i] == '-') {
            if (i + 1 == size) return false; // если символ последний
            if (text[i + 1] == ' ' or text[i + 1] == '-') return false; // если после символа пробел или еще один минус
        }
    }
    return true;
}

void SearchServer::AddDocument(int document_id, const string& document, DocumentStatus status, const vector<int>& ratings) {
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

vector<Document> SearchServer::FindTopDocuments(const string& raw_query, DocumentStatus status) const {
    return FindTopDocuments(
            raw_query,
            [status](int document_id, DocumentStatus document_status, int rating) {
                return document_status == status;
            });
}

vector<Document> SearchServer::FindTopDocuments(const string& raw_query) const {
    return FindTopDocuments(raw_query, DocumentStatus::ACTUAL);
}

int SearchServer::GetDocumentCount() const {
    return documents_.size();
}

int SearchServer::GetDocumentId(int index) const {
    /*
    Метод GetDocumentId должен выбрасывать исключение out_of_range, если индекс переданного документа выходит за пределы допустимого диапазона (0; количество документов).
    */
    if (index >= 0 && index < GetDocumentCount()) {
        return document_ids_[index];
    }
    throw out_of_range("Некорректный индекс"s);
}

tuple<vector<string>, DocumentStatus> SearchServer::MatchDocument(const string& raw_query, int document_id) const {
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

bool SearchServer::IsStopWord(const string& word) const {
    return stop_words_.count(word) > 0;
}

bool SearchServer::IsValidWord(const string& word) {
    // A valid word must not contain special characters
    return none_of(word.begin(), word.end(), [](char c) {
        return c >= '\0' && c < ' ';
    });
}

optional<vector<string>> SearchServer::SplitIntoWordsNoStop(const string& text) const {
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

int SearchServer::ComputeAverageRating(const vector<int>& ratings) {
    if (ratings.empty()) {
        return 0;
    }
    int rating_sum = 0;
    for (const int rating : ratings) {
        rating_sum += rating;
    }
    return rating_sum / static_cast<int>(ratings.size());
}

// Existence required
double SearchServer::ComputeWordInverseDocumentFreq(const string& word) const {
    return log(GetDocumentCount() * 1.0 / word_to_document_freqs_.at(word).size());
}




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
    } catch (const invalid_argument& e) {
        cout << "Ошибка добавления документа "s << document_id << ": "s << e.what() << endl;
    }
}

void FindTopDocuments(const SearchServer& search_server, const string& raw_query) {
    cout << "Результаты поиска по запросу: "s << raw_query << endl;
    LOG_DURATION("Operation time"s);
    try {
        for (const Document& document : search_server.FindTopDocuments(raw_query)) {
            PrintDocument(document);
        }
    } catch (const invalid_argument& e) {
        cout << "Ошибка поиска: "s << e.what() << endl;
    }
}

void MatchDocuments(const SearchServer& search_server, const string& query) {
    try {
        LOG_DURATION("Operation time"s);
        cout << "Матчинг документов по запросу: "s << query << endl;
        const int document_count = search_server.GetDocumentCount();
        for (int index = 0; index < document_count; ++index) {
            const int document_id = search_server.GetDocumentId(index);
            const auto [words, status] = search_server.MatchDocument(query, document_id);
            PrintMatchDocumentResult(document_id, words, status);
        }
    } catch (const invalid_argument& e) {
        cout << "Ошибка матчинга документов на запрос "s << query << ": "s << e.what() << endl;
    }
}



void PaginateDocuments(const SearchServer& search_server, const string& query, const int page_size) {
    cout << "Результаты поиска по запросу : "s << query <<  " с разбиением на страницы по "s << page_size << " документам"s << endl;
    try {
        const auto pages = Paginate(search_server.FindTopDocuments(query), page_size);
        for (auto page = pages.begin(); page != pages.end(); ++page) {
            cout << *page << endl;
            cout << "Page break"s << endl;
        }
    } catch (const invalid_argument& e) {
        cout << "Ошибка поиска: "s << e.what() << endl;
    }
}


        