#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include <map>

using namespace std;

int CountAndAddNewDogs(const vector<string>& new_dogs,
                       const map<string, int>& max_amount,
                       map<string, int>& shelter) {
    int count_added = count_if(new_dogs.begin(), new_dogs.end(), [&](const string& dog){
        if (shelter[dog] + 1 > max_amount.find(dog)->second) return 0;
        shelter[dog] += 1;
        return 1;
        });
    return count_added;
}

int main() {
    map<string, int> shelter {
        {"landseer"s, 1},
        {"otterhound"s, 2},
        {"pekingese"s, 2},
        {"pointer"s, 3}
    };
    const map<string, int> max_amount {
        {"landseer"s, 2},
        {"otterhound"s, 3},
        {"pekingese"s, 4},
        {"pointer"s, 7}
    };
    const vector<string> new_dogs {
        "landseer"s,
        "otterhound"s,
        "otterhound"s,
        "otterhound"s,
        "pointer"s
    };
    cout << CountAndAddNewDogs(new_dogs, max_amount, shelter);
    return 0;
}
