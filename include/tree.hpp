#pragma once

#include <iostream>
#include <functional>


namespace trees {

    template <typename KeyT, typename Compare>
    class AVLtree;


    template <typename KeyT = int>
    class Node
    {
        friend class AVLtree<KeyT, std::less<KeyT>>;

        Node *left_ = nullptr;
        Node *right_ = nullptr;
        Node *parent_ = nullptr;
        KeyT key_; 

        Node(KeyT key) : key_(key) {}
        Node(KeyT key, Node *parent, Node *left = nullptr, Node *right = nullptr) :
            key_(key), parent_(parent), left_(left), right_(right) {}

        ~Node()
        {
            delete left_;
            delete right_;
        }

        size_t num_less_elems(KeyT key)
        {
            if (key_ > key)
                return 0;

            size_t count = key_ < key ? 1 : 0;

            if (left_ != nullptr)
                count += left_->num_less_elems(key);
            if (right_ != nullptr)
                count += right_->num_less_elems(key);

            return count;
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
    private:
        using NodeT = Node<KeyT>;

        size_t height_ = 0;
        size_t size_ = 0;
        NodeT *root_ = nullptr;

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

        size_t num_less_elems(KeyT key) const
        {
            return root_ == nullptr ? 0 : root_->num_less_elems(key);
        }

    }; // class AVL tree 

} // namespace tree