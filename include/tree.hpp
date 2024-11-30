#pragma once

#include <iostream>
#include <functional>
#include <cassert>
#include <limits>
#include <string>

namespace trees {

    template <typename KeyT = int, typename Compare = std::less<KeyT>>
    class AVLtree
    {
        class Iterator;

        struct Node
        {
            Node() = delete;
            Node(KeyT key) : key_(key), height_(1) {}
            Node(KeyT key, Node* parent, Node* left = nullptr, Node* right = nullptr) :
                key_(key),
                parent_(parent),
                left_(left),
                right_(right),
                height_(1) {}
            ~Node() = default;

            std::pair<Iterator, bool> insert(KeyT key)
            {
                std::pair<Iterator, bool> pair;

                if (key < key_)
                {
                    if (!left_)
                    {   
                        left_ = new Node(key, this);
                        count_left_childs_++;
                        return {Iterator{left_, nullptr}, true};
                    }
                    
                    pair = left_->insert(key);
                    if (pair.second)
                        count_left_childs_++;
                }
                else if (key > key_)
                {
                    if (!right_)
                    {   
                        right_ = new Node(key, this);
                        count_right_childs_++;
                        return {Iterator{right_, nullptr}, true};
                    }
                    
                    pair = right_->insert(key);
                    if (pair.second)
                        count_right_childs_++;
                }
                else
                {
                    return {Iterator{this, nullptr}, false};
                }

                if (left_)
                    left_->update_node();
                if (right_)
                    right_->update_node();

                left_ = balance_node(key, left_);
                right_ = balance_node(key, right_);                

                return pair;
            }

            static Node *balance_node(KeyT key, Node* node)
            {
                int balance = balance_factor(node); 

                if (balance > 1 && key < node->left_->key_) 
                    return rotate_right(node); 

                if (balance < -1 && key > node->right_->key_) 
                    return rotate_left(node); 

                if (balance > 1 && key > node->left_->key_)
                { 
                    node->left_ = rotate_left(node->left_); 
                    return rotate_right(node); 
                } 

                if (balance < -1 && key < node->right_->key_)
                { 
                    node->right_ = rotate_right(node->right_); 
                    return rotate_left(node); 
                }

                return node;
            }

            size_t count_bigger(Node *root)
            {
                Node *cur = root;
                size_t count = 0;

                while (cur != nullptr)
                {
                    if (cur->key_ > key_)
                    {
                        count += cur->count_right_childs_ + 1;
                        cur = cur->left_;
                    }
                    else if (cur->key_ < key_)
                    {
                        cur = cur->right_;
                    }
                    else
                    {
                        return count + cur->count_right_childs_;
                    }  
                }
                return count;
            }
            
            void print_in_preorder() const
            {
                if (parent_)
                    std::cout << "#" << key_ << " " << parent_->key_ << " " << height_ << "#" << "(";
                else
                    std::cout << "#" << key_ << " " << height_ << "#" << "(";

                if (left_)
                    left_->print_in_preorder();
                
                std::cout << ", ";

                if (right_)
                    right_->print_in_preorder();
            
                std::cout << ")";
            }

            void print_vertical(const std::string& prefix, bool is_left) const
            {
                std::cout << prefix;
                std::cout << (is_left ? "├──" : "└──" );
                std::cout << key_ << "(" << count_left_childs_ << ", " << count_right_childs_ << ")" << std::endl;

                if (left_ != nullptr)
                    left_->print_vertical(prefix + (is_left ? "│   " : "    "), true);
                if (right_ != nullptr)
                    right_->print_vertical(prefix + (is_left ? "│   " : "    "), false);
            }

        private:

            static int height(Node *node)
            {
                return node ? node->height_ : 0;
            }

            static Node *rotate(Node **x, Node **first_node, Node **second_node, size_t *first_address, size_t *second_address)
            {
                Node *y = *first_node;
                Node *T2 = *second_node;
                *second_node = *x;
                *first_node = T2;

                *first_address = *second_address;
                *second_address = 1 + (*x)->count_left_childs_ + (*x)->count_right_childs_;

                (*x)->update_node();
                y->update_node();

                if (T2)
                    T2->parent_ = *x;

                y->parent_ = (*x)->parent_;
                (*x)->parent_ = y;

                return y;
            }

            static Node *rotate_right(Node *x)
            {
                return rotate(std::addressof(x), std::addressof(x->left_), std::addressof(x->left_->right_), std::addressof(x->count_left_childs_), std::addressof(x->left_->count_right_childs_));
            }

            static Node *rotate_left(Node *x)
            {
                return rotate(std::addressof(x), std::addressof(x->right_), std::addressof(x->right_->left_), std::addressof(x->count_right_childs_), std::addressof(x->right_->count_left_childs_));
            }

            static int balance_factor(Node *node) 
            { 
                return node ? (height(node->left_) - height(node->right_)) : 0;
            }

            void update_node()
            {
                height_ = 1 + std::max(height(left_), height(right_)); 
            }

        public:
            Node* left_ = nullptr;
            Node* right_ = nullptr;
            Node* parent_ = nullptr;
            int height_;
            KeyT key_;
            size_t count_left_childs_ = 0;
            size_t count_right_childs_ = 0;
        }; // class Node

        class Iterator
        {
        public:
        
            Iterator() = default;
            Iterator(Node *node, const AVLtree<KeyT, Compare> *tree) : node_(node), tree_(tree) {}

            Iterator &operator++()
            {
                if (node_ == nullptr || node_ == tree_->back_)
                {
                    node_ = nullptr;
                    return *this;
                }

                return do_next_step(true);
            }

            Iterator &operator--()
            {
                if (node_ == tree_->front_)
                {
                    node_ = nullptr;
                    return *this;
                }
                return do_next_step(false);
            }

            Iterator &operator++(int)
            {
                Iterator it(*this);
                ++(*this);
                return it;
            }

            Iterator &operator--(int)
            {
                Iterator it(*this);
                --(*this);
                return it;
            }

            KeyT operator*()
            {
                if (node_ == nullptr)
                    throw std::out_of_range("Iterator is at post-end");

                return node_->key_;
            }

            Node* get_node() const
            {
                return node_;
            }

            void set_tree(const AVLtree<KeyT, Compare> *tree)
            {
                tree_ = tree;
            }

            friend bool operator==(const Iterator& lhs, const Iterator& rhs)
            {
                return lhs.node_ == rhs.node_ && lhs.tree_ == rhs.tree_;
            }
        
        private:

            Iterator &do_next_step(const bool forward)
            {
                Node* A = nullptr, *B = nullptr;

                if (forward)
                    A = node_->right_;
                else
                    A = node_->left_;

                if (A != nullptr)
                {
                    node_ = A;
                    if (forward)
                    {
                        while (node_->left_ != nullptr)
                            node_ = node_->left_;
                    }
                    else
                    {
                        while (node_->right_ != nullptr)
                            node_ = node_->right_;
                    }
                    
                    return *this;
                }

                assert(node_->parent_ != nullptr);

                Node *parent = node_->parent_;
                Node *current = node_;

                if (forward)
                    B = parent->right_;
                else
                    B = parent->left_;
                
                while (current == B)
                {
                    current = parent;
                    parent = parent->parent_;

                    if (forward)
                        B = parent->right_;
                    else
                        B = parent->left_;
                    
                    assert(parent != nullptr);
                }

                node_ = parent;
                return *this;
            }

            Node *node_ = nullptr;
            const AVLtree<KeyT, Compare> *tree_ = nullptr;
        }; // class Iterator;

    public:
        AVLtree() = default;
        AVLtree(KeyT key) : root_(new Node(key)), height_(1) {}
        
        AVLtree(const AVLtree<KeyT, Compare> &other) = delete;
        AVLtree<KeyT, Compare> &operator=(const AVLtree<KeyT, Compare> &other) = delete;

        AVLtree(AVLtree<KeyT, Compare> &&other) = delete;
        AVLtree<KeyT, Compare> &operator=(AVLtree<KeyT, Compare> &&other) = delete;

        ~AVLtree()
        {
            Iterator it_end = end();
            std::vector<Node*> nodes;

            for (Iterator it = begin(); it != it_end; ++it)
                nodes.push_back(it.get_node());

            for (auto& node : nodes)
                delete node;
        }

        std::pair<Iterator, bool> insert(KeyT key)
        {
            std::pair<Iterator, bool> pair;

            if (root_ == nullptr)
            {
                root_ = new Node(key, nullptr);
                pair = {Iterator{root_, this}, true};
            }
            else
            {
                pair = root_->insert(key);
                root_ = Node::balance_node(key, root_);
                pair.first.set_tree(this);
            }

            update_front_back();

            return pair;
        }

        Iterator lower_bound(KeyT key) const
        {
            Iterator it = begin();

            if (back_ != nullptr)
                if (key > back_->key_)
                    return Iterator{nullptr, this};
            
            while (it.node_ != nullptr && it.node_->key_ < key)
                ++it;

            return it;
        }

        Iterator upper_bound(KeyT key) const
        {
            Iterator it = back();

            if (front_ != nullptr)
                if (key < front_->key_)
                    return Iterator{nullptr, this};

            while (it.node_ != nullptr && it.node_->key_ > key)
                --it;

            return it;
        }

        size_t distance(Iterator &it1, Iterator &it2) const
        {
            if (it1.node_ == nullptr || it2.node_ == nullptr || *it1 > *it2)
                return 0;

            size_t count = 1;
            for (; it1.node_ != it2.node_; ++it1, ++count) ;

            return count;
        }

        size_t get_num_elems_from_diapason(KeyT key1, KeyT key2) const
        {
            if (key1 > key2 || root_ == nullptr)
                return 0;

            Node* node1 = lower_bound_node(key1);
            Node* node2 = upper_bound_node(key2);
            if (node1 == nullptr || node2 == nullptr)
                return 0;

            size_t count1 = node1->count_bigger(root_);
            size_t count2 = node2->count_bigger(root_);

            assert(count1 + 2U > count2);
            return 1U + count1 - count2;
        }

        KeyT front() const
        {
            return front_->key_;
        }

        KeyT back() const
        {
            return back_->key_;
        }

        Iterator begin() const
        {
            return Iterator{front_, this};
        }

        Iterator end() const
        {
            return Iterator{nullptr, this};
        }

        void print() const 
        {
            root_->print_in_preorder();
            std::cout << std::endl;
        }

        void print_vertical() const 
        {
            root_->print_vertical("", false);
            std::cout << std::endl;
        }

    private:
        void update_front_back()
        {
            Node *front = root_, *back = root_;

            while (front->left_ != nullptr)
                front = front->left_;

            front_ = front;
            
            while (back->right_ != nullptr)
                back = back->right_;

            back_ = back;
        }

        Node* lower_bound_node(KeyT key) const
        {
            Node *cur = root_;
            Node *ans = root_;

            while (cur != nullptr)
            {
                if (cur->key_ > key)
                {
                    ans = cur;
                    cur = cur->left_; 
                }
                else if (cur->key_ < key)
                {
                    cur = cur->right_;
                }
                else
                {
                    return cur;
                }
            }

            if (ans->key_ < key)
                return nullptr;

            return ans;
        }

        Node* upper_bound_node(KeyT key) const
        {
            Node *cur = root_;
            Node *ans = root_;

            while (cur != nullptr)
            {
                if (cur->key_ < key)
                {
                    ans = cur;
                    cur = cur->right_; 
                }
                else if (cur->key_ > key)
                {
                    cur = cur->left_;
                }
                else
                {
                    return cur;
                }
            }

            if (ans->key_ > key)
                return nullptr;
                
            return ans;
        }
        
        size_t height_ = 0;
        Node *root_ = nullptr;
        Node *front_ = nullptr;
        Node *back_ = nullptr;
    }; // class AVL tree

} // namespace tree