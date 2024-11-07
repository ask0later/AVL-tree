#include <iostream>
#include <vector>
#include <cassert>

#include "tree.hpp"

namespace
{
    template <typename KeyT>
    void process_input(trees::AVLtree<KeyT> &tree, std::vector<size_t> &answer_tree)
    {
        char command = 0;
        KeyT temp1{};
        KeyT temp2{};

        while (1)
        {
            std::cin >> command;

            if (std::cin.eof())
                return;

            if (command == 'k')
            {
                std::cin >> temp1;
                assert(std::cin.good());

                tree.insert(temp1);
            }
            else if (command == 'q')
            {
                std::cin >> temp1 >> temp2;
                assert(std::cin.good());

                answer_tree.push_back(tree.get_num_elems_from_diapason(temp1, temp2));
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
}

int main()
{
    trees::AVLtree<int> tree;
    std::vector<size_t> answer_tree;

    process_input(tree, answer_tree);
    print_answers(answer_tree);

    return 0;
}