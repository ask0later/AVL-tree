#pragma once

#include <cassert>
#include <functional>
#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <stack>
#include <algorithm>
#include <iterator>

namespace trees {
namespace details {

template <typename T> class Builder final {
public:
    template <class... Args> T *get_obj(Args&&... args) {
        auto tmp = std::make_unique<T>(std::forward<Args>(args)...);
        auto raw_ptr = tmp.get();
        buffer_.push_back(std::move(tmp));
        return raw_ptr;
    }
    
    auto move_begin() {
        return std::make_move_iterator(buffer_.begin());
    }

    auto move_end() {
        return std::make_move_iterator(buffer_.end());
    }
private:
    std::vector<std::unique_ptr<T>> buffer_;
}; // class Builder
} // namespace details

template <typename KeyT = int, typename Compare = std::less<KeyT>>
class AVLtree final {
    struct Node final {
        Node() = delete;
        Node(const KeyT &key) : key_(key) {}
        Node(const KeyT &key, Node *parent, Node *left = nullptr,
             Node *right = nullptr)
            : key_(key), parent_(parent), left_(left), right_(right) {}

        static Node *balance_node(const KeyT &key, Node *node) noexcept {
            int balance = balance_factor(node);

            if (balance > 1 && key < node->left_->key_)
                return rotate_right(node);

            if (balance < -1 && key > node->right_->key_)
                return rotate_left(node);

            if (balance > 1 && key > node->left_->key_) {
                node->left_ = rotate_left(node->left_);
                return rotate_right(node);
            }

            if (balance < -1 && key < node->right_->key_) {
                node->right_ = rotate_right(node->right_);
                return rotate_left(node);
            }

            return node;
        }

        void update_node() noexcept {
            height_ = 1 + std::max(height(left_), height(right_));
        }

        size_t count_bigger(Node *root) {
            Node *cur = root;
            size_t count = 0;

            while (cur != nullptr) {
                if (cur->key_ > key_) {
                    count += cur->count_right_childs_ + 1;
                    cur = cur->left_;
                } else if (cur->key_ < key_) {
                    cur = cur->right_;
                } else {
                    return count + cur->count_right_childs_;
                }
            }
            return count;
        }

    private:
        void update_and_balance(const KeyT &key) {
            if (left_)
                left_->update_node();
            if (right_)
                right_->update_node();

            left_ = balance_node(key, left_);
            right_ = balance_node(key, right_);
        }

        static int height(Node *node) noexcept {
            return node ? node->height_ : 0;
        }

        static Node *rotate(Node *&x, Node *&first_node, Node *&second_node,
                            size_t &first_address,
                            size_t &second_address) noexcept {
            auto y = first_node;

            auto T2 = second_node;
            second_node = x;
            first_node = T2;

            first_address = second_address;
            second_address = 1 + x->count_left_childs_ + x->count_right_childs_;

            x->update_node();
            y->update_node();

            if (T2)
                T2->parent_ = x;

            y->parent_ = x->parent_;
            x->parent_ = y;

            return y;
        }

        static Node *rotate_right(Node *&x) noexcept {
            return rotate(x, x->left_, x->left_->right_, x->count_left_childs_,
                          x->left_->count_right_childs_);
        }

        static Node *rotate_left(Node *&x) noexcept {
            return rotate(x, x->right_, x->right_->left_,
                          x->count_right_childs_,
                          x->right_->count_left_childs_);
        }

        static int balance_factor(Node *node) noexcept {
            return node ? (height(node->left_) - height(node->right_)) : 0;
        }

    public:
        Node *left_ = nullptr;
        Node *right_ = nullptr;
        Node *parent_ = nullptr;
        int height_ = 1;
        KeyT key_;
        size_t count_left_childs_ = 0;
        size_t count_right_childs_ = 0;
    }; // class Node

    class Iterator final {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = KeyT;
        using difference_type = size_t;
        using pointer = value_type*;
        using reference = value_type&;

        Iterator() = default;
        Iterator(Node *node, const AVLtree<KeyT, Compare> &tree)
            : node_(node), tree_(tree) {}

        Iterator &operator++() noexcept {
            if (node_ == nullptr || node_ == tree_.back_) {
                node_ = nullptr;
                return *this;
            }

            if (node_->right_) {
                node_ = find_min(node_->right_);
                return *this;
            }

            assert(node_->parent_ != nullptr);
            auto parent = node_->parent_;

            while (parent && node_ == parent->right_) {
                node_ = parent;
                parent = parent->parent_;
            }

            node_ = parent;
            return *this;
        }

        Iterator &operator--() noexcept {
            if (node_ == tree_.front_) {
                node_ = nullptr;
                return *this;
            }

            if (node_->left_) {
                node_ = find_max(node_->right_);
                return *this;
            }

            assert(node_->parent_ != nullptr);
            auto parent = node_->parent_;

            while (parent && node_ == parent->left_) {
                node_ = parent;
                parent = parent->parent_;
            }

            node_ = parent;
            return *this;
        }

        Iterator &operator++(int) noexcept {
            Iterator it(*this);
            ++(*this);
            return it;
        }

        Iterator &operator--(int) noexcept {
            Iterator it(*this);
            --(*this);
            return it;
        }

        KeyT operator*() const {
            if (node_ == nullptr)
                throw std::out_of_range("Iterator is at post-end");

            return node_->key_;
        }

        KeyT &operator*() {
            if (node_ == nullptr)
                throw std::out_of_range("Iterator is at post-end");

            return node_->key_;
        }

        KeyT const *operator->() const {
            if (node_ == nullptr)
                throw std::out_of_range("Iterator is at post-end");

            return &(node_->key_);
        }

        KeyT *operator->() {
            if (node_ == nullptr)
                throw std::out_of_range("Iterator is at post-end");

            return &(node_->key_);
        }

        friend bool operator==(const Iterator &lhs,
                               const Iterator &rhs) noexcept {
            return lhs.node_ == rhs.node_ && &lhs.tree_ == &rhs.tree_;
        }

    private:
        Node *find_min(Node *node) const noexcept {
            while (node && node->left_)
                node = node->left_;
            return node;
        }

        Node *find_max(Node *node) const noexcept {
            while (node && node->right_)
                node = node->right_;
            return node;
        }

        Node *node_ = nullptr;
        const AVLtree<KeyT, Compare> &tree_;
    }; // class Iterator;

public:
    AVLtree() = default;
    AVLtree(const KeyT &key) {
        details::Builder<Node> builder;
        root_ = builder.get_obj(key, nullptr);
        std::move(builder.move_begin(), builder.move_end(), std::back_inserter(buffer_));
    }

    AVLtree(const AVLtree<KeyT, Compare> &other) {
        if (other.root_ == nullptr) {
            return;
        }

        details::Builder<Node> builder;
        root_ = builder.get_obj(other.root_->key_);
        root_->height_ = other.root_->height_;

        std::stack<std::pair<Node*, Node*>> stack;
        stack.push({other.root_, root_});

        while (!stack.empty()) {
            auto [original, copy] = stack.top();
            copy->count_left_childs_ = original->count_left_childs_;
            copy->count_right_childs_ = original->count_right_childs_;
            stack.pop();

            if (original->left_) {
                copy->left_ = builder.get_obj(original->left_->key_);
                copy->left_->parent_ = copy;
                copy->left_->height_ = original->left_->height_;
                stack.push({original->left_, copy->left_});
            }

            if (original->right_) {
                copy->right_ = builder.get_obj(original->right_->key_);
                copy->right_->parent_ = copy;
                copy->right_->height_ = original->right_->height_;
                stack.push({original->right_, copy->right_});
            }
        }
        
        auto newIterBegin = builder.move_begin();
        auto newIterEnd = builder.move_end();
        buffer_.reserve(std::distance(newIterBegin, newIterEnd));

        std::move(newIterBegin, newIterEnd, std::back_inserter(buffer_));
    }

    AVLtree<KeyT, Compare> &operator=(const AVLtree<KeyT, Compare> &other) {
        if (this != &other) {
            auto tmp{other};
            std::swap(*this, tmp);
        }

        return *this;
    }

    AVLtree(AVLtree<KeyT, Compare> &&other) noexcept = default;
    AVLtree<KeyT, Compare> &
    operator=(AVLtree<KeyT, Compare> &&other) noexcept = default;
    ~AVLtree() = default;

    std::pair<Iterator, bool> insert(const KeyT &key) {
        auto [place, parent] = find(key);

        if (!place)
            return {Iterator{parent, *this}, false};

        details::Builder<Node> builder;
        *place = builder.get_obj(key, parent);
        auto current = *place;

        while (parent) {
            if (parent->left_ == current)
                parent->count_left_childs_++;
            else if (parent->right_ == current)
                parent->count_right_childs_++;

            if (current->left_)
                current->left_->update_node();
            if (current->right_)
                current->right_->update_node();

            current->left_ = Node::balance_node(key, current->left_);
            current->right_ = Node::balance_node(key, current->right_);

            current = parent;
            parent = parent->parent_;
        }

        update_front_back();

        auto newIterBegin = builder.move_begin();
        auto newIterEnd = builder.move_end();
        buffer_.reserve(buffer_.size() + std::distance(newIterBegin, newIterEnd));
        std::move(newIterBegin, newIterEnd, std::back_inserter(buffer_));
        
        return {Iterator{*place, *this}, true};
    }

    Iterator lower_bound(const KeyT &key) const {
        Iterator it = begin();

        if (back_ != nullptr)
            if (key > back_->key_)
                return Iterator{nullptr, this};

        while (it.node_ != nullptr && it.node_->key_ < key)
            ++it;

        return it;
    }

    Iterator upper_bound(const KeyT &key) const {
        Iterator it = back();

        if (front_ != nullptr)
            if (key < front_->key_)
                return Iterator{nullptr, this};

        while (it.node_ != nullptr && it.node_->key_ > key)
            --it;

        return it;
    }

    size_t get_num_elems_from_diapason(const KeyT &key1, const KeyT &key2) const {
        if (key1 > key2 || root_ == nullptr)
            return 0;

        Node *node1 = lower_bound_node(key1);
        Node *node2 = upper_bound_node(key2);
        if (node1 == nullptr || node2 == nullptr)
            return 0;

        size_t count1 = node1->count_bigger(root_);
        size_t count2 = node2->count_bigger(root_);

        assert(count1 + 2U > count2);
        return 1U + count1 - count2;
    }

    KeyT front() const { return front_->key_; }

    KeyT back() const { return back_->key_; }

    Iterator begin() const { return Iterator{front_, *this}; }

    Iterator end() const { return Iterator{nullptr, *this}; }

private:
    void update_front_back() {
        Node *front = root_, *back = root_;

        while (front->left_ != nullptr)
            front = front->left_;

        front_ = front;

        while (back->right_ != nullptr)
            back = back->right_;

        back_ = back;
    }
    
    Node* lower_bound_node(const KeyT &key) const {
        return bound_node(key, std::less<KeyT>{}, true);
    }
    
    Node* upper_bound_node(const KeyT &key) const {
        return bound_node(key, std::greater<KeyT>{}, false);
    }

    template <typename CompT>
    Node* bound_node(const KeyT &key, CompT comp, bool is_lower) const {
        Node* cur = root_;
        Node* ans = root_;
    
        while (cur != nullptr) {
            if (comp(key, cur->key_)) {
                ans = cur;
                cur = is_lower ? cur->left_ : cur->right_;
            } else if (cur->key_ == key) {
                return cur;
            } else {
                cur = is_lower ? cur->right_ : cur->left_;
            }
        }
    
        if (is_lower && ans->key_ < key) {
            return nullptr;
        } else if (!is_lower && ans->key_ > key) {
            return nullptr;
        }
    
        return ans;
    }

    std::pair<Node**, Node*> find(const KeyT &key) noexcept {
        Node *parent = nullptr;
        Node **place = &root_;

        while (*place) {
            if (key == (*place)->key_)
                return {nullptr, *place};

            if (key < (*place)->key_) {
                parent = *place;
                place = &parent->left_;
            } else {
                parent = *place;
                place = &parent->right_;
            }
        }

        return {place, parent};
    }

    Node *root_ = nullptr;
    Node *front_ = nullptr;
    Node *back_ = nullptr;
    std::vector<std::unique_ptr<Node>> buffer_;
}; // class AVL tree

} // namespace trees