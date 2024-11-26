#include <iostream>
#include <vector>
#include <cassert>
#include <set>

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

    std::set<int> tree;

    auto distance = [](std::set<int> &tree, int key1, int key2)
                    {return std::distance(tree.lower_bound(key1), tree.upper_bound(key2));};

    std::vector<size_t> answer_tree = query::get_answers<int>(tree, queries, distance);

    query::print_answers(answer_tree);

    return 0;
}