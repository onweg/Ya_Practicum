#include <string>
#include <iostream>

using namespace std;

enum class AnimalType {
    Cat,
    Dog,
    Mouse,
};

class Animal {
public:

    Animal() {
        type_ = AnimalType::Cat;
        name_ = ""s;
        owner_name_ = ""s;
    }

    Animal(AnimalType type, const string& name, const string& owner_name) {
        type_ = type;
        name_ = name;
        owner_name_ = owner_name;
    }

    AnimalType GetType() const {
        return type_;
    }

    const string& GetName() const {
        return name_;
    }

    const string& GetOwnerName() const {
        return owner_name_;
    }

private:
    AnimalType type_;
    string name_;
    string owner_name_;
};

int main() {
    Animal animal;
    cout << animal.GetName() << " "s << animal.GetOwnerName() << " "s << static_cast<int>(animal.GetType()) << endl;
    Animal a(AnimalType::Dog, "Rax"s, "Ted"s);
    cout << a.GetName() << " "s << a.GetOwnerName() << " "s << static_cast<int>(a.GetType()) << endl;


}