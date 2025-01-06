#include <iostream>
#include <vector>
#include <numeric> 
#include "framework/framework.hpp"
#include "algorithms/algorithms.hpp"
template <typename IterContainer>
void test_hl_dynamic(IterContainer A, IterContainer B, const IterContainer& expected) {
    // Run hl_dynamic
    IterContainer result_hl_dynamic = hl_dynamic(A, B);

    // Print the input and output for clarity
    std::cout << "A: ";
    for (const auto& a : A) std::cout << a << " ";
    std::cout << "\nB: ";
    for (const auto& b : B) std::cout << b << " ";
    std::cout << "\nResult: ";
    for (const auto& r : result_hl_dynamic) std::cout << r << " ";
    std::cout << "\nExpected: ";
    for (const auto& e : expected) std::cout << e << " ";

    // Check correctness
    if (result_hl_dynamic == expected) {
        std::cout << "\nTest passed ✅\n" << std::endl;
    } else {
        std::cout << "\nTest failed ❌\n" << std::endl;
    }
}


int main() {
    int size_a = 100; // Size of first sorted vector
    int size_b = 10000000; // Size of second sorted vector

    std::vector<int> a;
    std::vector<int> b;
    std::vector<int> result;

    generate_sorted_vectors(size_a, size_b, a, b, result);

    // Optionally print some elements of the merged result for verification
    std::cout << "First 10 elements of merged vector: ";
    for (int i = 0; i < 10 && i < result.size(); ++i) {
        std::cout << result[i] << " ";
    }
    std::cout << "\n" << std::endl;

    // Tape Merge
    std::cout << "Running tape_merge" << std::endl;
    std::vector<int> result_ape_merge = tape_merge(a, b);
    
    if (result == result_ape_merge) {
        std::cout << "tape_merge is correct\n" << std::endl;
    } else {
        std::cout << "tape_merge is incorrect\n" << std::endl;
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

    if (result.size() != result_hl_dynamic.size()) {
        std::cout << "Sizes differ. Cannot compare completely.\n";
    } else {
        for (size_t i = 0; i < result.size(); ++i) {
            if (result[i] != result_hl_dynamic[i]) {
                std::cout << "First mismatch at index " << i << ":\n";
                size_t start = (i > 5) ? i - 5 : 0;
                size_t end = std::min(i + 5, result.size());
                for (size_t j = start; j < end; ++j) {
                    std::cout << "Index " << j << ": Expected = " << result[j] << ", Actual = " << result_hl_dynamic[j] << std::endl;
                }
                break;
            }
        }
    }

    return 0;
}