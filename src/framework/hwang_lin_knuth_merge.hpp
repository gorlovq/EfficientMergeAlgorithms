#ifndef HWANG_LIN_KNUTH_MERGE_HPP
#define HWANG_LIN_KNUTH_MERGE_HPP

#include "merge_algorithm.hpp"
#include "../algorithms/algorithms.hpp"

class HwangLinKnuthMergeAlgorithm : public MergeAlgorithm {
public:
    std::string getName() const override {
        return "HwangLinKnuthMerge";
    }
    std::vector<int> merge(const std::vector<int>& a,
                           const std::vector<int>& b) override {
        return hwang_lin_knuth_merge(a, b);
    }
};

#endif // HWANG_LIN_KNUTH_MERGE_HPP
