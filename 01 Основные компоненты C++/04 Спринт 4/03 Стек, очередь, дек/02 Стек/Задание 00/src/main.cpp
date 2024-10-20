#include <iostream>
#include <vector>

using namespace std;

template <typename It>
void PrintRange(It range_begin, It range_end) {
    for (auto it = range_begin; it != range_end; ++it) {
        cout << *it << " "s;
    }
    cout << endl;
}

template <typename Type>
class Stack {
public:
    void Push(const Type& element) {
        // напишите реализацию
        elements_.push_back(element);
    }
    void Pop() {
        // напишите реализацию
        if (elements_.empty()) {
            throw out_of_range("Невозможно удалить элемент в пустом стеке"s);
        }
        elements_.pop_back();
    }
    const Type& Peek() const {
        // напишите реализацию
        if (elements_.empty()) {
            throw out_of_range("Невозможно взять элемент в пустом стеке"s);
        }
        return elements_[elements_.size() - 1];
    }
    Type& Peek() {
        // напишите реализацию
        if (elements_.empty()) {
            throw out_of_range("Невозможно взять элемент в пустом стеке"s);
        }
        return elements_[elements_.size() - 1];
    }
    void Print() const {
        // напишите реализацию
        for (const auto element : elements_) {
            cout << element << " "s;
        }
        cout << endl;
    }
    uint64_t Size() const {
        // напишите реализацию
        return elements_.size();
    }
    bool IsEmpty() const {
        // напишите реализацию
        return elements_.empty();
    }

private:
    vector<Type> elements_;
};

int main() {
    Stack<int> stack;
    for (uint32_t i = 0; i < 10; ++i) {
        stack.Push(i);
        stack.Print();
    }
    const int& top = stack.Peek();
    stack.Print();
    while (!stack.IsEmpty()) {
        stack.Pop();
        stack.Print();
    }

    return 0;
}
