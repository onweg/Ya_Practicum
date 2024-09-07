#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

struct Animal{
    string name;
    int number;

    bool operator<(const Animal& other) const {
        return make_pair(name, number) < make_pair(other.name, other.number);
    }

    bool operator> (const Animal& other) const {
        return make_pair(name, number) > make_pair(other.name, other.number);
    }
 };

template <typename Term>
map<Term, int> ComputeTermFreqs(const vector<Term>& terms) {
    map<Term, int> term_freqs;
    for (const Term& term : terms) {
        ++term_freqs[term];
    }
    return term_freqs;
}

Animal FindMaxFreqAnimal(const vector<Animal>& animals) {
    // верните животного с максимальной частотой
    // нужен список с животными и сколько раз оно встречается
    map<Animal, int> stat = ComputeTermFreqs(animals);
    // перебрать их и найти сначала самое частое, а потом синтаксически маленькое
    int max_count = 0;
    Animal max_count_animal = animals[0];
    for (const auto& [animal, count] : stat) {
        if (count > max_count) {
            max_count_animal = animal;
            max_count = count;
        } else if (count == max_count && max_count_animal > animal) {
            max_count_animal = animal;
        }
    }
    return max_count_animal;
}

int main() {
    const vector<Animal> animals = {
        {"Murka"s, 5},
        {"Belka"s, 6},
        {"Murka"s, 7},
        {"Murka"s, 5}, 
        {"Belka"s, 6},
    };
    const Animal max_freq_animal = FindMaxFreqAnimal(animals);
    cout << max_freq_animal.name << " "s
         << max_freq_animal.number << endl;
}


//проработать структуру animal и оператор < для этой структуры 
//узанать, что выводить, либо само животное как пару или название животного и количество солько оно встречается 