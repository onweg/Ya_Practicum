#include <cassert>
#include <iostream>
#include <memory>

template <typename T>
struct TreeNode;

template <typename T>
using TreeNodePtr = std::unique_ptr<TreeNode<T>>;

template <typename T>
struct TreeNode {
    TreeNode(T val, TreeNodePtr<T>&& left, TreeNodePtr<T>&& right)
        : value(std::move(val))
        , left(std::move(left))
        , right(std::move(right)) {
    }

    T value;
    TreeNodePtr<T> left;
    TreeNodePtr<T> right;
    TreeNode* parent = nullptr;
};

template <typename T>
bool CheckTreeProperty(const TreeNode<T>* node) noexcept {
    // реализуйте функцию
    return false;
}

template <typename T>
TreeNode<T>* begin(TreeNode<T>* node) noexcept {
    // Реализуйте функцию
    return nullptr;
}

template <typename T>
TreeNode<T>* next(TreeNode<T>* node) noexcept {
    // Реализуйте функцию
    return nullptr;
}

// функция создаёт новый узел с заданным значением и потомками
TreeNodePtr<int> N(int val, TreeNodePtr<int>&& left = {}, TreeNodePtr<int>&& right = {}) {
    // Реализуйте функцию
    return nullptr;
}

int main() {
    using namespace std;
    using T = TreeNode<int>;
    auto root1 = N(6, N(4, N(3), N(5)), N(7));
    assert(CheckTreeProperty(root1.get()));

    T* iter = begin(root1.get());
    while (iter) {
        cout << iter->value << " "s;
        iter = next(iter);
    }
    cout << endl;

    auto root2 = N(6, N(4, N(3), N(5)), N(7, N(8)));
    assert(!CheckTreeProperty(root2.get()));

    // Функция DeleteTree не нужна. Узлы дерева будут рекурсивно удалены
    // благодаря деструкторам unique_ptr
}