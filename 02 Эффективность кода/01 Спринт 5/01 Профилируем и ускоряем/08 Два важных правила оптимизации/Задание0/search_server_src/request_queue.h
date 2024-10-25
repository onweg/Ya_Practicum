#pragma once

#include <string>
#include <queue>
#include "search_server.h"

class RequestQueue{
public:
    RequestQueue(SearchServer& search_server) 
    : time_sec_(0), search_server_(search_server)
    {}
    
    void AddFindRequest(const string& query);

    int GetNoResultRequests() const;

private:
    int time_sec_;
    SearchServer& search_server_;
    std::deque<string> requests_;
};
