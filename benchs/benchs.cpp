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
bool process_input(trees::AVLtree<KeyT> &tree, std::vector<size_t> &answer_tree)
{
    char command = 0;
    KeyT temp1{};
    KeyT temp2{};

    std::ifstream in("benchs/data.dat");
    if (!in.is_open())
        return false;

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

            answer_tree.push_back(tree.get_num_elems_from_diapason(temp1, temp2));
        }
        else
        {
            return false;
        }
    }
}

template <typename KeyT>
bool process_input(std::set<KeyT> &tree, std::vector<size_t> &answer_tree)
{
    char command = 0;
    KeyT temp1{};
    KeyT temp2{};

    std::ifstream in("benchs/data.dat");
    if (!in.is_open())
        return false;

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

            answer_tree.push_back(std::distance(tree.lower_bound(temp1), tree.upper_bound(temp2)));
        }
        else
        {
            return false;
        }
    }
}

} // namespace

BENCHMARK(NumKeysFromDiapason, BenchOurTree, 1, 1) {
    trees::AVLtree<int> tree;
    std::vector<size_t> answer_tree;

    if (!process_input(tree, answer_tree))
    {
        std::cout << "Incorrect input" << std::endl;
        abort();
    }
}

BENCHMARK(NumKeysFromDiapason, BenchSet, 1, 1) {
    std::set<int> tree;
    std::vector<size_t> answer_tree;

    if (!process_input(tree, answer_tree))
    {
        std::cout << "Incorrect input" << std::endl;
        abort();
    }
}

int main()
{
    hayai::MainRunner runner;
    return runner.Run();
}