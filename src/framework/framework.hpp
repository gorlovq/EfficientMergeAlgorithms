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

void generate_sorted_vectors(int size_a, int size_b, std::vector<int>& a, std::vector<int>& b, std::vector<int>& result,  CornerCaseType caseType);

#endif // FRAMEWORK_H