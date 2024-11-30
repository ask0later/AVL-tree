#include "tree.hpp"
#include <gtest/gtest.h>

TEST(TREE_TESTS, test_1) {
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

TEST(TREE_TESTS, test_2) {
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

TEST(TREE_TESTS, test_3) {
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

TEST(TREE_TESTS, test_4) {
    trees::AVLtree<int> tree;
    for (int i = 0; i < 100; i++)
        tree.insert(i);

    ASSERT_EQ(tree.get_num_elems_from_diapason(5, 48), 44);
}

TEST(TREE_TESTS, test_5) {
    trees::AVLtree<long long> tree;
    for (int i = 0; i < 10000; i++)
        tree.insert(i);

    ASSERT_EQ(tree.get_num_elems_from_diapason(1000, 2050), 1051);
}

TEST(TREE_TESTS, test_6) {
    trees::AVLtree<int> tree;
    for (int i = 0; i < 1000; i++)
        tree.insert(i);

    int i = 0;
    for (auto it = tree.begin(); it != tree.end(); ++it) {
        ASSERT_EQ(i, *it);
        i++;
    }
}

TEST(TREE_TESTS, test_7) {
    trees::AVLtree<int> tree;
    for (int i = 0; i < 1000; i++)
        tree.insert(i);

    int i = 0;
    for (auto it = tree.begin(); it != tree.end(); ++it) {
        ASSERT_EQ(i, *it);
        i++;
    }
}

TEST(TREE_TESTS, test_8) {
    trees::AVLtree<int> tree;
    tree.insert(1);
    tree.insert(2);
    tree.insert(9);

    ASSERT_EQ(tree.get_num_elems_from_diapason(1, 3), 2);

    tree.insert(3);
    ASSERT_EQ(tree.get_num_elems_from_diapason(1, 3), 3);
}

TEST(TREE_TESTS, test_9) {
    trees::AVLtree<int> tree;
    tree.insert(1);
    tree.insert(2);
    tree.insert(3);
    tree.insert(5);
    tree.insert(7);
    tree.insert(9);
    tree.insert(8);

    ASSERT_EQ(tree.get_num_elems_from_diapason(2, 3), 2);
    ASSERT_EQ(tree.get_num_elems_from_diapason(2, 5), 3);
    ASSERT_EQ(tree.get_num_elems_from_diapason(2, 7), 4);
    ASSERT_EQ(tree.get_num_elems_from_diapason(2, 8), 5);
    ASSERT_EQ(tree.get_num_elems_from_diapason(2, 9), 6);
    ASSERT_EQ(tree.get_num_elems_from_diapason(1, 9), 7);
    ASSERT_EQ(tree.get_num_elems_from_diapason(-1, 9), 7);
    ASSERT_EQ(tree.get_num_elems_from_diapason(-1, 7), 5);
    ASSERT_EQ(tree.get_num_elems_from_diapason(-1, 0), 0);
    ASSERT_EQ(tree.get_num_elems_from_diapason(15, 19), 0);
    ASSERT_EQ(tree.get_num_elems_from_diapason(8, 5), 0);
}

TEST(TREE_TESTS, test_10) {
    trees::AVLtree<int> tree;
    tree.insert(1);
    tree.insert(2);
    tree.insert(3);
    tree.insert(5);
    tree.insert(7);
    tree.insert(9);
    tree.insert(8);
    tree.insert(12);
    tree.insert(-3);
    tree.insert(15);
    tree.insert(10);
    tree.insert(-5);
    tree.insert(13);
    tree.insert(0);

    ASSERT_EQ(tree.get_num_elems_from_diapason(2, 10), 7);
    ASSERT_EQ(tree.get_num_elems_from_diapason(-4, 14), 12);
    ASSERT_EQ(tree.get_num_elems_from_diapason(-4, 25), 13);
    ASSERT_EQ(tree.get_num_elems_from_diapason(-10, 30), 14);
    ASSERT_EQ(tree.get_num_elems_from_diapason(4, 6), 1);
    ASSERT_EQ(tree.get_num_elems_from_diapason(4, 11), 5);
    ASSERT_EQ(tree.get_num_elems_from_diapason(12, 15), 3);
    ASSERT_EQ(tree.get_num_elems_from_diapason(8, 10), 3);
    ASSERT_EQ(tree.get_num_elems_from_diapason(12, 13), 2);
}