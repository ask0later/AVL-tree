#pragma once

#include <iostream>
#include <functional>
#include <cassert>
#include <limits>
#include <string>

namespace trees {

    template <typename KeyT, typename Compare>
    class AVLtree;

    template <typename KeyT = int>
    class Node
    {
        friend class AVLtree<KeyT, std::less<KeyT>>;
    
    public:
        Node() = delete;
        Node(KeyT key) : key_(key), height_(1) {}
        Node(KeyT key, Node* parent, Node* left = nullptr, Node* right = nullptr) : key_(key),
                                                                                    parent_(parent),
                                                                                    left_(left),
                                                                                    right_(right),
                                                                                    height_(1) {}
        ~Node() = default;

        static int height(Node *node) 
        { 
            return node ? node->height_ : 0;
        } 

        static Node *rotate_right(Node *y) 
        { 
            Node *x = y->left_; 
            Node *T2 = x->right_; 

            x->right_ = y;
            y->left_ = T2; 

            y = update_node(y);
            x = update_node(x);

            if (T2)
            {
                T2->parent_ = y;
                y->count_left_childs_ = 1 + T2->count_left_childs_ + T2->count_right_childs_;
            }
            else
            {
                y->count_left_childs_ = 0;
            }

            x->parent_ = y->parent_;
            y->parent_ = x;

            x->count_left_childs_ = 1 + y->count_left_childs_ + x->count_right_childs_;

            if (x->parent_ != nullptr)
            {
                if (x == x->parent_->left_)
                    x->parent_->count_left_childs_ = 1 + x->count_left_childs_ + x->count_right_childs_;
                else
                    x->parent_->count_right_childs_ = 1 + x->count_left_childs_ + x->count_right_childs_;
            }

            return x; 
        } 
 
        static Node *rotate_left(Node *x)
        { 
            Node *y = x->right_; 
            Node *T2 = y->left_; 

            y->left_ = x;
            x->right_ = T2; 

            x = update_node(x);
            y = update_node(y);

            if (T2)
            {
                T2->parent_ = x;
                x->count_right_childs_ = 1 + T2->count_left_childs_ + T2->count_right_childs_;
            }
            else
            {
                x->count_right_childs_ = 0;
            }

            y->count_left_childs_ = 1 + x->count_left_childs_ + x->count_right_childs_;

            y->parent_ = x->parent_;
            x->parent_ = y;

            if (y->parent_ != nullptr)
            {
                if (y == y->parent_->left_)
                    y->parent_->count_left_childs_ = 1 + y->count_left_childs_ + y->count_right_childs_;
                else
                    y->parent_->count_right_childs_ = 1 + y->count_left_childs_ + y->count_right_childs_;
            }

            return y; 
        } 

        static int balance_factor(Node *node) 
        { 
            return node ? (height(node->left_) - height(node->right_)) : 0;
        }

        static Node *update_node(Node *node)
        {
            node->height_ = 1 + std::max(height(node->left_), height(node->right_));
            return node; 
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

        static Node *insert(KeyT key, Node *node, Node *parent) 
        { 
            if (node == nullptr) 
                return new Node(key, parent);

            if (key < node->key_)
            {
                node->count_left_childs_++;
                node->left_ = insert(key, node->left_, node);
            }
            else if (key > node->key_) 
            {
                node->count_right_childs_++;
                node->right_ = insert(key, node->right_, node);
            }
            else
            {
                return node; 
            }
            
            node = update_node(node);
            node = balance_node(key, node);

            return node; 
        }

        Node* lower_bound_fast(KeyT key)
        {
            if (key_ == key)
                return this;

            if (key_ > key)
            {
                if (left_ == nullptr)
                    return this;
                if (left_->key_ < key)
                    return this;
                return left_->lower_bound_fast(key);
            }

            if (right_ == nullptr)
                return nullptr;
            return right_->lower_bound_fast(key);
        }

        Node* upper_bound_fast(KeyT key)
        {
            if (key_ == key)
                return this;

            if (key_ < key)
            {
                if (right_ == nullptr)
                    return this;
                if (right_->key_ < key)
                    return this;
                return right_->upper_bound_fast(key);
            }

            if (left_ == nullptr)
                return nullptr;
            return left_->upper_bound_fast(key);
        }

        size_t distance_fast(Node *node1, Node *node2)
        {
            if (this == node2)
                return 1;
            
            size_t add = (node1->key_ <= key_ && key_ <= node2->key_) ? 1 : 0;

            if (parent_ != nullptr)
            {
                if (parent_ == node2)
                    return count_right_childs_ + 1;
                
                if (parent_->key_ > node2->key_)
                {
                    assert(right_ != nullptr);
                    return add + right_->distance_fast(node1, node2);
                }
                else
                {
                    if (right_ != nullptr)
                        return add + right_->distance_fast(node1, node2);
                    
                    return count_right_childs_ + parent_->distance_fast(node1, node2);
                }
            }

            return add + right_->distance_fast(node1, node2);
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
        Node* left_ = nullptr;
        Node* right_ = nullptr;
        Node* parent_ = nullptr;
        int height_;
        KeyT key_;
        size_t count_left_childs_ = 0;
        size_t count_right_childs_ = 0;

    }; // class Node

    template <typename KeyT = int, typename Compare = std::less<KeyT>>
    class AVLtree 
    {
        class Iterator
        {
            friend class AVLtree<KeyT, std::less<KeyT>>;
        public:
            KeyT POISON = std::numeric_limits<KeyT>::max();
            Iterator() = default;
            Iterator(Node<KeyT> *node, const AVLtree<KeyT, Compare> *tree) : node_(node), tree_(tree) {}

            bool operator==(Iterator& other)
            {
                return node_ == other.node_ && tree_ == other.tree_;
            }

            bool operator!=(Iterator& other)
            {
                return node_ != other.node_ || tree_ != other.tree_;
            }

            Iterator &operator++()
            {
                if (node_ == nullptr || node_ == tree_->back_)
                {
                    node_ = nullptr;
                    return *this;
                }
                
                if (node_->right_ != nullptr)
                {
                    node_ = node_->right_;
                    while (node_->left_ != nullptr)
                        node_ = node_->left_;
                    
                    return *this;
                }

                assert(node_->parent_ != nullptr);

                Node<KeyT> *parent = node_->parent_;
                Node<KeyT> *current = node_;
                
                while (current == parent->right_)
                {
                    current = parent;
                    parent = parent->parent_;

                    assert(parent != nullptr);
                }

                node_ = parent;
                return *this;
            }

            Iterator &operator++(int)
            {
                Iterator it(*this);
                ++(*this);
                return it;
            }

            Iterator &operator--()
            {
                if (node_ == tree_->front_)
                {
                    node_ = nullptr;
                    return *this;
                }
                
                if (node_->left_ != nullptr)
                {
                    node_ = node_->left_;
                    while (node_->right_ != nullptr)
                        node_ = node_->right_;
                    
                    return *this;
                }

                assert(node_->parent_ != nullptr);

                Node<KeyT> *parent = node_->parent_;
                Node<KeyT> *current = node_;

                while (current == parent->left_)
                {
                    current = parent;
                    parent = parent->parent_;

                    assert(parent != nullptr);
                }

                node_ = parent;
                return *this;
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
                    return POISON;

                return node_->key_;
            }

        private:
            Node<KeyT> *node_;
            const AVLtree<KeyT, Compare> *tree_;

        }; // class Iterator;

    public:
        AVLtree() = default;
        AVLtree(KeyT key) : root_(new Node(key)), size_(1), height_(1) {}
        
        AVLtree(const AVLtree<KeyT, Compare> &other) = delete;
        AVLtree<KeyT, Compare> &operator=(const AVLtree<KeyT, Compare> &other) = delete;

        AVLtree(const AVLtree<KeyT, Compare> &&other) = delete;
        AVLtree<KeyT, Compare> &operator=(const AVLtree<KeyT, Compare> &&other) = delete;

        ~AVLtree()
        {
            Iterator it_end = end();
            std::vector<Node<KeyT>*> nodes;
            for (Iterator it = begin(); it != it_end; ++it)
                nodes.push_back(it.node_);

            for (auto& node : nodes)
                delete node;
        }

        void insert(KeyT key)
        {
            root_ = Node<KeyT>::insert(key, root_, nullptr);

            Node<KeyT> *front = root_, *back = root_;

            while (front->left_ != nullptr)
                front = front->left_;

            front_ = front;
            
            while (back->right_ != nullptr)
                back = back->right_;

            back_ = back;
        }

        Iterator lower_bound(KeyT key) const
        {
            Iterator it = begin();

            if (back_ != nullptr)
                if (key > back_->key_)
                    return Iterator{nullptr, this};

            for (; it.node_ != nullptr && it.node_->key_ < key; ++it)
                ;

            return it;
        }

        Iterator upper_bound(KeyT key) const
        {
            Iterator it = back();

            if (front_ != nullptr)
                if (key < front_->key_)
                    return Iterator{nullptr, this};

            for (; it.node_ != nullptr && it.node_->key_ > key; --it)
                ;

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
            
            size_t count = 0;
            Iterator it_end = end();

            for (Iterator it = begin(); it != it_end; ++it)
            {
                if (key1 <= *it && *it <= key2)
                    count++;

                if (*it > key2)
                    break;
            }

            return count;
        }

        size_t get_num_elems_from_diapason_fast(KeyT key1, KeyT key2) const
        {
            if (key1 > key2 || root_ == nullptr)
                return 0;
            
            Node<KeyT>* node1 = root_->lower_bound_fast(key1);
            Node<KeyT>* node2 = root_->upper_bound_fast(key2);
            if (node1 == nullptr || node2 == nullptr)
                return 0;
            
            return node1->distance_fast(node1, node2);
        }

        Iterator front() const
        {
            return Iterator{front_, this};
        }

        Iterator back() const
        {
            return Iterator{back_, this};
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
        size_t height_ = 0;
        size_t size_ = 0;
        Node<KeyT> *root_ = nullptr;
        Node<KeyT> *front_ = nullptr;
        Node<KeyT> *back_ = nullptr;
    }; // class AVL tree 

} // namespace tree