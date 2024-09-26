#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <set>

using namespace std;

void PrintStringToChar(const string& s) {
    size_t size = s.size();
    for (size_t i = 0; i < size; i++) {
        cout << "\"" << s[i];
    }
    cout << "\"";
}

enum class QueryType {
    NewBus,
    BusesForStop,
    StopsForBus,
    AllBuses,
};

struct Query {
    QueryType type;
    string bus;
    string stop;
    vector<string> stops;
};

istream& operator>>(istream& is, Query& q) {
    // Реализуйте эту функцию
    string s;
    is >> s;
    if (s == "NEW_BUS") {
        q.type = QueryType::NewBus;
        is >> q.bus;
        int stop_count;
        is >> stop_count;
        vector<string> &stops = q.stops;
        stops.resize(stop_count);
        set<string> set_stops;
        string stop;
        int id = 0;
        for (int i = 0; i < stop_count; i++) {
            is >> stop;
            if (set_stops.find(stop) == set_stops.end()) stops[id++] = stop;
            set_stops.insert(stop);
        }
        stops.resize(id);
        q.stop = ""s;
    } else if (s == "BUSES_FOR_STOP") {
        q.type = QueryType::BusesForStop;
        is >> q.stop;

    } else if (s == "STOPS_FOR_BUS") {
        q.type = QueryType::StopsForBus;
        is >> q.bus;

    } else if (s == "ALL_BUSES") {
        q.type = QueryType::AllBuses;
    }
    return is;
}

struct BusesForStopResponse {
    // Наполните полями эту структуру
    string stop;
    map<string, vector<string> > stops_to_buses;
};

ostream& operator<<(ostream& os, const BusesForStopResponse& r) {
    // Реализуйте эту функцию]
    if (r.stops_to_buses.count(r.stop)) {
        const vector<string>& buses = r.stops_to_buses.at(r.stop);
        size_t size = buses.size();
        for (size_t i = 0; i < size; ++i) {
            os << buses[i];
            if (i + 1 < size) os << " "s;
        }
    } else {
        os << "No stop"s;
    }
    os << endl;
    return os;
}

struct StopsForBusResponse {
    // Наполните полями эту структуру
    string bus;
    map<string, vector<string> > buses_to_stops;
    map<string, vector<string> > stops_to_buses;
};

ostream& operator<<(ostream& os, const StopsForBusResponse& r) {
    // Реализуйте эту функцию
    if (r.buses_to_stops.count(r.bus)) {
        for (const auto& stop : r.buses_to_stops.at(r.bus)) {
            os << "Stop "s << stop << ":"s;
            if (r.stops_to_buses.at(stop).size() == 1) {
                os << " no interchange"s;
            } else {
                const vector<string>& buses = r.stops_to_buses.at(stop);
                size_t size = buses.size();
                for (size_t i = 0; i < size; ++i) {
                    if (buses[i] != r.bus) {
                        os << " "s <<buses[i];
                    }
                }
            }
            os << endl;
        }
    } else {
        os << "No bus"s << endl;
    }
    return os;
}

struct AllBusesResponse {
    // Наполните полями эту структуру
    map<string, vector<string> > buses_to_stops;
};

ostream& operator<<(ostream& os, const AllBusesResponse& r) {
    // Реализуйте эту функцию
    if (!r.buses_to_stops.empty()) {
        vector<pair<string, vector<string> > > sort_all_buses;
        for (const auto& [bus, stops] : r.buses_to_stops) {
            sort_all_buses.push_back(make_pair(bus, stops));
        }
        sort(sort_all_buses.begin(), sort_all_buses.end());
        for (const auto& [bus, stops] : sort_all_buses) {
            os << "Bus "s << bus << ":"s;
            size_t size = stops.size();
            for (size_t i = 0; i < size; ++i) {
                os << " "s << stops[i];
            }
            os << endl;
        }
    } else {
        os << "No buses"s << endl;
    }
    return os;
}

class BusManager {
public:
    void AddBus(const string& bus, const vector<string>& stops) {
        // Реализуйте этот метод
        buses_to_stops[bus] = stops;
        for (const auto& stop : stops) {
            stops_to_buses[stop].push_back(bus);
        }
    }

    BusesForStopResponse GetBusesForStop(const string& stop) const {
        // Реализуйте этот метод
        BusesForStopResponse result = {stop, stops_to_buses};
        return result;

    }

    StopsForBusResponse GetStopsForBus(const string& bus) const {
        // Реализуйте этот метод
        StopsForBusResponse result = {bus, buses_to_stops, stops_to_buses};
        return result;
    }

    AllBusesResponse GetAllBuses() const {
        // Реализуйте этот метод
        AllBusesResponse result = {buses_to_stops};
        return result;
    }

    auto GetBusesToStops(){
        return buses_to_stops;
    }

    auto GetStopsToBuses(){
        return stops_to_buses;
    }

private:
    map<string, vector<string> > buses_to_stops;
    map<string, vector<string> > stops_to_buses;
};

void TestAddingBuses() {
    BusManager bm;
    bm.AddBus("bus1", {"stop1"});
    assert(bm.GetBusesToStops().size() == 1 && bm.GetBusesToStops().count("bus1") == 1 && bm.GetBusesToStops().at("bus1").size() == 1 && bm.GetBusesToStops().at("bus1")[0] == "stop1");
    assert(bm.GetStopsToBuses().size() == 1 && bm.GetStopsToBuses().count("stop1") == 1 && bm.GetStopsToBuses().at("stop1").size() == 1 && bm.GetStopsToBuses().at("stop1")[0] == "bus1");

    bm.AddBus("bus2", {"stop2", "stop3", "stop4"});
    assert(bm.GetBusesToStops().size() == 2 && bm.GetBusesToStops().count("bus1") == 1 && bm.GetBusesToStops().at("bus1").size() == 1 && bm.GetBusesToStops().at("bus1")[0] == "stop1");
    assert(bm.GetStopsToBuses().size() == 4 && bm.GetStopsToBuses().count("stop1") == 1 && bm.GetStopsToBuses().at("stop1").size() == 1 && bm.GetStopsToBuses().at("stop1")[0] == "bus1");

    assert(bm.GetBusesToStops().size() == 2 && bm.GetBusesToStops().count("bus2") == 1 && bm.GetBusesToStops().at("bus2").size() == 3 && bm.GetBusesToStops().at("bus2")[0] == "stop2" && bm.GetBusesToStops().at("bus2")[1]== "stop3" && bm.GetBusesToStops().at("bus2")[2] == "stop4");
    assert(bm.GetStopsToBuses().size() == 4 && bm.GetStopsToBuses().count("stop2") == 1 && bm.GetStopsToBuses().at("stop2").size() == 1 && bm.GetStopsToBuses().at("stop2")[0] == "bus2");
    assert(bm.GetStopsToBuses().size() == 4 && bm.GetStopsToBuses().count("stop3") == 1 && bm.GetStopsToBuses().at("stop3").size() == 1 && bm.GetStopsToBuses().at("stop3")[0] == "bus2");
    assert(bm.GetStopsToBuses().size() == 4 && bm.GetStopsToBuses().count("stop4") == 1 && bm.GetStopsToBuses().at("stop4").size() == 1 && bm.GetStopsToBuses().at("stop4")[0] == "bus2");

    bm.AddBus("bus3", {"stop1", "stop2"});
    assert(bm.GetBusesToStops().size() == 3 && bm.GetBusesToStops().count("bus1") == 1 && bm.GetBusesToStops().at("bus1").size() == 1 && bm.GetBusesToStops().at("bus1")[0] == "stop1");
    assert(bm.GetStopsToBuses().size() == 4 && bm.GetStopsToBuses().count("stop1") == 1 && bm.GetStopsToBuses().at("stop1").size() == 2 && bm.GetStopsToBuses().at("stop1")[0] == "bus1" && bm.GetStopsToBuses().at("stop1")[1] == "bus3");

    assert(bm.GetBusesToStops().size() == 3 && bm.GetBusesToStops().count("bus2") == 1 && bm.GetBusesToStops().at("bus2").size() == 3 && bm.GetBusesToStops().at("bus2")[0] == "stop2" && bm.GetBusesToStops().at("bus2")[1]== "stop3" && bm.GetBusesToStops().at("bus2")[2] == "stop4");
    assert(bm.GetStopsToBuses().size() == 4 && bm.GetStopsToBuses().count("stop2") == 1 && bm.GetStopsToBuses().at("stop2").size() == 2 && bm.GetStopsToBuses().at("stop2")[0] == "bus2" && bm.GetStopsToBuses().at("stop2")[1] == "bus3");
    assert(bm.GetStopsToBuses().size() == 4 && bm.GetStopsToBuses().count("stop3") == 1 && bm.GetStopsToBuses().at("stop3").size() == 1 && bm.GetStopsToBuses().at("stop3")[0] == "bus2");
    assert(bm.GetStopsToBuses().size() == 4 && bm.GetStopsToBuses().count("stop4") == 1 && bm.GetStopsToBuses().at("stop4").size() == 1 && bm.GetStopsToBuses().at("stop4")[0] == "bus2");

    assert(bm.GetBusesToStops().size() == 3 && bm.GetBusesToStops().count("bus3") == 1 && bm.GetBusesToStops().at("bus3").size() == 2 && bm.GetBusesToStops().at("bus3")[0] == "stop1" && bm.GetBusesToStops().at("bus3")[1] == "stop2");


    bm.AddBus("bus4", {"stop1", "stop2", "stop3", "stop4", "stop5"});
    assert(bm.GetBusesToStops().size() == 4 && bm.GetBusesToStops().count("bus1") == 1 && bm.GetBusesToStops().at("bus1").size() == 1 && bm.GetBusesToStops().at("bus1")[0] == "stop1");
    assert(bm.GetStopsToBuses().size() == 5 && bm.GetStopsToBuses().count("stop1") == 1 && bm.GetStopsToBuses().at("stop1").size() == 3 && bm.GetStopsToBuses().at("stop1")[0] == "bus1" && bm.GetStopsToBuses().at("stop1")[1] == "bus3" && bm.GetStopsToBuses().at("stop1")[2] == "bus4");

    assert(bm.GetBusesToStops().size() == 4 && bm.GetBusesToStops().count("bus2") == 1 && bm.GetBusesToStops().at("bus2").size() == 3 && bm.GetBusesToStops().at("bus2")[0] == "stop2" && bm.GetBusesToStops().at("bus2")[1]== "stop3" && bm.GetBusesToStops().at("bus2")[2] == "stop4");
    assert(bm.GetStopsToBuses().size() == 5 && bm.GetStopsToBuses().count("stop2") == 1 && bm.GetStopsToBuses().at("stop2").size() == 3 && bm.GetStopsToBuses().at("stop2")[0] == "bus2" && bm.GetStopsToBuses().at("stop2")[1] == "bus3" && bm.GetStopsToBuses().at("stop2")[2] == "bus4");
    assert(bm.GetStopsToBuses().size() == 5 && bm.GetStopsToBuses().count("stop3") == 1 && bm.GetStopsToBuses().at("stop3").size() == 2 && bm.GetStopsToBuses().at("stop3")[0] == "bus2" && bm.GetStopsToBuses().at("stop3")[1] == "bus4");
    assert(bm.GetStopsToBuses().size() == 5 && bm.GetStopsToBuses().count("stop4") == 1 && bm.GetStopsToBuses().at("stop4").size() == 2 && bm.GetStopsToBuses().at("stop4")[0] == "bus2" && bm.GetStopsToBuses().at("stop4")[1] == "bus4");

    assert(bm.GetBusesToStops().size() == 4 && bm.GetBusesToStops().count("bus3") == 1 && bm.GetBusesToStops().at("bus3").size() == 2 && bm.GetBusesToStops().at("bus3")[0] == "stop1" && bm.GetBusesToStops().at("bus3")[1] == "stop2");

    assert(bm.GetBusesToStops().size() == 4 && bm.GetBusesToStops().count("bus4") == 1 && bm.GetBusesToStops().at("bus4").size() == 5 && bm.GetBusesToStops().at("bus4")[0] == "stop1" && bm.GetBusesToStops().at("bus4")[1] == "stop2" && bm.GetBusesToStops().at("bus4")[2] == "stop3" && bm.GetBusesToStops().at("bus4")[3] == "stop4" && bm.GetBusesToStops().at("bus4")[4] == "stop5");
    cout << "TestAddingBuses SUCCESSFUL" << endl;
}

void TestQueryInput() {
    istringstream input;
    Query q;

    input.str("NEW_BUS bus 1 stop"s);
    input >> q;
    assert(q.type == QueryType::NewBus && q.bus == "bus"s && q.stops.size() == 1 && q.stops[0] == "stop"s && q.stop == ""s);

    input.clear();
    input.str("NEW_BUS bus_b2 3 stop1 stop2 stop3"s);
    input >> q;
    assert(q.type == QueryType::NewBus && q.bus == "bus_b2"s && q.stops.size() == 3 && q.stops[0] == "stop1"s && q.stops[1] == "stop2"s && q.stops[2] == "stop3"s && q.stop == ""s);

    //название маршрута совпадает с наpванием остановки
    input.clear();
    input.str("NEW_BUS stop1 2 stop1 stop2"s);
    input >> q;
    assert(q.type == QueryType::NewBus && q.bus == "stop1"s && q.stops.size() == 2 && q.stops[0] == "stop1"s && q.stops[1] == "stop2"s && q.stop == ""s);

    //название остановки повторяется несоклько раз
    input.clear();
    input.str("NEW_BUS bus 3 stop1 stop1 stop2"s);
    input >> q;
    assert(q.type == QueryType::NewBus && q.bus == "bus"s && q.stops.size() == 2 && q.stops[0] == "stop1"s && q.stops[1] == "stop2"s && q.stop == ""s);

    input.clear();
    input.str("NEW_BUS bus 3 stop1 stop2 stop1"s);
    input >> q;
    assert(q.type == QueryType::NewBus && q.bus == "bus"s && q.stops.size() == 2 && q.stops[0] == "stop1"s && q.stops[1] == "stop2"s && q.stop == ""s);

    input.clear();
    input.str("NEW_BUS bus 3 stop2 stop1 stop1"s);
    input >> q;
    assert(q.type == QueryType::NewBus && q.bus == "bus"s && q.stops.size() == 2 && q.stops[0] == "stop2"s && q.stops[1] == "stop1"s && q.stop == ""s);

    input.clear();
    input.str("NEW_BUS bus 5 stop1 stop1 stop2 stop1 stop2"s);
    input >> q;
    assert(q.type == QueryType::NewBus && q.bus == "bus"s && q.stops.size() == 2 && q.stops[0] == "stop1"s && q.stops[1] == "stop2"s && q.stop == ""s);

    input.clear();
    input.str("NEW_BUS bus 5 stop1 stop1 stop1 stop2 stop2"s);
    input >> q;
    assert(q.type == QueryType::NewBus && q.bus == "bus"s && q.stops.size() == 2 && q.stops[0] == "stop1"s && q.stops[1] == "stop2"s && q.stop == ""s);
    
    cout << "QueryInput SUCCESSFUL" << endl;
}

void TestOutput() {
    BusManager bm;
    ostringstream output;

    output << bm.GetStopsForBus("bus");
    assert(output.str() == "No bus\n");
    output.str("");
    output << bm.GetBusesForStop("stop");
    assert(output.str() == "No stop\n");
    output.str("");
    output << bm.GetAllBuses();
    assert(output.str() == "No buses\n");

    bm.AddBus("bus1",{"stop1"});

    output.str("");
    output << bm.GetStopsForBus("bus2");
    assert(output.str() == "No bus\n");
    output.str("");
    output << bm.GetBusesForStop("stop2");
    assert(output.str() == "No stop\n");
    output.str("");
    output << bm.GetAllBuses();
    assert(output.str() == "Bus bus1: stop1\n");

    output.str("");
    output << bm.GetStopsForBus("bus1");
    assert(output.str() == "Stop stop1: no interchange\n");

    output.str("");
    output << bm.GetBusesForStop("stop1");
    assert(output.str() == "bus1\n");

    output.str("");
    output << bm.GetAllBuses();
    assert(output.str() == "Bus bus1: stop1\n");

    bm.AddBus("bus3", {"stop1", "stop2"});

    output.str("");
    output << bm.GetStopsForBus("bus1");
    assert(output.str() == "Stop stop1: bus3\n");
    output.str("");
    output << bm.GetStopsForBus("bus3");
    assert(output.str() == "Stop stop1: bus1\nStop stop2: no interchange\n");
    
    output.str("");
    output << bm.GetBusesForStop("stop1");
    assert(output.str() == "bus1 bus3\n");
    output.str("");
    output << bm.GetAllBuses();
    assert(output.str() == "Bus bus1: stop1\nBus bus3: stop1 stop2\n");

    bm.AddBus("bus2", {"stop3", "stop2"});

    output.str("");
    output << bm.GetBusesForStop("stop2");
    assert(output.str() == "bus3 bus2\n");
    output.str("");
    output << bm.GetStopsForBus("bus2");
    assert(output.str() == "Stop stop3: no interchange\nStop stop2: bus3\n");
    output.str("");
    output << bm.GetAllBuses();
    assert(output.str() == "Bus bus1: stop1\nBus bus2: stop3 stop2\nBus bus3: stop1 stop2\n");

    bm.AddBus("bus4", {"stop3", "stop2", "stop4", "stop1"});
    output.str("");
    output <<  bm.GetStopsForBus("bus4");
    assert(output.str() == "Stop stop3: bus2\nStop stop2: bus3 bus2\nStop stop4: no interchange\nStop stop1: bus1 bus3\n");
    output.str("");
    output << bm.GetAllBuses();
    assert(output.str() == "Bus bus1: stop1\nBus bus2: stop3 stop2\nBus bus3: stop1 stop2\nBus bus4: stop3 stop2 stop4 stop1\n");

    cout << "TestOutput SUCCESSFUL" << endl;
}

void TestTaskBuses() {
    TestQueryInput();
    TestAddingBuses();
    TestOutput();
    cout << "Class is correct!"s << endl;
}

// Не меняя тела функции main, реализуйте функции и классы выше

int main() {

    TestTaskBuses();

    int query_count;
    Query q;

    cin >> query_count;

    BusManager bm;
    for (int i = 0; i < query_count; ++i) {
        cin >> q;
        switch (q.type) {
            case QueryType::NewBus:
                bm.AddBus(q.bus, q.stops);
                break;
            case QueryType::BusesForStop:
                cout << bm.GetBusesForStop(q.stop) << endl;
                break;
            case QueryType::StopsForBus:
                cout << bm.GetStopsForBus(q.bus) << endl;
                break;
            case QueryType::AllBuses:
                cout << bm.GetAllBuses() << endl;
                break;
        }
    }
}
