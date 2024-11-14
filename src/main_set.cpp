#include <iostream>
#include <vector>
#include <cassert>
#include <set>

namespace
{
    template <typename KeyT>
    bool process_input(std::set<KeyT> &tree, std::vector<size_t> &answer_tree)
    {
        char command = 0;
        KeyT temp1{};
        KeyT temp2{};

        while (1)
        {
            std::cin >> command;

            if (std::cin.eof())
                return true;

            if (command == 'k')
            {
                std::cin >> temp1;
                if (!std::cin.good())
                    return false;

                tree.insert(temp1);
            }
            else if (command == 'q')
            {
                std::cin >> temp1 >> temp2;
                if (!std::cin.good())
                    return false;

                answer_tree.push_back(std::distance(tree.lower_bound(temp1), tree.upper_bound(temp2)));
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
}

int main()
{
    std::set<int> tree;
    std::vector<size_t> answer_tree;

    if (!process_input(tree, answer_tree))
    {
        std::cout << "Incorrect input" << std::endl;
        return 1;
    }
    print_answers(answer_tree);

    return 0;
}