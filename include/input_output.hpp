#pragma once

#include <iostream>
#include <vector>

namespace io
{
    template <typename KeyT, typename TreeT, typename DistanceT>
    bool process_input(TreeT &tree, std::vector<size_t> &answer_tree, 
                       DistanceT distance, std::istream& in)
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

                tree.insert(temp1);
            }
            else if (command == 'q')
            {
                in >> temp1 >> temp2;
                if (!in.good())
                    return false;

                answer_tree.push_back(distance(tree, temp1, temp2));
            }
            else
            {
                return false;
            }
        }
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

