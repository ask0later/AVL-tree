#pragma once

#include <iostream>
#include <functional>
#include <cassert>

namespace trees {

    template <typename KeyT, typename Compare>
    class AVLtree;

    template <typename KeyT = int>
    class Node
    {
        friend class AVLtree<KeyT, std::less<KeyT>>;
    
    public:
        Node() = delete;
        Node(KeyT key) : key_(key) {}
        Node(KeyT key, Node* parent, Node* left = nullptr, Node* right = nullptr) : key_(key),
                                                                                    parent_(parent),
                                                                                    left_(left),
                                                                                    right_(right),
                                                                                    height_(1),
                                                                                    balance_factor_(0) {}

        ~Node()
        {
            delete left_;
            delete right_;
        }

        bool insert(KeyT key)
        {
            if (key == key_)
                return false;
            
            if (key < key_) // left
            {
                if (left_ == nullptr)
                {
                    left_ = new Node(key, this);
                    return true;
                }
                
                return left_->insert(key);
            }
            else // right
            {
                if (right_ == nullptr)
                {
                    right_ = new Node(key, this);
                    return true;
                }
                
                return right_->insert(key);
            }
        }

    private:
    
        Node* left_ = nullptr;
        Node* right_ = nullptr;
        Node* parent_ = nullptr;
        int height_;
        int balance_factor_;
        KeyT key_; 

    }; // class Node

    template <typename KeyT = int, typename Compare = std::less<KeyT>>
    class AVLtree 
    {
        class Iterator
        {
        public:
            Iterator() = default;
            Iterator(Node<KeyT> *node, const AVLtree<KeyT, Compare> *tree) : node_(node), tree_(tree) {}

            // Iterator(const Iterator &other) = delete;
            // Iterator &operator=(const Iterator &other) = delete;

            // Iterator(const Iterator &&other) = delete;
            // Iterator &operator=(const Iterator &&other) = delete;

            bool operator==(Iterator& other)
            {
                return node_ == other.node_ && tree_ == other.tree_;
            }

            bool operator!=(Iterator& other)
            {
                return node_ != other.node_ || tree_ != other.tree_;
            }

            Iterator operator++()
            {
                if (*this == tree_->end() || *this == tree_->back())
                {
                    *this = tree_->end();
                    return tree_->end();
                }
                
                if (node_->right_ != nullptr)
                {
                    node_ = node_->right_;
                    while (node_->left_ != nullptr)
                        node_ = node_->left_;
                    
                    return *this;
                }

                if (node_->parent_ == nullptr)
                {
                    return *this;
                }

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

            Iterator operator++(int)
            {
                Iterator it(*this);
                ++(*this);
                return it;
            }

            Iterator operator--()
            {
                if (*this == tree_->front())
                {
                    *this = tree_->end();
                    return tree_->end();
                }
                
                if (node_->left_ != nullptr)
                {
                    node_ = node_->left_;
                    while (node_->right_ != nullptr)
                        node_ = node_->right_;
                    
                    return *this;
                }

                if (node_->parent_ == nullptr)
                {
                    return *this;
                }

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

            Iterator operator--(int)
            {
                Iterator it(*this);
                --(*this);
                return it;
            }

            KeyT operator*()
            {
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
            delete root_;
        }

        void insert(KeyT key)
        {
            if (root_ == nullptr)
            {
                root_ = new Node(key);
                size_++;
            }
            else
            {
                if (root_->insert(key))
                {
                    size_++;
                }
            }
        }

        Iterator lower_bound(KeyT key) const
        {
            Iterator it = begin();
            Iterator end_it = end();

            for (; it != end_it && *it <= key; ++it) ;

            if (it == end_it)
                return end();

            return --it;
        }

        Iterator upper_bound(KeyT key) const
        {
            Iterator it = back();
            Iterator end_it = end();

            for (; it != end_it && *it >= key; --it) ;

            if (it == end_it)
                return end();

            return ++it;
        }

        size_t distance(Iterator it1, Iterator it2) const
        {
            if (it1 == it2)
                return 0;

            size_t count = 1;
            for (; it1 != it2; ++it1, ++count) ;

            return count;
        }

        size_t get_num_elems_from_diapason(KeyT key1, KeyT key2) const
        {
            if (key1 > key2 || root_ == nullptr)
                return 0;
        
            Iterator it1 = lower_bound(key1);
            Iterator it2 = upper_bound(key2);
            
            return distance(it1, it2);
        }

        Iterator front() const
        {
            if (root_ == nullptr)
                return Iterator{nullptr, this};

            Node<KeyT>* current = root_;
            while (current->left_ != nullptr)
                current = current->left_;

            return Iterator{current, this};
        }

        Iterator back() const
        {
            if (root_ == nullptr)
                return Iterator{nullptr, this};
            
            Node<KeyT>* current = root_;

            while (current->right_ != nullptr)
                current = current->right_;

            return Iterator{current, this};
        }

        Iterator begin() const
        {
            return front();
        }

        Iterator end() const
        {
            return Iterator{nullptr, this};
        }

    private:
        size_t height_ = 0;
        size_t size_ = 0;
        Node<KeyT>* root_ = nullptr;
    }; // class AVL tree 

} // namespace tree