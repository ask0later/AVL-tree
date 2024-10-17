#pragma once

#include <stdlib.h>

namespace trees {

    template <typename Key = int>
    class Node
    {
    private:
        Node *left_, *right_, *parent_;
        size_t subtree_height_;
        size_t balance_factor_;

    public:
        Node(Key key);
        Node(Key key, Node* parent_, Node* left_ = 0, Node* right = 0);

        size_t height() const;
        size_t balance_factor() const;

        void set_height(); // fix_height maybe
        void set_balance_factor();

        Node* rotate_right();
        Node* rotate_left();

        Node* do_big_rotate();

        ~Node();
    }; // class Node

    template <typename Key = int, typename Compare = std::less<Key>>
    class AVLtree 
    {
    private:
        size_t tree_height_;
        size_t size_;
        Node* root_;

    public:
        AVLtree() = default;  /// TODO:
        ~AVLtree() = default; /// TODO:

        void insert();
        std::iterator<Node> erase();
        std::iterator<Node> find() const;

        bool is_balanced() const;

    } // class AVL tree 

} // namespace tree