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
    ONE_ARRAY_EMPTY         // One array is empty, the other non-empty
};

void generate_sorted_vectors(int size_a, int size_b, std::vector<int>& a, std::vector<int>& b, std::vector<int>& result,  CornerCaseType caseType);

#endif // FRAMEWORK_H