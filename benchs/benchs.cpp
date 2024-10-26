#include <cassert>
#include <iostream>
#include <set>
#include <vector>

#include "tree.hpp"
#include "hayai.hpp"
#include "hayai_main.hpp"

namespace
{

template <typename KeyT>
void process_input(trees::AVLtree<KeyT> &tree, std::vector<size_t> &answer_tree)
{
    char command = 0;
    KeyT temp1{};
    KeyT temp2{};

    std::ifstream in("benchs/data.dat");
    assert(in.is_open());

    while (1)
    {
        in >> command;

        if (in.eof())
            return;

        if (command == 'k')
        {
            in >> temp1;
            assert(in.good());

            tree.insert(temp1);
        }
        else if (command == 'q')
        {
            in >> temp1 >> temp2;
            assert(in.good());

            answer_tree.push_back(tree.get_num_elems_from_diapason(temp1, temp2));
        }
    }
}

template <typename KeyT>
void process_input(std::set<KeyT> &tree, std::vector<size_t> &answer_tree)
{
    char command = 0;
    KeyT temp1{};
    KeyT temp2{};

    std::ifstream in("benchs/data.dat");
    assert(in.is_open());

    while (1)
    {
        in >> command;

        if (in.eof())
            return;

        if (command == 'k')
        {
            in >> temp1;
            assert(in.good());

            tree.insert(temp1);
        }
        else if (command == 'q')
        {
            in >> temp1 >> temp2;
            assert(in.good());

            answer_tree.push_back(std::distance(tree.lower_bound(temp1), tree.upper_bound(temp2)));
        }
    }
}

} // namespace

BENCHMARK(NumKeysFromDiapason, BenchSet, 1, 1) {
    std::set<int> tree;
    std::vector<size_t> answer_tree;

    process_input(tree, answer_tree);
}

BENCHMARK(NumKeysFromDiapason, BenchOurTree, 1, 1) {
    trees::AVLtree<int> tree;
    std::vector<size_t> answer_tree;

    process_input(tree, answer_tree);
}

int main()
{
    srandom(time(NULL));
    hayai::MainRunner runner;
    return runner.Run();
}