#include "tree.hpp"
#include <gtest/gtest.h>

TEST(TREE_TESTS, ctor1) {
    trees::AVLtree<int> tree{1};
    tree.insert(3);
    tree.insert(2);
    tree.insert(5);

    ASSERT_EQ(tree.get_num_elems_from_diapason(1, 5), 4);
}

TEST(TREE_TESTS, get_num_elems_from_diapason_1) {
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

TEST(TREE_TESTS, get_num_elems_from_diapason_2) {
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

TEST(TREE_TESTS, get_num_elems_from_diapason_3) {
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

TEST(TREE_TESTS, get_num_elems_from_diapason_4) {
    trees::AVLtree<int> tree;
    for (int i = 0; i < 100; i++)
        tree.insert(i);

    ASSERT_EQ(tree.get_num_elems_from_diapason(5, 48), 44);
}

TEST(TREE_TESTS, get_num_elems_from_diapason_5) {
    trees::AVLtree<long long> tree;
    for (int i = 0; i < 10000; i++)
        tree.insert(i);

    ASSERT_EQ(tree.get_num_elems_from_diapason(1000, 2050), 1051);
}

TEST(TREE_TESTS, iterator_1) {
    trees::AVLtree<int> tree;
    for (int i = 0; i < 1000; i++)
        tree.insert(i);

    int i = 0;
    for (auto it = tree.begin(); it != tree.end(); ++it) {
        ASSERT_EQ(i, *it);
        i++;
    }
}

TEST(TREE_TESTS, iterator_2) {
    trees::AVLtree<int> tree;
    for (int i = 0; i < 1000; i++)
        tree.insert(i);

    int i = 0;
    for (auto it = tree.begin(); it != tree.end(); ++it) {
        ASSERT_EQ(i, *it);
        i++;
    }
}

TEST(TREE_TESTS, iterator_3) {
    struct Person {
        std::string name_;
        int age_;

        bool operator<(const Person& other) const {
            return age_ < other.age_;
        }

        bool operator==(const Person& other) const {
            return age_ == other.age_;
        }

        bool operator<=(const Person& other) const {
            return *this < other || *this == other;
        }

        bool operator!=(const Person& other) const {
            return !(*this == other);
        }

        bool operator>(const Person& other) const {
            return !(*this < other) && *this != other;
        }

        bool operator>=(const Person& other) const {
            return *this > other || *this == other;
        }
    };

    trees::AVLtree<Person> tree;
    for (int i = 0; i < 1000; i++)
    {
        Person pers{"Oleg" + std::to_string(i), i};
        tree.insert(pers);
    }

    int i = 0;
    for (auto it = tree.begin(); it != tree.end(); ++it) {
        ASSERT_EQ("Oleg" + std::to_string(i), it->name_);
        i++;
    }
}

TEST(TREE_TESTS, iterator_4) {
    trees::AVLtree<int> tree;
    for (int i = 0; i < 1000; i++)
        tree.insert(i);

    int i = 0;
    for (auto it : tree) {
        ASSERT_EQ(i, it);
        i++;
    }
}

TEST(TREE_TESTS, iterator_5) {
    trees::AVLtree<int> tree;
    for (int i = 0; i < 1000; i++)
        tree.insert(i);

    auto it1 = tree.begin();
    auto it2 = tree.end();

    std::advance(it1, 5);
    ASSERT_EQ(std::distance(it1, it2), 1000 - 6);
}

TEST(TREE_TESTS, get_num_elems_from_diapason_6) {
    trees::AVLtree<int> tree;
    tree.insert(1);
    tree.insert(2);
    tree.insert(9);

    ASSERT_EQ(tree.get_num_elems_from_diapason(1, 3), 2);

    tree.insert(3);
    ASSERT_EQ(tree.get_num_elems_from_diapason(1, 3), 3);
}

TEST(TREE_TESTS, get_num_elems_from_diapason_7) {
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

TEST(TREE_TESTS, get_num_elems_from_diapason_8) {
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

TEST(TREE_TESTS, copy_semantic) {
    trees::AVLtree<int> tree1;
    tree1.insert(1);
    tree1.insert(2);
    tree1.insert(3);
    tree1.insert(4);
    tree1.insert(5);

    ASSERT_EQ(tree1.get_num_elems_from_diapason(1, 5), 5);

    auto tree2{tree1};
    tree2.insert(6);
    ASSERT_EQ(tree2.get_num_elems_from_diapason(1, 6), 6);

    trees::AVLtree<int> tree3;
    tree3.insert(6);
    tree3 = tree1;
    ASSERT_EQ(tree3.get_num_elems_from_diapason(1, 6), 5);
}

TEST(TREE_TESTS, move_semantic) {
    trees::AVLtree<int> tree1;
    tree1.insert(1);
    tree1.insert(2);
    tree1.insert(3);
    tree1.insert(4);
    tree1.insert(5);

    ASSERT_EQ(tree1.get_num_elems_from_diapason(1, 5), 5);

    auto tree2{std::move(tree1)};
    ASSERT_EQ(tree2.get_num_elems_from_diapason(1, 5), 5);
    trees::AVLtree<int> tree3;
    tree3.insert(6);
    tree3 = std::move(tree2);
    ASSERT_EQ(tree3.get_num_elems_from_diapason(1, 6), 5);
}