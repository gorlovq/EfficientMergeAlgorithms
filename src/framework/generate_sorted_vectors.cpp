/*
 * Author: Sergei Gorlov.
 * Description:
 */

#include "generate_sorted_vectors.hpp"
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <vector>
#include "counting_int.hpp"

inline int rand_in_range(int minVal, int maxVal) {
    return minVal + std::rand() % (maxVal - minVal + 1);
}

MergeTestCase generate_sorted_vectors(
    int size_a,
    int size_b,
    CornerCaseType case_type = CornerCaseType::RANDOM,
    int random_min,
    int random_max,
    int block_size_a,
    int block_size_b)
{
    // Seed for random number generation
    srand(static_cast<unsigned int>(time(nullptr)));

    MergeTestCase test_case;
    // Pre-size the vectors where applicable.
    test_case.a.resize(size_a);
    test_case.b.resize(size_b);

    switch (case_type) {
    case CornerCaseType::RANDOM:
        {
            for (int i = 0; i < size_a; ++i) {
                test_case.a[i] = CountingInt(rand_in_range(random_min, random_max), Slice::A);
            }
            for (int i = 0; i < size_b; ++i) {
                test_case.b[i] = CountingInt(rand_in_range(random_min, random_max), Slice::B);
            }
        }
        break;

    case CornerCaseType::FIRST_ALL_SMALLER:
        {
            int mid = (random_min + random_max) / 2;
            for (int i = 0; i < size_a; ++i) {
                test_case.a[i] = CountingInt(rand_in_range(random_min, mid), Slice::A);
            }
            for (int i = 0; i < size_b; ++i) {
                test_case.b[i] = CountingInt(rand_in_range(mid + 1, random_max), Slice::B);
            }
        }
        break;

    case CornerCaseType::FIRST_ALL_GREATER:
        {
            int mid = (random_min + random_max) / 2;
            for (int i = 0; i < size_a; ++i) {
                test_case.a[i] = CountingInt(rand_in_range(mid + 1, random_max), Slice::A);
            }
            for (int i = 0; i < size_b; ++i) {
                test_case.b[i] = CountingInt(rand_in_range(random_min, mid), Slice::B);
            }
        }
        break;

    case CornerCaseType::PARTIAL_OVERLAP:
        {
            // Example: Let A be in [randomMin, mid2] and B be in [mid1, randomMax]
            int mid1 = random_min + (random_max - random_min) / 3;
            int mid2 = random_min + 2 * (random_max - random_min) / 3;
            for (int i = 0; i < size_a; ++i) {
                test_case.a[i] = CountingInt(rand_in_range(random_min, mid2), Slice::A);
            }
            for (int i = 0; i < size_b; ++i) {
                test_case.b[i] = CountingInt(rand_in_range(mid1, random_max), Slice::B);
            }
        }
        break;

    case CornerCaseType::ONE_ELEMENT_EACH:
        {
            test_case.a.resize(1);
            test_case.b.resize(1);
            test_case.a[0] = CountingInt(rand_in_range(random_min, random_max), Slice::A);
            test_case.b[0] = CountingInt(rand_in_range(random_min, random_max), Slice::B);
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
                int val = rand_in_range(random_min, random_max);
                test_case.a[i] = CountingInt(val, Slice::A);
                test_case.b[i] = CountingInt(val, Slice::B);
            }
        }
        break;

    case CornerCaseType::DUPLICATES_IN_BOTH:
        {
            // Use a small range to force duplicates.
            for (int i = 0; i < size_a; ++i) {
                test_case.a[i] = CountingInt(rand_in_range(0, 5), Slice::A);
            }
            for (int i = 0; i < size_b; ++i) {
                test_case.b[i] = CountingInt(rand_in_range(0, 5), Slice::B);
            }
        }
        break;

    case CornerCaseType::ONE_ARRAY_EMPTY:
        {
            test_case.a.clear();
            test_case.a.shrink_to_fit();
            size_a = 0;

            // Fill b with random numbers.
            for (int i = 0; i < size_b; ++i) {
                test_case.b[i] = CountingInt(rand_in_range(random_min, random_max), Slice::B);
            }
        }
        break;

    case CornerCaseType::BLOCK_INTERLEAVE_A_B:
        {
            int K = block_size_a;
            int L = block_size_b;

            test_case.a.clear();
            test_case.b.clear();
            test_case.result.clear();

            // Determine how many blocks are needed for each vector.
            int num_blocks_a = (size_a + K - 1) / K;
            int num_blocks_b = (size_b + L - 1) / L;
            int total_blocks = std::max(num_blocks_a, num_blocks_b);

            // Calculate the range available per block step (each cycle uses two blocks).
            int range_per_block = (random_max - random_min) / (total_blocks * 2);
            int current_value = random_min;

            // Fill arrays A and B in blocks.
            for (int block = 0; block < total_blocks; ++block) {
                // Fill block for A: up to K elements.
                for (int i = 0; i < K && static_cast<int>(test_case.a.size()) < size_a; ++i) {
                    test_case.a.push_back(CountingInt(rand_in_range(current_value, current_value + range_per_block - 1), Slice::A));
                }
                current_value += range_per_block;

                // Fill block for B: up to L elements.
                for (int i = 0; i < L && static_cast<int>(test_case.b.size()) < size_b; ++i) {
                    test_case.b.push_back(CountingInt(rand_in_range(current_value, current_value + range_per_block - 1), Slice::B));
                }
                current_value += range_per_block;
            }
        }
        break;

    case CornerCaseType::BLOCK_INTERLEAVE_B_A:
        {
            int K = block_size_a;
            int L = block_size_b;

            test_case.a.clear();
            test_case.b.clear();
            test_case.result.clear();

            // Determine how many blocks are needed for each vector.
            int num_blocks_a = (size_a + K - 1) / K;
            int num_blocks_b = (size_b + L - 1) / L;
            int total_blocks = std::max(num_blocks_a, num_blocks_b);

            // Calculate the range available per block step (each cycle uses two blocks).
            int range_per_block = (random_max - random_min) / (total_blocks * 2);
            int current_value = random_min;

            // Fill arrays A and B in blocks.
             for (int block = 0; block < total_blocks; ++block) {
                // For this case, fill block for B first.
                for (int i = 0; i < L && static_cast<int>(test_case.b.size()) < size_b; ++i) {
                    test_case.b.push_back(CountingInt(rand_in_range(current_value, current_value + range_per_block - 1), Slice::B));
                }
                current_value += range_per_block;

                // Then fill block for A.
                for (int i = 0; i < K && static_cast<int>(test_case.a.size()) < size_a; ++i) {
                    test_case.a.push_back(CountingInt(rand_in_range(current_value, current_value + range_per_block - 1), Slice::A));
                }
                current_value += range_per_block;
            }
        }
        break;
    }

    std::sort(test_case.a.begin(), test_case.a.end());
    std::sort(test_case.b.begin(), test_case.b.end());

    // Update the indices to reflect the sorted order.
    for (int i = 0; i < size_a; ++i) {
        test_case.a[i].index = i;
    }
    for (int i = 0; i < size_b; ++i) {
        test_case.b[i].index = i;
    }

    test_case.result.resize(size_a + size_b);
    std::merge(test_case.a.begin(), test_case.a.end(),
               test_case.b.begin(), test_case.b.end(),
               test_case.result.begin());

    return test_case;
}
