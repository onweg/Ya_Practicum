#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <random>

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
        elements_.push_back(element);
    }
    void Pop() {
        elements_.pop_back();
    }
    const Type& Peek() const {
        return elements_.back();
    }
    Type& Peek() {
        return elements_.back();
    }
    void Print() const {
        PrintRange(elements_.begin(), elements_.end());
    }
    uint64_t Size() const {
        return elements_.size();
    }
    bool IsEmpty() const {
        return elements_.empty();
    }

private:
    vector<Type> elements_;
};

template <typename Type>
class SortedStack {
public:
    void Push(const Type& element) {
    // напишите реализацию метода
        Stack<int> tmp_stack;
        while (!elements_.IsEmpty() && elements_.Peek() < element) {
            tmp_stack.Push(elements_.Peek());
            elements_.Pop();
        }
        elements_.Push(element);
        while (!tmp_stack.IsEmpty()) {
            elements_.Push(tmp_stack.Peek());
            tmp_stack.Pop();
        }
    }
    void Pop() {
    // напишите реализацию метода
    elements_.Pop();
    }
    const Type& Peek() const {
    return elements_.Peek();
    }
    Type& Peek() {
    return elements_.Peek();
    }
    void Print() const {
    elements_.Print();
    }
    uint64_t Size() const {
    return elements_.Size();
    }
    bool IsEmpty() const {
    return elements_.IsEmpty();
    }
private:
    Stack<Type> elements_;
};

int main() {
    SortedStack<int> stack;
    vector<int> values(5);

    // заполняем вектор для тестирования нашего стека
    iota(values.begin(), values.end(), 1);
    // перемешиваем значения
    std::random_device rd;   // Источник случайных чисел
    std::mt19937 g(rd());    // Генератор на основе Mersenne Twister
    shuffle(values.begin(), values.end(), g);

    // заполняем стек и проверяем, что сортировка сохраняется после каждой вставки
    for (int i = 0; i < 5; ++i) {
        cout << "Вставляемый элемент = "s << values[i] << endl;
        stack.Push(values[i]);
        stack.Print();
    }
}