#include "framework.hpp"
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <iostream>

inline int rand_in_range(int minVal, int maxVal) {
    return minVal + std::rand() % (maxVal - minVal + 1);
}

void generate_sorted_vectors(
    int size_a,
    int size_b,
    std::vector<int>& a,
    std::vector<int>& b,
    std::vector<int>& result,
    CornerCaseType caseType = CornerCaseType::RANDOM){
    // Seed for random number generation
    srand(static_cast<unsigned int>(time(nullptr)));

    a.resize(size_a);
    b.resize(size_b);

    switch (caseType) {
    case CornerCaseType::RANDOM:
        {
            for (int i = 0; i < size_a; ++i) {
                a[i] = std::rand() % 10000;
            }
            for (int i = 0; i < size_b; ++i) {
                b[i] = std::rand() % 10000;
            }
        }
        break;

    case CornerCaseType::FIRST_ALL_SMALLER:
        {
            for (int i = 0; i < size_a; ++i) {
                a[i] = rand_in_range(0, 5000);
            }
            for (int i = 0; i < size_b; ++i) {
                b[i] = rand_in_range(6000, 10000);
            }
        }
        break;

    case CornerCaseType::FIRST_ALL_GREATER:
        {
            for (int i = 0; i < size_a; ++i) {
                a[i] = rand_in_range(6000, 10000);
            }
            for (int i = 0; i < size_b; ++i) {
                b[i] = rand_in_range(0, 5000);
            }
        }
        break;

    case CornerCaseType::PARTIAL_OVERLAP:
        {
            // Example: A in [0..5000], B in [3000..8000].
            for (int i = 0; i < size_a; ++i) {
                a[i] = rand_in_range(0, 5000);
            }
            for (int i = 0; i < size_b; ++i) {
                b[i] = rand_in_range(3000, 8000);
            }
        }
        break;

    case CornerCaseType::ONE_ELEMENT_EACH:
        {
            a.resize(1);
            b.resize(1);

            a[0] = std::rand() % 10000;
            b[0] = std::rand() % 10000;
            size_a = 1;
            size_b = 1;
        }
        break;

    case CornerCaseType::EQUAL_ARRAYS:
        {
            if (size_a != size_b) {
                throw std::runtime_error("EQUAL_ARRAYS corner case requires the same sizes for A and B.");
            }

            for (int i = 0; i < size_a; ++i) {
                int val = std::rand() % 10000;
                a[i] = val;
                b[i] = val;
            }
        }
        break;

    case CornerCaseType::DUPLICATES_IN_BOTH:
        {
            for (int i = 0; i < size_a; ++i) {
                a[i] = rand_in_range(0, 5);
            }
            for (int i = 0; i < size_b; ++i) {
                b[i] = rand_in_range(0, 5);
            }
        }
        break;

    case CornerCaseType::ONE_ARRAY_EMPTY:
        {
            a.clear();
            a.shrink_to_fit(); // ensure capacity is zero if you'd like
            size_a = 0;

            // b is random
            for (int i = 0; i < size_b; ++i) {
                b[i] = std::rand() % 10000;
            }
        }
        break;
    }

    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());

    result.resize(size_a + size_b);
    std::merge(a.begin(), a.end(), b.begin(), b.end(), result.begin());
}