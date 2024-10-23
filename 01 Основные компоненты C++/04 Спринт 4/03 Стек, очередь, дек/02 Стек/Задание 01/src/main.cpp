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
        PrintRange(elements_.begin(), elements_.end());
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

template <typename Type>
class StackMin {
public:
    void Push(const Type& element) {
    // напишите реализацию метода
        if (!elements_.IsEmpty() && element > minimuns_.Peek()) {
            elements_.Push(element);
            minimuns_.Push(minimuns_.Peek());
        } else {
            elements_.Push(element);
            minimuns_.Push(element);
        }
    }
    void Pop() {
    // напишите реализацию метода
        elements_.Pop();
        minimuns_.Pop();
    }
    const Type& Peek() const {
        return elements_.Peek();
    }
    Type& Peek() {
        return elements_.Peek();
    }
    void Print() const {
        // работу этого метода мы проверять не будем,
        // но если он вам нужен, то можете его реализовать
        elements_.Print();
    }
    uint64_t Size() const {
        return elements_.size();
    }
    bool IsEmpty() const {
        return elements_.IsEmpty();
    }
    const Type& PeekMin() const {
    // напишите реализацию метода
        return minimuns_.Peek();
    }
    Type& PeekMin() {
    // напишите реализацию метода
        return minimuns_.Peek();
    }
private:
    Stack<Type> elements_;
    // возможно, здесь вам понадобится что-то изменить
    Stack<Type> minimuns_; //этот stack хранит минимумы на каждом шаге добавления в основной стек

};

int main() {
    StackMin<int> stack;
    vector<int> values(5);

    // заполняем вектор для тестирования нашего стека
    iota(values.begin(), values.end(), 1);
    // перемешиваем значения
    std::random_device rd;   // Источник случайных чисел
    std::mt19937 g(rd());    // Генератор на основе Mersenne Twister
    shuffle(values.begin(), values.end(), g);

    // заполняем стек
    for (int i = 0; i < 5; ++i) {
        stack.Push(values[i]);
    }
    // печатаем стек и его минимум, постепенно убирая из стека элементы
    while (!stack.IsEmpty()) {
        stack.Print();
        cout << "Минимум = "s << stack.PeekMin() << endl;
        stack.Pop();
    }
}
