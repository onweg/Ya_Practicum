#include <stack>

using namespace std;

template <typename Type>
class Queue {
public:
    void Push(const Type& element) {
        // напишите реализацию
    }
    void Pop() {
        // напишите реализацию
    }
    Type& Front() {
        // напишите реализацию
    }
    const Type& Front() const {
        // напишите реализацию
    }
    uint64_t Size() const {
        // напишите реализацию
    }
    bool IsEmpty() const {
        // напишите реализацию
    }

private:
    stack<Type> stack1_;
    stack<Type> stack2_;
};