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

        using IterT = Node*;

        IterT left_ = nullptr;
        IterT right_ = nullptr;
        IterT parent_ = nullptr;
        KeyT key_; 

        Node(KeyT key) : key_(key) {}
        Node(KeyT key, IterT parent, IterT left = nullptr, IterT right = nullptr) :
            key_(key), parent_(parent), left_(left), right_(right) {}

        ~Node()
        {
            delete left_;
            delete right_;
        }

        IterT lower_bound(KeyT key)
        {
            if (key_ == key)
                return this;
            
            if (key_ > key)
            {
                if (left_ == nullptr)
                    return this;

                return left_->lower_bound(key);
            }

            if (key_ < key)
            {
                if (right_ == nullptr)
                    return this;

                if (right_->key_ <= key)
                    return right_->lower_bound(key);
                else
                    return this;
            }
        }

        IterT upper_bound(KeyT key)
        {
            if (key_ == key)
                return this;
            
            if (key_ < key)
            {
                if (right_ == nullptr)
                    return this;

                return right_->upper_bound(key);
            }

            if (key_ > key)
            {
                if (left_ == nullptr)
                    return this;

                if (left_->key_ >= key)
                    return left_->upper_bound(key);
                else
                    return this;
            }
        }

        void insert(KeyT key)
        {
            if (key == key_)
                return;
            
            if (key < key_) // left
            {
                if (left_ == nullptr)
                {
                    left_ = new Node(key);
                    return;
                }
                else
                {
                    left_->insert(key);
                }
            }
            else // right
            {
                if (right_ == nullptr)
                {
                    right_ = new Node(key);
                    return;
                }
                else
                {
                    right_->insert(key);
                }
            }
        }

    }; // class Node

    template <typename KeyT = int, typename Compare = std::less<KeyT>>
    class AVLtree 
    {
        using NodeT = Node<KeyT>;
        using IterT = NodeT*;

        size_t height_ = 0;
        size_t size_ = 0;
        IterT root_ = nullptr;

        IterT lower_bound(KeyT key) const
        {
            return root_->lower_bound(key);
        }

        IterT upper_bound(KeyT key) const
        {
            return root_->upper_bound(key);
        }

        size_t distance(IterT it1, IterT it2) const
        {
            if (it1 == it2)
                return 1;
            
            size_t count = 1;
            if (it1->right_ != nullptr)
            {
                count += distance(it1->right_, it2);
            }

            if (it1->parent_ != nullptr)
            {
                count += distance(it1->parent_, it2);
            }
        
            return count;
        }

    public:
        AVLtree() = default;
        AVLtree(KeyT key) : root_(new Node(key)), size_(1), height_(1) {}

        ~AVLtree()
        {   
            delete root_;
        }

        void insert(KeyT key)
        {
            if (root_ == nullptr)
                root_ = new Node(key);
            else
                root_->insert(key);

            size_++;
            height_++;
        }

        size_t get_num_elems_from_diapason(KeyT key1, KeyT key2) const
        {
            if (key1 > key2 || root_ == nullptr)
                return 0;
        
            IterT it1 = lower_bound(key1);
            IterT it2 = upper_bound(key2);

            std::cout << it1->key_ << " " << it2->key_ << std::endl;
            
            return distance(it1, it2);
        }

    }; // class AVL tree 

} // namespace tree