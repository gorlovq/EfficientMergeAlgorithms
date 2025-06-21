/*
 * Author: Sergei Gorlov.
 * Description: Declares the SplitMergeAlgorithm class.
 */

#ifndef SPLIT_MERGE_HPP
#define SPLIT_MERGE_HPP

#include <string>
#include "merge_algorithm.hpp"
#include "../algorithms/algorithms.hpp"

class SplitMergeAlgorithm : public MergeAlgorithm {
public:
    std::string getName() const override {
        return "SplitMerge";
    }
    std::vector<CountingInt> merge(const std::vector<CountingInt>& a,
                           const std::vector<CountingInt>& b) override {

        std::vector<CountingInt> A = a;
        std::vector<CountingInt> B = b;

        return split_merge(A, B);
    }
};

#endif // SPLIT_MERGE_HPP