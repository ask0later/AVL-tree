#include <cassert>
#include <iostream>
#include <set>
#include <vector>

#include "hayai.hpp"
#include "hayai_main.hpp"
#include "process_queries.hpp"
#include "tree.hpp"

std::vector<query::Query<int>> queries;

BENCHMARK(NumKeysFromDiapason, BenchOurTree, 1, 1) {
    trees::AVLtree<int> tree;

    auto distance = [](trees::AVLtree<int> &tree, int key1, int key2) {
        return tree.get_num_elems_from_diapason(key1, key2);
    };

    std::vector<size_t> answer_tree =
        query::get_answers<int>(tree, queries, distance);
}

BENCHMARK(NumKeysFromDiapason, BenchSet, 1, 1) {
    std::set<int> tree;

    auto distance = [](std::set<int> &tree, int key1, int key2) {
        return std::distance(tree.lower_bound(key1), tree.upper_bound(key2));
    };

    std::vector<size_t> answer_tree =
        query::get_answers<int>(tree, queries, distance);
}

int main() {
    std::ifstream in("benchs/data.dat");
    bool result = query::process_input<int>(queries, in);
    if (!result) {
        std::cout << "Incorrect input" << std::endl;
        return 1;
    }
    in.close();

    hayai::MainRunner runner;
    return runner.Run();
}