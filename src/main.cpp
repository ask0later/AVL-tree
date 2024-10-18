#include <iostream>
#include <set>
#include <cassert>

#include "tree.hpp"

namespace
{
    template <typename KeyT>
    void process_input(trees::AVLtree<KeyT> &tree)
    {
        /// TODO
    }

    template <typename T>
    int range_query(const std::set<T>& s, T fst, T snd)
    {
        using itt = typename std::set<T>::iterator;
        itt start = s.lower_bound(fst);
        itt fin = s.upper_bound(snd);
        return std::distance(start, fin);
    }
}

int main()
{
    trees::AVLtree<int> tree;
    tree.insert(4);
    tree.insert(2);
    tree.insert(3);
    tree.insert(2);
    tree.insert(6);
    tree.insert(1);
    tree.insert(5);
    tree.insert(0);
    tree.insert(8);

    std::cout << tree.get_num_elems_from_diapason(2, 8) << std::endl;

}