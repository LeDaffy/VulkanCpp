#pragma once
#include <cstddef>
#include <stack>
#include <vector>
#include <cassert>
#include <queue>
#include <fmt/format.h>
#include <iterator>


namespace ngui {
template<typename T>
struct BTreeNode {
    std::optional<T> data;
    BTreeNode<T>* parent;
    BTreeNode<T>* left;
    BTreeNode<T>* right;

    BTreeNode() : parent(nullptr), left(nullptr), right(nullptr) {}
    BTreeNode(T data) : data(data), parent(nullptr), left(nullptr), right(nullptr) {}
    BTreeNode(BTreeNode& o) : data(o.data), parent(nullptr), left(nullptr), right(nullptr) {}
    constexpr BTreeNode& operator=(const BTreeNode& o) {
        data = o.data;
        parent = o.parent;
        left = o.left;
        right = o.right;
        return *this;
    };
    constexpr BTreeNode& operator=(BTreeNode& o) {
        data = o.data;
        parent = o.parent;
        left = o.left;
        right = o.right;
        return *this;
    }
};


template<typename T>
struct BTreeNodeIterator {
    BTreeNode<T>* node;
    BTreeNode<T>* parent;
    bool peak;
    constexpr BTreeNodeIterator(BTreeNode<T>* iter, BTreeNode<T>* parent, bool peak) : node(iter), parent(parent), peak(peak) {}
    BTreeNodeIterator begin() { 
        BTreeNode<T>* current = node;
        parent = current;
        bool peak = false;
        while (current.left != nullptr) {
            current = current.left;
        }
        if (current->parent == nullptr) {
            peak = true;
        }
        return BTreeNodeIterator(current, parent, peak);
    }
    BTreeNodeIterator next() {
        BTreeNode<T> current = node;
        // if (current->left == nullptr && current->right == nullptr) {
        //     return BTreeNodeIterator(current->parent, parent, kk
        // }

    }

};
//static_assert(std::forward_iterator<BTreeNodeIterator<int>>);

template<typename T>
struct FullBTree {


    constexpr FullBTree() : num_nodes(0) {}
    constexpr FullBTree(FullBTree& o) : root(o.root), num_nodes(o.num_nodes) {}
    constexpr FullBTree(FullBTree&& o) : root(o.root), num_nodes(o.num_nodes) {}

    template<typename... Args>
    constexpr FullBTree(Args... args) : root(new BTreeNode<T>(args...)), num_nodes(1) {}
    ~FullBTree() {
        std::stack<BTreeNode<T>*> deletion_stack;
        std::queue<BTreeNode<T>*> queue;
        if (root == nullptr) { return; }
        queue.push(root);
        while (queue.empty() == false) {
         
        auto* node = queue.front();
        deletion_stack.push(node);
        queue.pop();
 
        // Enqueue left child
        if (node->left != nullptr)
            queue.push(node->left);
 
        // Enqueue right child
        if (node->right != nullptr)
            queue.push(node->right);
        while (!deletion_stack.empty()) {
            delete deletion_stack.top();
            deletion_stack.pop();
        }
    }

    }

    auto head() -> BTreeNode<T>* {
        return root;
    }
    auto print_in_order() {
    fmt::print("[ ");
    in_order(root, [](BTreeNode<T>& node) {
            if (node.data.has_value()) {
                fmt::print("{}, ", node.data.value()); 
            } else { 
                fmt::print("?, "); 
            }
            });
    fmt::println(" ]");
    }

    void printBT(const std::string& prefix, const BTreeNode<T>* node, bool isLeft) {
        if( node != nullptr )
        {
            fmt::print("{}", prefix);

            fmt::print("{}", (isLeft ? "├──" : "└──" ));

            // print the value of the node
            if (node->data.has_value()) {
                fmt::println("{}",  node->data.value());
            } else {
                fmt::println("{}", "?");
            }

            // enter the next tree level - left and right branch
            printBT( prefix + (isLeft ? "│   " : "    "), node->left, true);
            printBT( prefix + (isLeft ? "│   " : "    "), node->right, false);
        }
    }
    void print()
    {
        printBT("", root, false);    
    }

    constexpr auto at(size_t index) const -> const BTreeNode<T>* {
        size_t counter = 0;
        std::stack<const BTreeNode<T>*> stack;

        const BTreeNode<T>* current = root;
        while (current != nullptr || stack.empty() == false) {
            while (current != nullptr) {
                stack.push(current);
                current = current->left;
            }
            current = stack.top();
            stack.pop();

            if (counter == index) { return current; }
            counter++;
            current = current->right;
        }
        return nullptr;
    }
    constexpr auto at(size_t index) -> BTreeNode<T>* {
        size_t counter = 0;
        std::stack<BTreeNode<T>*> stack;

        BTreeNode<T>* current = root;
        while (current != nullptr || stack.empty() == false) {
            while (current != nullptr) {
                stack.push(current);
                current = current->left;
            }
            current = stack.top();
            stack.pop();

            if (counter == index) { return current; }
            counter++;
            current = current->right;
        }
        return nullptr;
    }
    constexpr auto height() const -> size_t {
        return height(root);
    }
    constexpr auto sibling(BTreeNode<T>* node) const -> BTreeNode<T>* {
        if (node->parent == nullptr) {
            return nullptr;
        }
        if (node->parent->left == node) {
            return node->parent->right;
        }
        return node->parent->left;
    }
    constexpr auto height(const BTreeNode<T>* node) const -> size_t {
        if (node == nullptr) { return 0; }

        // Compute the height of each subtree
        size_t lheight = height(node->left);
        size_t rheight = height(node->right);

        // Use the larger one
        if (lheight > rheight)
            return (lheight + 1);
        else
            return (rheight + 1);
    }

    template<typename Function>
    auto pre_order(BTreeNode<T>* node, Function fn) {
        if (node == nullptr) { return; }
        fn(*node);
        pre_order(node->left, fn);
        pre_order(node->right, fn);
    }

    template<typename Function>
    auto in_order(BTreeNode<T>* node, Function fn) {
        std::stack<BTreeNode<T>*> stack;
        BTreeNode<T>* current = node;
        while (current != nullptr || stack.empty() == false) {
            while (current != nullptr) {
                stack.push(current);
                current = current->left;
            }
            current = stack.top();
            stack.pop();
            fn(*current);
            current = current->right;
        }
    }
    template<typename Function>
    auto post_order(BTreeNode<T>* node, Function fn) {
        if (node == nullptr) { return; }
        post_order(node->left, fn);
        post_order(node->right, fn);
        fn(*node);
    }
    auto split(BTreeNode<T>* const node) {
        assert(node != nullptr);
        node->left = new BTreeNode<T>(*node);
        node->right = new BTreeNode<T>(*node);
        node->left->data = std::move(node->data);
        node->data = std::nullopt;

        node->left->parent = node;
        node->right->parent = node;
        num_nodes += 2;
    }
    template<typename... Args>
    auto split_construct(BTreeNode<T>* const node, Args... args) {
        assert(node != nullptr);
        node->left = new BTreeNode<T>(*node);
        node->right = new BTreeNode<T>(args...);
        node->left->data = std::move(node->data);
        node->data = std::nullopt;

        node->left->parent = node;
        node->right->parent = node;
    }
    private:
    BTreeNode<T>* root;
    size_t num_nodes;
};
}
