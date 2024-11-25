#include <iostream>
#include <vector>
#include <cassert>

#include "tree.hpp"
#include "process_queries.hpp"

int main()
{
    std::vector<query::Query<int>> queries;
    bool result = query::process_input<int>(queries, std::cin);
    if (!result)
    {
        std::cout << "Incorrect input" << std::endl;
        return 1;
    }

    trees::AVLtree<int> tree;

    auto distance = [](trees::AVLtree<int> &tree, int key1, int key2)
                    {return tree.get_num_elems_from_diapason_fast(key1, key2);};

    std::vector<size_t> answer_tree = query::get_answers<int>(tree, queries, distance);

    query::print_answers(answer_tree);

    return 0;
}