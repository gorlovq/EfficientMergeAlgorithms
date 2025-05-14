/*
 * Author: Igor Stikentzin.
 * Description: Declares the HwangLinKnuthMergeAlgorithm class.
 */

#ifndef HWANG_LIN_KNUTH_MERGE_HPP
#define HWANG_LIN_KNUTH_MERGE_HPP

#include <string>
#include "merge_algorithm.hpp"
#include "../algorithms/algorithms.hpp"

class HwangLinKnuthMergeAlgorithm : public MergeAlgorithm {
public:
    std::string getName() const override {
        return "HwangLinKnuthMerge";
    }
    std::vector<CountingInt> merge(const std::vector<CountingInt>& a,
                           const std::vector<CountingInt>& b) override {
        return hwang_lin_knuth_merge(a, b);
    }
};

#endif // HWANG_LIN_KNUTH_MERGE_HPP
