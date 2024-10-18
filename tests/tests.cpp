#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "tree.hpp"

TEST(TREE_TESTS, test_1)
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

    ASSERT_EQ(tree.get_num_elems_from_diapason(3, 5), 3);
}

TEST(TREE_TESTS, test_2)
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

    ASSERT_EQ(tree.get_num_elems_from_diapason(8, 5), 0);
}

TEST(TREE_TESTS, test_3)
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

    ASSERT_EQ(tree.get_num_elems_from_diapason(10, 12), 0);
}

TEST(TREE_TESTS, test_4)
{
    trees::AVLtree<int> tree;
    for (int i = 0; i < 100; i++)
        tree.insert(i);

    ASSERT_EQ(tree.get_num_elems_from_diapason(5, 48), 44);
}

TEST(TREE_TESTS, test_5)
{
    trees::AVLtree<long long> tree;
    for (int i = 0; i < 10000; i++)
        tree.insert(i);

    ASSERT_EQ(tree.get_num_elems_from_diapason(1000, 2050), 1051);
}