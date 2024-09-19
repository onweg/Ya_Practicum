#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <set>

using namespace std;

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
    // Реализуйте эту функцию
    if (r.stops_to_buses.count(r.stop)) {
        for (const auto& i : r.stops_to_buses.at(r.stop)) {
            os << i << ' ';
        }
    } else {
        os << "No stop"s;
    }
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
            os << "Stop "s << stop << ": "s;
            if (r.stops_to_buses.at(stop).size() == 1) {
                os << "no interchange"s;
            } else {
                for (const auto& bus : r.stops_to_buses.at(stop)) {
                    if (bus != r.bus) {
                        os << bus << ' ';
                    }
                }
            }
            os << endl;
        }
    } else {
        os << "No bus"s;
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
            os << "Bus "s << bus << ": "s;
            for (const auto& stop : stops) {
                os << stop << ' ';
            }
            os << endl;
        }
    } else {
        os << "No buses"s;
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
    bm.NEW_BUS("bus1", 1, "stop1");
    assert(bm.GetBusesToStops.size() == 1 && bm.GetBusesToStops.count("bus1") == 1 && bm.GetBusesToStops.at("bus1").size() == 1 && bm.GetBusesToStops.at("bus1")[0] == "stop1");
    assert(bm.GetStopsToBuses.size() == 1 && bm.GetStopsToBuses.count("stop1") == 1 && bm.GetStopsToBuses.at("stop1").size() == 1 && bm.GetStopsToBuses.at("stop1")[0] == "bus1");

    bm.NEW_BUS("bus2", 3, "stop2", "stop3", "stop4");
    assert(bm.GetBusesToStops.size() == 2 && bm.GetBusesToStops.count("bus1") == 1 && bm.GetBusesToStops.at("bus1").size() == 1 && bm.GetBusesToStops.at("bus1")[0] == "stop1");
    assert(bm.GetStopsToBuses.size() == 4 && bm.GetStopsToBuses.count("stop1") == 1 && bm.GetStopsToBuses.at("stop1").size() == 1 && bm.GetStopsToBuses.at("stop1")[0] == "bus1");

    assert(bm.GetBusesToStops.size() == 2 && bm.GetBusesToStops.count("bus2") == 1 && bm.GetBusesToStops.at("bus2").size() == 3 && bm.GetBusesToStops.at("bus2")[0] == "stop2" && bm.GetBusesToStops.at("bus2")[1]== "stop3" && bm.GetBusesToStops.at("bus2")[2] == "stop4");
    assert(bm.GetStopsToBuses.size() == 4 && bm.GetStopsToBuses.count("stop2") == 1 && bm.GetStopsToBuses.at("stop2").size() == 1 && bm.GetStopsToBuses.at("stop2")[0] == "bus2");
    assert(bm.GetStopsToBuses.size() == 4 && bm.GetStopsToBuses.count("stop3") == 1 && bm.GetStopsToBuses.at("stop3").size() == 1 && bm.GetStopsToBuses.at("stop3")[0] == "bus2");
    assert(bm.GetStopsToBuses.size() == 4 && bm.GetStopsToBuses.count("stop4") == 1 && bm.GetStopsToBuses.at("stop4").size() == 1 && bm.GetStopsToBuses.at("stop4")[0] == "bus2");

    bm.NEW_BUS("bus3", 2, "stop1", "stop2");
    assert(bm.GetBusesToStops.size() == 3 && bm.GetBusesToStops.count("bus1") == 1 && bm.GetBusesToStops.at("bus1").size() == 1 && bm.GetBusesToStops.at("bus1")[0] == "stop1");
    assert(bm.GetStopsToBuses.size() == 4 && bm.GetStopsToBuses.count("stop1") == 1 && bm.GetStopsToBuses.at("stop1").size() == 2 && bm.GetStopsToBuses.at("stop1")[0] == "bus1" && bm.GetStopsToBuses.at("stop1")[1] == "bus3");

    assert(bm.GetBusesToStops.size() == 3 && bm.GetBusesToStops.count("bus2") == 1 && bm.GetBusesToStops.at("bus2").size() == 3 && bm.GetBusesToStops.at("bus2")[0] == "stop2" && bm.GetBusesToStops.at("bus2")[1]== "stop3" && bm.GetBusesToStops.at("bus2")[2] == "stop4");
    assert(bm.GetStopsToBuses.size() == 4 && bm.GetStopsToBuses.count("stop2") == 1 && bm.GetStopsToBuses.at("stop2").size() == 2 && bm.GetStopsToBuses.at("stop2")[0] == "bus2" && bm.GetStopsToBuses.at("stop2")[1] == "bus3");
    assert(bm.GetStopsToBuses.size() == 4 && bm.GetStopsToBuses.count("stop3") == 1 && bm.GetStopsToBuses.at("stop3").size() == 1 && bm.GetStopsToBuses.at("stop3")[0] == "bus2");
    assert(bm.GetStopsToBuses.size() == 4 && bm.GetStopsToBuses.count("stop4") == 1 && bm.GetStopsToBuses.at("stop4").size() == 1 && bm.GetStopsToBuses.at("stop4")[0] == "bus2");

    assert(bm.GetBusesToStops.size() == 3 && bm.GetBusesToStops.count("bus3") == 1 && bm.GetBusesToStops.at("bus3").size() == 2 && bm.GetBusesToStops.at("bus3")[0] == "stop1" && bm.GetBusesToStops.at("bus3")[1] == "stop2");


    bm.NEW_BUS("bus4", 5, "stop1", "stop2", "stop3", "stop4", "stop5")
    assert(bm.GetBusesToStops.size() == 4 && bm.GetBusesToStops.count("bus1") == 1 && bm.GetBusesToStops.at("bus1").size() == 1 && bm.GetBusesToStops.at("bus1")[0] == "stop1");
    assert(bm.GetStopsToBuses.size() == 5 && bm.GetStopsToBuses.count("stop1") == 1 && bm.GetStopsToBuses.at("stop1").size() == 3 && bm.GetStopsToBuses.at("stop1")[0] == "bus1" && bm.GetStopsToBuses.at("stop1")[1] == "bus3" && bm.GetStopsToBuses.at("stop1")[2] == "bus5");

    assert(bm.GetBusesToStops.size() == 4 && bm.GetBusesToStops.count("bus2") == 1 && bm.GetBusesToStops.at("bus2").size() == 3 && bm.GetBusesToStops.at("bus2")[0] == "stop2" && bm.GetBusesToStops.at("bus2")[1]== "stop3" && bm.GetBusesToStops.at("bus2")[2] == "stop4");
    assert(bm.GetStopsToBuses.size() == 5 && bm.GetStopsToBuses.count("stop2") == 1 && bm.GetStopsToBuses.at("stop2").size() == 3 && bm.GetStopsToBuses.at("stop2")[0] == "bus2" && bm.GetStopsToBuses.at("stop2")[1] == "bus3" && bm.GetStopsToBuses.at("stop2")[2] == "bus4");
    assert(bm.GetStopsToBuses.size() == 5 && bm.GetStopsToBuses.count("stop3") == 1 && bm.GetStopsToBuses.at("stop3").size() == 2 && bm.GetStopsToBuses.at("stop3")[0] == "bus2" && bm.GetStopsToBuses.at("stop3")[1] == "bus4");
    assert(bm.GetStopsToBuses.size() == 5 && bm.GetStopsToBuses.count("stop4") == 1 && bm.GetStopsToBuses.at("stop4").size() == 2 && bm.GetStopsToBuses.at("stop4")[0] == "bus2" && bm.GetStopsToBuses.at("stop4")[1] == "bus4");

    assert(bm.GetBusesToStops.size() == 4 && bm.GetBusesToStops.count("bus3") == 1 && bm.GetBusesToStops.at("bus3").size() == 2 && bm.GetBusesToStops.at("bus3")[0] == "stop1" && bm.GetBusesToStops.at("bus3")[1] == "stop2");

    assert(bm.GetBusesToStops.size() == 4 && bm.GetBusesToStops.count("bus4") == 1 && bm.GetBusesToStops.at("bus4").size() == 5 && bm.GetBusesToStops.at("bus4")[0] == "stop1" && bm.GetBusesToStops.at("bus4")[1] == "stop2" && bm.GetBusesToStops.at("bus4")[2] == "stop3" && bm.GetBusesToStops.at("bus4")[3] == "stop4" && bm.GetBusesToStops.at("bus4")[4] == "stop5");
    cout << "TestAddingBuses SUCCESSFUL";
}

void TestQueryInput() {
    istringstream input;
    Query q;

    input.str("NEW_BUS bus 1 stop"s);
    input >> q;
    assert(q.type == QueryType::NEW_BUS && q.bus == "bus"s, q.size() == 1 && q.stops[0] == "stop"s && q.stop == ""s);

    input.str("NEW_BUS bus_b2 3 stop1 stop2 stop3"s);
    input >> q;
    assert(q.type == QueryType::NEW_BUS && q.bus == "bus_b2"s, q.size() == 3 && q.stops[0] == "stop1"s && q.stops[1] == "stop2"s && q.stops[2] == "stop3"s && q.stop == ""s);

    //название маршрута совпадает с наpванием остановки
    input.str("NEW_BUS stop1 2 stop1 stop2"s);
    input >> q;
    assert(q.type == QueryType::NEW_BUS && q.bus == "stop1"s, q.size() == 2 && q.stops[0] == "stop1"s && q.stops[1] == "stop2"s && q.stop == ""s);

    //название остановки повторяется несоклько раз
    input.str("NEW_BUS bus 3 stop1 stop1 stop2"s);
    input >> q;
    assert(q.type == QueryType::NEW_BUS && q.bus == "bus"s, q.size() == 2 && q.stops[0] == "stop1"s && q.stops[1] == "stop2"s && q.stop == ""s);

    input.str("NEW_BUS bus 3 stop1 stop2 stop1"s);
    input >> q;
    assert(q.type == QueryType::NEW_BUS && q.bus == "bus"s, q.size() == 2 && q.stops[0] == "stop1"s && q.stops[1] == "stop2"s && q.stop == ""s);

    input.str("NEW_BUS bus 3 stop2 stop1 stop1"s);
    input >> q;
    assert(q.type == QueryType::NEW_BUS && q.bus == "bus"s, q.size() == 2 && q.stops[0] == "stop2"s && q.stops[1] == "stop1"s && q.stop == ""s);

    input.str("NEW_BUS bus 5 stop1 stop1 stop2 stop1 stop2"s);
    input >> q;
    assert(q.type == QueryType::NEW_BUS && q.bus == "bus"s, q.size() == 2 && q.stops[0] == "stop1"s && q.stops[1] == "stop2"s && q.stop == ""s);

    input.str("NEW_BUS bus 5 stop1 stop1 stop1 stop2 stop2"s);
    input >> q;
    assert(q.type == QueryType::NEW_BUS && q.bus == "bus"s, q.size() == 2 && q.stops[0] == "stop1"s && q.stops[1] == "stop2"s && q.stop == ""s);
    
    cout << "QueryInput SUCCESSFUL" << endl;
}

void TestOutput() {
    BusManager bm;
    ostringstream output;

    output << bm.GetStopsForBus("bus");
    assert(output.str() == "No bus\n");
    output << bm.GetBusesForStop("stop");
    assert(output.str() == "No stop\n");
    output << bm.GetAllBuses();
    assert(output.str() == "No buses\n");

    bm.NEW_BUS("bus1", 1, "stop1");

    output << bm.GetStopsForBus("bus");
    assert(output.str() == "No bus\n");
    output << bm.GetBusesForStop("stop");
    assert(output.str() == "No stop\n");
    output << bm.GetAllBuses();
    assert(output.str() == "No buses\n");

    output << bm.GetStopsForBus("bus1");
    assert(output.str() == "Stop stop1: no interchange\n");
    output << bm.GetBusesForStop("stop1");
    assert(output.str() == "bus1\n");
    output << bm.GetAllBuses();
    assert(output.str() == "Bus bus1: stop1\n"

    bm.NEW_BUS("bus3", 2, "stop1", "stop2\n");

    output << bm.GetBusesForStop("stop1");
    assert(output.str() == "bus1 bus3\n");
    output << bm.GetAllBuses();
    assert(output.str() == "Bus bus1: stop1\nBus bus3: stop1 stop2")

    bm.NEW_BUS("bus2", 2, "stop3, stop2");

    output << bm.GetBusesForStop("stop1");
    assert(output.str() == "bus1 bus3\n");
    1) проверить на вывод метода GetBusesForStop класса BusManager
    2) проверить на вывод метода GetStopsForBus класса BusManager
    3) проверить на вывод метода GetAllBuses класса BusManager

    4) проверка на вывод
        1) busmanager пустой
        2) busmanager что-то содержит
            1) содрежит bus/stop
            2) не содержит bus/stop
}

void TestTaskBuses() {
    TestQueryInput();
    TestAddingBuses();
    TestOutput();
    cout << "Class is correct!"s << endl;
    return 0;
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


✅Проверка на ввод данных Query убедиться, хранятся корректные данные и нет никакого мусора
Проверка метода NewBus: Корректность хранения данных: Нужно проверить, что после добавления нового маршрута автобуса все остановки добавлены как для автобуса, так и для остановок.
Проверка ввода данных: Пустые запросы: Хотя по условию маршруты и остановки всегда передаются, важно обработать случай, если запрос поступит с неверными данными (например, пустое название автобуса или остановки).
Проверка вывода метода BusesForStop: Остановка не существует: Вывести "No stop".
                                     Корректность порядка автобусов: Автобусы должны выводиться в том порядке, в каком они были добавлены.
Проверка метода StopsForBus: Автобус не существует: Вывести "No bus".
                                     Перекрестки (interchanges): Убедиться, что при выводе остановок маршрута указываются корректные автобусы, кроме текущего.
                                     Остановка без пересадок: Должно выводиться "no interchange".                                     
Проверка метода AllBuses: Нет автобусов: Вывести "No buses".
                          Алфавитный порядок: Убедиться, что автобусы выводятся в алфавитном порядке с корректными остановками.
Дополнительные проверки: Проверить, что запросы корректно обрабатываются, если в системе нет автобусов или остановок.
                         Краевые случаи:
                         Один автобус и одна остановка.
                         Один автобус и несколько остановок.
                         Несколько автобусов, пересекающихся на одной остановке.
                         Обработка одинаковых названий маршрутов и остановок (например, если автобус и остановка имеют одинаковые имена).
Проверка при отсутствии данных: Запрос на остановку, которая не существует.
                                Запрос на автобус, который не существует.