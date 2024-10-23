#include <stack>
#include <numeric>
#include <algorithm>
#include <random> 
#include <iostream>

using namespace std;

template <typename It>
void PrintRange(It range_begin, It range_end) {
    for (auto it = range_begin; it != range_end; ++it) {
        cout << *it << " "s;
    }
    cout << endl;
}


template <typename Type>
class Queue {
public:
    void Push(const Type& element) {
        // напишите реализацию
        stack1_.push(element);
    }
    void Pop() {
        // напишите реализацию
        if (stack2_.empty()) {
            while (!stack1_.empty()) {
                stack2_.push(stack1_.top());
                stack1_.pop();
            }
        }
        stack2_.pop();
    }
    Type& Front() {
        // напишите реализацию
        if (stack2_.empty()) {
            while (!stack1_.empty()) {
                stack2_.push(stack1_.top());
                stack1_.pop();
            }
        }
        return stack2_.top();
    }
    const Type& Front() const {
        // напишите реализацию
        if (stack2_.empty()) {
            while (!stack1_.empty()) {
                stack2_.push(stack1_.top());
                stack1_.pop();
            }
        }
        return stack2_.top();
    }
    uint64_t Size() const {
        // напишите реализацию
        return stack1_.size() + stack2_.size();
    }
    bool IsEmpty() const {
        // напишите реализацию
        return stack1_.empty() && stack2_.empty();
    }

private:
    stack<Type> stack1_;
    stack<Type> stack2_;
};

int main() {
    Queue<int> queue;
    vector<int> values(5);

    // заполняем вектор для тестирования очереди
    iota(values.begin(), values.end(), 1);

    // random_shuffle   -> https://ru.cppreference.com/w/cpp/algorithm/random_shuffle
    // Перемешивает элементы в случайном порядке
    std::random_device rd;   // Источник случайных чисел
    std::mt19937 g(rd());    // Генератор на основе Mersenne Twister
    // Перемешиваем вектор с использованием std::shuffle
    std::shuffle(values.begin(), values.end(), g);

    PrintRange(values.begin(), values.end());

    cout << "Заполняем очередь"s << endl;

    // заполняем очередь и выводим элемент в начале очереди
    for (int i = 0; i < 5; ++i) {
        queue.Push(values[i]);
        cout << "Вставленный элемент "s << values[i] << endl;
        cout << "Первый элемент очереди "s << queue.Front() << endl;
    }

    cout << "Вынимаем элементы из очереди"s << endl;

    // выводим элемент в начале очереди и вытаскиваем элементы по одному
    while (!queue.IsEmpty()) {
        // сначала будем проверять начальный элемент, а потом вытаскивать,
        // так как операция Front на пустой очереди не определена
        cout << "Будем вынимать элемент "s << queue.Front() << endl;
        queue.Pop();
    }
    return 0;
}