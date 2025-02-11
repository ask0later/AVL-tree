#pragma once

#include <iostream>
#include <variant>
#include <vector>

namespace query {
template <typename KeyT> class Key final {
public:
    Key() {}
    Key(KeyT key) : key_(key) {}

public:
    KeyT key_;
};

template <typename KeyT> class Request final {
public:
    Request() {}
    Request(KeyT key1, KeyT key2) : key1_(key1), key2_(key2) {}

public:
    KeyT key1_;
    KeyT key2_;
};

template <typename KeyT> using Query = std::variant<Key<KeyT>, Request<KeyT>>;

template <typename KeyT>
bool process_input(std::vector<Query<KeyT>> &queries, std::istream &in) {
    char command = 0;
    KeyT temp1{};
    KeyT temp2{};

    while (true) {
        in >> command;

        if (in.eof())
            return true;

        if (command == 'k') {
            in >> temp1;
            if (!in.good())
                return false;

            Query<KeyT> v;
            v.template emplace<Key<KeyT>>(temp1);
            queries.push_back(std::move(v));
        } else if (command == 'q') {
            in >> temp1 >> temp2;
            if (!in.good())
                return false;

            Query<KeyT> v;
            v.template emplace<Request<KeyT>>(temp1, temp2);
            queries.push_back(std::move(v));
        } else {
            return false;
        }
    }
}

template <typename KeyT, typename TreeT, typename DistanceT>
struct CallQueryProcess final {
    void operator()(const Key<KeyT> &key) { tree.insert(key.key_); }

    void operator()(const Request<KeyT> &request) {
        answers.push_back(distance(tree, request.key1_, request.key2_));
    }

    TreeT &tree;
    std::vector<size_t> &answers;
    DistanceT distance;
};

template <typename KeyT, typename TreeT, typename DistanceT, typename Iter>
std::vector<size_t> get_answers(TreeT &tree, Iter begin, Iter end,
                                DistanceT distance) {
    std::vector<size_t> answers;
    for (auto it = begin; it != end; ++it) {
        std::visit(
            CallQueryProcess<KeyT, TreeT, DistanceT>{tree, answers, distance},
            *it);
    }
    return answers;
}

void print_answers(std::vector<size_t> &answer_tree) {
    for (auto &elem : answer_tree) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}
} // namespace query
