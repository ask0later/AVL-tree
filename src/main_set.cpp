#include <cassert>
#include <iostream>
#include <set>
#include <vector>

#include "process_queries.hpp"

int main() {
    std::vector<query::Query<int>> queries;
    bool result = query::process_input<int>(queries, std::cin);
    if (!result) {
        std::cout << "Incorrect input" << std::endl;
        return 1;
    }

    try {
        std::set<int> tree;
        auto distance = [](std::set<int> &tree, int key1, int key2) {
            return std::distance(tree.lower_bound(key1),
                                 tree.upper_bound(key2));
        };

        std::vector<size_t> answer_tree = query::get_answers<int>(
            tree, queries.begin(), queries.end(), distance);

        query::print_answers(answer_tree);
    } catch (std::out_of_range &out_of_range_ex) {
        std::cout << "Out of Range error: " << std::endl
                  << out_of_range_ex.what() << std::endl;
    } catch (std::logic_error &logic_ex) {
        std::cout << "Logic error: " << std::endl
                  << logic_ex.what() << std::endl;
    } catch (std::bad_alloc &bad_alloc_ex) {
        std::cout << "Bad alloc error: " << std::endl
                  << bad_alloc_ex.what() << std::endl;
    } catch (std::exception &ex) {
        std::cout << "Exception: " << std::endl << ex.what() << std::endl;
    }

    return 0;
}