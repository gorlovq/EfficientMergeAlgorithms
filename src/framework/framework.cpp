#include "framework.hpp"
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <iostream>

inline int rand_in_range(int minVal, int maxVal) {
    return minVal + std::rand() % (maxVal - minVal + 1);
}

MergeTestCase generate_sorted_vectors(
    int size_a,
    int size_b,
    CornerCaseType caseType = CornerCaseType::RANDOM,
    int randomMin,
    int randomMax,
    int blockSizeA,
    int blockSizeB)
{
    // Seed for random number generation
    srand(static_cast<unsigned int>(time(nullptr)));

    MergeTestCase testCase;
    // Pre-size the vectors where applicable.
    testCase.a.resize(size_a);
    testCase.b.resize(size_b);

    switch (caseType) {
    case CornerCaseType::RANDOM:
        {
            for (int i = 0; i < size_a; ++i) {
                testCase.a[i] = rand_in_range(randomMin, randomMax);
            }
            for (int i = 0; i < size_b; ++i) {
                testCase.b[i] = rand_in_range(randomMin, randomMax);
            }
        }
        break;

    case CornerCaseType::FIRST_ALL_SMALLER:
        {
            int mid = (randomMin + randomMax) / 2;
            for (int i = 0; i < size_a; ++i) {
                testCase.a[i] = rand_in_range(randomMin, mid);
            }
            for (int i = 0; i < size_b; ++i) {
                testCase.b[i] = rand_in_range(mid + 1, randomMax);
            }
        }
        break;

    case CornerCaseType::FIRST_ALL_GREATER:
        {
            int mid = (randomMin + randomMax) / 2;
            for (int i = 0; i < size_a; ++i) {
                testCase.a[i] = rand_in_range(mid + 1, randomMax);
            }
            for (int i = 0; i < size_b; ++i) {
                testCase.b[i] = rand_in_range(randomMin, mid);
            }
        }
        break;

    case CornerCaseType::PARTIAL_OVERLAP:
        {
            // Example: Let A be in [randomMin, mid2] and B be in [mid1, randomMax]
            int mid1 = randomMin + (randomMax - randomMin) / 3;
            int mid2 = randomMin + 2 * (randomMax - randomMin) / 3;
            for (int i = 0; i < size_a; ++i) {
                testCase.a[i] = rand_in_range(randomMin, mid2);
            }
            for (int i = 0; i < size_b; ++i) {
                testCase.b[i] = rand_in_range(mid1, randomMax);
            }
        }
        break;

    case CornerCaseType::ONE_ELEMENT_EACH:
        {
            testCase.a.resize(1);
            testCase.b.resize(1);
            testCase.a[0] = rand_in_range(randomMin, randomMax);
            testCase.b[0] = rand_in_range(randomMin, randomMax);
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
                int val = rand_in_range(randomMin, randomMax);
                testCase.a[i] = val;
                testCase.b[i] = val;
            }
        }
        break;

    case CornerCaseType::DUPLICATES_IN_BOTH:
        {
            // Use a small range to force duplicates.
            for (int i = 0; i < size_a; ++i) {
                testCase.a[i] = rand_in_range(0, 5);
            }
            for (int i = 0; i < size_b; ++i) {
                testCase.b[i] = rand_in_range(0, 5);
            }
        }
        break;

    case CornerCaseType::ONE_ARRAY_EMPTY:
        {
            testCase.a.clear();
            testCase.a.shrink_to_fit();
            size_a = 0;

            // Fill b with random numbers.
            for (int i = 0; i < size_b; ++i) {
                testCase.b[i] = rand_in_range(randomMin, randomMax);
            }
        }
        break;

    case CornerCaseType::BLOCK_INTERLEAVE_A_B:
        {
            int K = blockSizeA;
            int L = blockSizeB;

            testCase.a.clear();
            testCase.b.clear();
            testCase.result.clear();

            // Determine how many blocks are needed for each vector.
            int num_blocks_a = (size_a + K - 1) / K;
            int num_blocks_b = (size_b + L - 1) / L;
            int total_blocks = std::max(num_blocks_a, num_blocks_b);

            // Calculate the range available per block step (each cycle uses two blocks).
            int range_per_block = (randomMax - randomMin) / (total_blocks * 2);
            int current_value = randomMin;

            // Fill arrays A and B in blocks.
            for (int block = 0; block < total_blocks; ++block) {
                // Fill block for A: up to K elements.
                for (int i = 0; i < K && static_cast<int>(testCase.a.size()) < size_a; ++i) {
                    testCase.a.push_back(rand_in_range(current_value, current_value + range_per_block - 1));
                }
                current_value += range_per_block;

                // Fill block for B: up to L elements.
                for (int i = 0; i < L && static_cast<int>(testCase.b.size()) < size_b; ++i) {
                    testCase.b.push_back(rand_in_range(current_value, current_value + range_per_block - 1));
                }
                current_value += range_per_block;
            }

            // Sort the arrays.
            std::sort(testCase.a.begin(), testCase.a.end());
            std::sort(testCase.b.begin(), testCase.b.end());
        }
        break;

    case CornerCaseType::BLOCK_INTERLEAVE_B_A:
        {
            int K = blockSizeA;
            int L = blockSizeB;

            testCase.a.clear();
            testCase.b.clear();
            testCase.result.clear();

            // Determine how many blocks are needed for each vector.
            int num_blocks_a = (size_a + K - 1) / K;
            int num_blocks_b = (size_b + L - 1) / L;
            int total_blocks = std::max(num_blocks_a, num_blocks_b);

            // Calculate the range available per block step (each cycle uses two blocks).
            int range_per_block = (randomMax - randomMin) / (total_blocks * 2);
            int current_value = randomMin;

            // Fill arrays A and B in blocks.
             for (int block = 0; block < total_blocks; ++block) {
                // For this case, fill block for B first.
                for (int i = 0; i < L && static_cast<int>(testCase.b.size()) < size_b; ++i) {
                    testCase.b.push_back(rand_in_range(current_value, current_value + range_per_block - 1));
                }
                current_value += range_per_block;

                // Then fill block for A.
                for (int i = 0; i < K && static_cast<int>(testCase.a.size()) < size_a; ++i) {
                    testCase.a.push_back(rand_in_range(current_value, current_value + range_per_block - 1));
                }
                current_value += range_per_block;
            }

            // Sort the arrays.
            std::sort(testCase.a.begin(), testCase.a.end());
            std::sort(testCase.b.begin(), testCase.b.end());
        }
        break;
    }

    std::sort(testCase.a.begin(), testCase.a.end());
    std::sort(testCase.b.begin(), testCase.b.end());
    testCase.result.resize(size_a + size_b);
    std::merge(testCase.a.begin(), testCase.a.end(),
               testCase.b.begin(), testCase.b.end(),
               testCase.result.begin());

    return testCase;
}