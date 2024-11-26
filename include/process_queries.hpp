#pragma once

#include <iostream>
#include <vector>
#include <variant>

namespace query
{
    template <typename KeyT>
    class Key
    {
    public:
        Key(KeyT key) : key_(key) {}

    public:
        KeyT key_;
    };

    template <typename KeyT>
    class Request
    {
    public:
        Request(KeyT key1, KeyT key2) : key1_(key1), key2_(key2) {}

    public:
        KeyT key1_;
        KeyT key2_;
    };

    template <typename KeyT>
    using Query = std::variant<Key<KeyT>, Request<KeyT>>;
    

    template <typename KeyT>
    bool process_input(std::vector<Query<KeyT>> &queries, std::istream& in)
    {
        char command = 0;
        KeyT temp1{};
        KeyT temp2{};

        while (1)
        {
            in >> command;

            if (in.eof())
                return true;

            if (command == 'k')
            {
                in >> temp1;
                if (!in.good())
                    return false;

                queries.push_back(Key(temp1));
            }
            else if (command == 'q')
            {
                in >> temp1 >> temp2;
                if (!in.good())
                    return false;

                queries.push_back(Request(temp1, temp2));
            }
            else
            {
                return false;
            }
        }
    }

    template <typename KeyT, typename TreeT, typename DistanceT>
    struct CallQueryProcess
    {
        void operator() (const Key<KeyT> &key)
        {
            tree.insert(key.key_);
        }

        void operator() (const Request<KeyT> &request)
        {
            answers.push_back(distance(tree, request.key1_, request.key2_));
        }

        TreeT &tree;
        std::vector<size_t> &answers;
        DistanceT distance;
    };

    template <typename KeyT, typename TreeT, typename DistanceT>
    std::vector<size_t> get_answers(TreeT &tree, std::vector<Query<KeyT>> &queries, DistanceT distance)
    {
        std::vector<size_t> answers;
        for (auto& query : queries)
        {
            std::visit(CallQueryProcess<KeyT, TreeT, DistanceT>{tree, answers, distance}, query);
        }
        return answers;
    }

    void print_answers(std::vector<size_t> &answer_tree)
    {
        for (auto &elem : answer_tree)
        {
            std::cout << elem << " ";
        }
        std::cout << std::endl;
    }
} // namespace input

