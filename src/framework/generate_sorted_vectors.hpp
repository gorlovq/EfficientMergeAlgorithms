#ifndef FRAMEWORK_H
#define FRAMEWORK_H

#include <vector>
#include <string>
#include <stdexcept>
#include "counting_int.hpp"

enum class CornerCaseType {
    RANDOM,                 // Completely random
    FIRST_ALL_SMALLER,      // All elements in A < all elements in B
    FIRST_ALL_GREATER,      // All elements in A > all elements in B
    PARTIAL_OVERLAP,        // Partial overlap in value ranges
    ONE_ELEMENT_EACH,       // Each array has exactly one element
    EQUAL_ARRAYS,           // Both arrays contain exactly the same elements
    DUPLICATES_IN_BOTH,     // Each array has many duplicates
    ONE_ARRAY_EMPTY,        // One array is empty, the other non-empty
    BLOCK_INTERLEAVE_A_B,   // result = {{K from a}, {L from b}, {K from a}, {L from b}...};  1<= K,L; K+L<=m+n
    BLOCK_INTERLEAVE_B_A    // result = {{K from b}, {L from a}, {K from b}, {L from a}...};  1<= K,L; K+L<=m+n
};


inline std::string toString(CornerCaseType caseType) {
    switch(caseType) {
        case CornerCaseType::RANDOM:
            return "RANDOM";
        case CornerCaseType::FIRST_ALL_SMALLER:
            return "FIRST_ALL_SMALLER";
        case CornerCaseType::FIRST_ALL_GREATER:
            return "FIRST_ALL_GREATER";
        case CornerCaseType::PARTIAL_OVERLAP:
            return "PARTIAL_OVERLAP";
        case CornerCaseType::ONE_ELEMENT_EACH:
            return "ONE_ELEMENT_EACH";
        case CornerCaseType::EQUAL_ARRAYS:
            return "EQUAL_ARRAYS";
        case CornerCaseType::DUPLICATES_IN_BOTH:
            return "DUPLICATES_IN_BOTH";
        case CornerCaseType::ONE_ARRAY_EMPTY:
            return "ONE_ARRAY_EMPTY";
        case CornerCaseType::BLOCK_INTERLEAVE_A_B:
            return "BLOCK_INTERLEAVE_A_B";
        case CornerCaseType::BLOCK_INTERLEAVE_B_A:
            return "BLOCK_INTERLEAVE_B_A";
        default:
            return "UNKNOWN";
    }
}

// Structure to hold the generated test case data.
struct MergeTestCase {
    std::vector<CountingInt> a;
    std::vector<CountingInt> b;
    std::vector<CountingInt> result;
};

/**
 * Generates two sorted vectors and a merged result according to the chosen corner case.
 *
 * For the BLOCK_INTERLEAVE cases, additional parameters control the block sizes.
 * For all cases, randomMin and randomMax determine the random value range.
 *
 * @param size_a       Desired size of vector A.
 * @param size_b       Desired size of vector B.
 * @param case_type    Corner case type.
 * @param random_min   (Optional) Minimum random value, default 0.
 * @param random_max   (Optional) Maximum random value, default 10000.
 * @param block_size_a (Optional) Block size for A in block interleaving cases, default 2.
 * @param block_size_b (Optional) Block size for B in block interleaving cases, default 3.
 */
MergeTestCase generate_sorted_vectors(int size_a,
                                      int size_b,
                                      CornerCaseType case_type,
                                      int random_min = 0,
                                      int random_max = 10000,
                                      int block_size_a = 2,
                                      int block_size_b = 3);

#endif // FRAMEWORK_H
