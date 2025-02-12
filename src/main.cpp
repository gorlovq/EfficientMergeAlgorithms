#include <iostream>
#include <vector>
#include "framework/framework.hpp"
#include "algorithms/algorithms.hpp"

int main() {
    int size_a = 100; 
    int size_b = 10000000;

    MergeTestCase test_case = generate_sorted_vectors(size_a, size_b, CornerCaseType::RANDOM);

    std::vector<int> a = test_case.a;
    std::vector<int> b = test_case.b;
    std::vector<int> result = test_case.result;

    std::cout << "Size of generated a vector is " << a.size() << std::endl;
    std::cout << "Size of generated b vector is " << b.size() << std::endl;
    std::cout << "Size of merged vector is " << result.size() << std::endl << std::endl;

    // Two way merge
    std::cout << "Running two_way_merge" << std::endl;
    std::vector<int> result_two_way_merge = two_way_merge(a, b);
    
    if (result == result_two_way_merge) {
        std::cout << "two_way_merge is correct\n" << std::endl;
    } else {
        std::cout << "two_way_merge is incorrect\n" << std::endl;
    }

    // Simple binary merge
    std::cout << "Running simple_binary_merge" << std::endl;
    std::vector<int> a_copy = a;
    std::vector<int> b_copy = b;
    std::vector<int> result_simple_binary_merge = simple_binary_merge(a_copy, b_copy);
    
    if (result == result_simple_binary_merge) {
        std::cout << "simple_binary_merge is correct\n" << std::endl;
    } else {
        std::cout << "simple_binary_merge is incorrect\n" << std::endl;
    }

    // Hwang-Lin Merging Algorithm
    std::cout << "Running hla" << std::endl;
    std::vector<int> result_hla = hla(a, b);

    if (result == result_hla) {
        std::cout << "hla is correct\n" << std::endl;
    } else {
        std::cout << "hla is incorrect\n" << std::endl;
    }

    // Fractile Insertion
    std::cout << "Running fractile_insertion" << std::endl;
    std::vector<int> result_fractile_insertion = fractile_insertion(a, b);

    if (result == result_fractile_insertion) {
        std::cout << "fractile_insertion is correct\n" << std::endl;
    } else {
        std::cout << "fractile_insertion is incorrect\n" << std::endl;
    }

    // Binary Merge
    // std::cout << "Running binary_merge" << std::endl;
    // std::vector<int> result_binary_merge = binary_merge(a, b);

    // if (result == result_binary_merge) {
    //     std::cout << "binary_merge is correct\n" << std::endl;
    // } else {
    //     std::cout << "binary_merge is incorrect\n" << std::endl;
    // }


    // Hwang-Lin Static
    std::cout << "Running hl_static" << std::endl;
    std::vector<int> a_copy_hl = a;
    std::vector<int> b_copy_hl = b;
    std::vector<int> result_hl_static = hl_static(a_copy_hl, b_copy_hl);

    if (result == result_hl_static) {
        std::cout << "hl_static is correct\n" << std::endl;
    } else {
        std::cout << "hl_static is incorrect\n" << std::endl;
    }


    // Hwang-Lin dynamic
    std::cout << "Running hl_dynamic" << std::endl;
    std::vector<int> a_copy_hl_2 = a;
    std::vector<int> b_copy_hl_2 = b;

    std::vector<int> result_hl_dynamic = hl_dynamic(a, b);

    if (result == result_hl_dynamic) {
        std::cout << "hl_dynamic is correct\n" << std::endl;
    } else {
        std::cout << "hl_dynamic is incorrect\n" << std::endl;
    }

    return 0;
}