#include <cassert>
#include <iostream>
#include <set>
#include <vector>

#include "tree.hpp"
#include "input_output.hpp"
#include "hayai.hpp"
#include "hayai_main.hpp"

BENCHMARK(NumKeysFromDiapason, BenchOurTree, 1, 1) {
    trees::AVLtree<int> tree;
    std::vector<size_t> answer_tree;
    std::ifstream in("benchs/data.dat");

    auto distance = [](trees::AVLtree<int> &tree, int key1, int key2)
                    {return tree.get_num_elems_from_diapason_fast(key1, key2);};
    bool result = io::process_input<int, trees::AVLtree<int>>(tree, answer_tree, distance, in);
    in.close();

    if (!result)
    {
        std::cout << "Incorrect input" << std::endl;
        abort();
    }
}

BENCHMARK(NumKeysFromDiapason, BenchSet, 1, 1) {
    std::set<int> tree;
    std::vector<size_t> answer_tree;
    std::ifstream in("benchs/data.dat");

    auto distance = [](std::set<int> &tree, int key1, int key2)
                    {return std::distance(tree.lower_bound(key1), tree.upper_bound(key2));};
    bool result = io::process_input<int, std::set<int>>(tree, answer_tree, distance, in);
    in.close();

    if (!result)
    {
        std::cout << "Incorrect input" << std::endl;
        abort;
    }
}

int main()
{
    hayai::MainRunner runner;
    return runner.Run();
}