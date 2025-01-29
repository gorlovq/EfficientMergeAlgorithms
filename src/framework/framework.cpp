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

    case CornerCaseType::BLOCK_INTERLEAVE_A_B:
        {
            // TODO: make as params.
            int K = 2;
            int L = 3;

            a.clear();
            b.clear();
            result.clear();

            // TODO: make as params.
            int minVal = 0;
            int maxVal = 10000;

            // Calculate the number of blocks needed to fill a and b
            int num_blocks_a = (size_a + K - 1) / K;
            int num_blocks_b = (size_b + L - 1) / L;

            // Range for each block
            int total_blocks = std::max(num_blocks_a, num_blocks_b);
            int range_per_block = (maxVal - minVal) / (total_blocks * 2); // Two blocks per step (a and b)

            // Fill a and b with controlled ranges
            int current_value = minVal;

            // Blocks for a and b
            for (int block = 0; block < total_blocks; ++block) {
                // Range for block a: [current_value, current_value + range_per_block - 1]
                for (int i = 0; i < K && a.size() < size_a; ++i) {
                    a.push_back(rand_in_range(current_value, current_value + range_per_block - 1));
                }
                current_value += range_per_block;

                // Range for block b: [current_value, current_value + range_per_block - 1]
                for (int i = 0; i < L && b.size() < size_b; ++i) {
                    b.push_back(rand_in_range(current_value, current_value + range_per_block - 1));
                }
                current_value += range_per_block;
            }

            // Sort a and b
            std::sort(a.begin(), a.end());
            std::sort(b.begin(), b.end());

            // Build result by alternating blocks
            int a_idx = 0, b_idx = 0;
            while (a_idx < size_a || b_idx < size_b) {
                // Add K elements from a
                for (int i = 0; i < K && a_idx < size_a; ++i) {
                    result.push_back(a[a_idx++]);
                }
                // Add L elements from b
                for (int i = 0; i < L && b_idx < size_b; ++i) {
                    result.push_back(b[b_idx++]);
                }
            }
        }
        break;

    case CornerCaseType::BLOCK_INTERLEAVE_B_A:
        {
            // TODO: Make K and L parameters
            int K = 2;
            int L = 3;

            a.clear();
            b.clear();
            result.clear();

            // TODO: Make min/max range parameters
            int minVal = 0;
            int maxVal = 10000;

            // Calculate number of blocks needed for a and b
            int num_blocks_a = (size_a + K - 1) / K; // Round up for a's blocks
            int num_blocks_b = (size_b + L - 1) / L; // Round up for b's blocks
            int total_blocks = std::max(num_blocks_a, num_blocks_b);

            // Range allocated per block (each a/b pair uses 2 blocks)
            int range_per_block = (maxVal - minVal) / (total_blocks * 2);

            int current_value = minVal;

            // Generate a and b with controlled ranges
            for (int block = 0; block < total_blocks; ++block) {
                // Generate L elements for b in [current_value, current_value + range_per_block - 1]
                for (int i = 0; i < L && b.size() < size_b; ++i) {
                    b.push_back(rand_in_range(current_value, current_value + range_per_block - 1));
                }
                current_value += range_per_block;

                // Generate K elements for a in [current_value, current_value + range_per_block - 1]
                for (int i = 0; i < K && a.size() < size_a; ++i) {
                    a.push_back(rand_in_range(current_value, current_value + range_per_block - 1));
                }
                current_value += range_per_block;
            }

            // Sort a and b
            std::sort(a.begin(), a.end());
            std::sort(b.begin(), b.end());

            // Merge by alternating L from b and K from a
            int a_idx = 0, b_idx = 0;
            while (a_idx < size_a || b_idx < size_b) {
                // Add L elements from b
                for (int i = 0; i < L && b_idx < size_b; ++i) {
                    result.push_back(b[b_idx++]);
                }
                // Add K elements from a
                for (int i = 0; i < K && a_idx < size_a; ++i) {
                    result.push_back(a[a_idx++]);
                }
            }
        }
        break;
    }

    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());

    result.resize(size_a + size_b);
    std::merge(a.begin(), a.end(), b.begin(), b.end(), result.begin());
}