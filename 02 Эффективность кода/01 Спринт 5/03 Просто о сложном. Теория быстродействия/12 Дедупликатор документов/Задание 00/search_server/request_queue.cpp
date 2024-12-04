#include "request_queue.h"

void RequestQueue::AddFindRequest(const std::string& query) {
    if (time_sec_ == 1440) {
        requests_.push_back(query);
        requests_.pop_front();
    } else {
        ++time_sec_;
        requests_.push_back(query);
    }
}

int RequestQueue::GetNoResultRequests() const {
    int count = 0;
    for (auto it = requests_.begin(); it != requests_.end(); ++it) {
        if (search_server_.FindTopDocuments(*it).empty()) {
            ++count;
        }
    }
    return count;
}