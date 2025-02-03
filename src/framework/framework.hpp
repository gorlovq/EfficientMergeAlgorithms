#ifndef FRAMEWORK_H
#define FRAMEWORK_H

#include <vector>

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

// Structure to hold the generated test case data.
struct MergeTestCase {
    std::vector<int> a;
    std::vector<int> b;
    std::vector<int> result;
};

/**
 * Generates two sorted vectors and a merged result according to the chosen corner case.
 *
 * For the BLOCK_INTERLEAVE cases, additional parameters control the block sizes.
 * For all cases, randomMin and randomMax determine the random value range.
 *
 * @param size_a     Desired size of vector A.
 * @param size_b     Desired size of vector B.
 * @param caseType   Corner case type.
 * @param randomMin  (Optional) Minimum random value, default 0.
 * @param randomMax  (Optional) Maximum random value, default 10000.
 * @param blockSizeA (Optional) Block size for A in block interleaving cases, default 2.
 * @param blockSizeB (Optional) Block size for B in block interleaving cases, default 3.
 */
MergeTestCase generate_sorted_vectors(int size_a,
                                      int size_b,
                                      CornerCaseType caseType,
                                      int randomMin = 0,
                                      int randomMax = 10000,
                                      int blockSizeA = 2,
                                      int blockSizeB = 3);

#endif // FRAMEWORK_H