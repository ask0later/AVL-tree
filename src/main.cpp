#include <iostream>
#include <vector>
#include <cassert>

#include "tree.hpp"
#include "input_output.hpp"


int main()
{
    trees::AVLtree<int> tree;
    std::vector<size_t> answer_tree;

    auto distance = [](trees::AVLtree<int> &tree, int key1, int key2)
                    {return tree.get_num_elems_from_diapason_fast(key1, key2);};
    bool result = io::process_input<int, trees::AVLtree<int>>(tree, answer_tree, distance, std::cin);

    if (!result)
    {
        std::cout << "Incorrect input" << std::endl;
        return 1;
    }

    io::print_answers(answer_tree);

    return 0;
}