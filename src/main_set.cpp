#include <iostream>
#include <vector>
#include <cassert>
#include <set>

#include "input_output.hpp"

int main()
{
    std::set<int> tree;
    std::vector<size_t> answer_tree;

    auto distance = [](std::set<int> &tree, int key1, int key2)
                    {return std::distance(tree.lower_bound(key1), tree.upper_bound(key2));};
    bool result = io::process_input<int, std::set<int>>(tree, answer_tree, distance, std::cin);

    if (!result)
    {
        std::cout << "Incorrect input" << std::endl;
        return 1;
    }

    io::print_answers(answer_tree);

    return 0;
}