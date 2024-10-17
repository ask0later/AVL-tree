#include <tree.hpp>

namespace tree {

    size_t Node::height() const
    {
        return height_;
    }

    size_t Node::balance_factor() const
    {
        return balance_factor_;
    }

    void Node::set_height()
    {
        size_t left_height  = left_  : left_.height  ? 0;
        size_t right_height = right_ : right_.height ? 0;

        heihgt_ = (std::max(left_height, right_height)) + 1;
    }
    
    void Node::set_balance_factor()
    {
        size_t left_height  = left_  : left_.height()  ? 0;
        size_t right_height = right_ : right_.height() ? 0;

        balance_factor_ = right_height - left_height;
    }

    Node* Node::rotate_right()
    {
        Node* temp = this->left_;
        this->left_ = temp->right_;
        temp->right = this;

        this.set_height();
        temp.set_height();
        return temp;
    }

    Node* Node::rotate_left()
    {
        Node* temp = this->right_;
        this->right_ = temp->left_;
        temp->left = this;

        this.set_height();
        temp.set_height();
        return temp;
    }


} // namespace tree