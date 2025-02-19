#include <iostream>
#include <vector>
#include "framework/framework.hpp"
#include "algorithms/algorithms.hpp"

int main() {
    // int size_a = 6; 
    // int size_b = 50;

    // MergeTestCase test_case = generate_sorted_vectors(size_a, size_b, CornerCaseType::RANDOM);

    // std::vector<int> a = test_case.a;
    // std::vector<int> b = test_case.b;
    // std::vector<int> result = test_case.result;

    std::vector<int> a = {505, 1815, 2572, 2637, 7619, 8161};
    std::vector<int> b = {71,252,308,537,758,1004,1070,1311,1499,1740,1751,2099,2487,2573,2642,2669,2714,2931,3095,3202,3271,3448,3830,3843,3989,4294,4631,4686,4829,5095,5429,5437,5569,5637,6001,6035,6060,6254,6442,6787,7400,7850,8441,8602,8829,9140,9231,9505,9674,9975};

    std::vector<int> result = {71,252,308,505,537,758,1004,1070,1311,1499,1740,1751,1815,2099,2487,2572,2573,2637,2642,2669,2714,2931,3095,3202,3271,3448,3830,3843,3989,4294,4631,4686,4829,5095,5429,5437,5569,5637,6001,6035,6060,6254,6442,6787,7400,7619,7850,8161,8441,8602,8829,9140,9231,9505,9674,9975,};

    for (const auto& element : a) {
        std::cout << element << ", ";
    }
    std::cout << std::endl;

    for (const auto& element : b) {
        std::cout << element << ",";
    }

    std::cout << std::endl;

    for (const auto& element : result) {
        std::cout << element << ",";
    }

    std::cout << std::endl;


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


    // Hwang-Lin Merging Algorithm
    std::cout << "Running hla2" << std::endl;
    std::vector<int> result_hla2 = hl_dynamic2(a, b);

    if (result == result_hla2) {
        std::cout << "hla2 is correct\n" << std::endl;
    } else {


        for (const auto& element : result_hla2) {
           std::cout << element << ", ";
        }
        std::cout << std::endl;

        std::cout << "hla2 is incorrect\n" << std::endl;
    }


    // Binary Merge
    // std::cout << "Running binary_merge" << std::endl;
    // std::vector<int> result_binary_merge = binary_merge(a, b);

    // if (result == result_binary_merge) {
    //     std::cout << "binary_merge is correct\n" << std::endl;
    // } else {
    //     std::cout << "binary_merge is incorrect\n" << std::endl;
    // }


    // // Hwang-Lin Static
    // std::cout << "Running hl_static" << std::endl;
    // std::vector<int> a_copy_hl = a;
    // std::vector<int> b_copy_hl = b;
    // std::vector<int> result_hl_static = hl_static(a_copy_hl, b_copy_hl);

    // if (result == result_hl_static) {
    //     std::cout << "hl_static is correct\n" << std::endl;
    // } else {
    //     std::cout << "hl_static is incorrect\n" << std::endl;
    // }


    // // Hwang-Lin dynamic
    // std::cout << "Running hl_dynamic" << std::endl;
    // std::vector<int> a_copy_hl_2 = a;
    // std::vector<int> b_copy_hl_2 = b;

    // std::vector<int> result_hl_dynamic = hl_dynamic(a, b);

    // if (result == result_hl_dynamic) {
    //     std::cout << "hl_dynamic is correct\n" << std::endl;
    // } else {
    //     std::cout << "hl_dynamic is incorrect\n" << std::endl;
    // }

    return 0;
}