/*
 * Author: Sergei Gorlov.
 * Description: Declares the HwangLinDynamicMergeAlgorithm class.
 */

#ifndef HWANG_LIN_DYNAMIC_MERGE_HPP
#define HWANG_LIN_DYNAMIC_MERGE_HPP

#include <string>
#include "merge_algorithm.hpp"
#include "../algorithms/algorithms.hpp"

class HwangLinDynamicMergeAlgorithm : public MergeAlgorithm {
public:
    std::string getName() const override {
        return "HwangLinDynamicMerge";
    }
    std::vector<CountingInt> merge(const std::vector<CountingInt>& a,
                           const std::vector<CountingInt>& b) override {
        std::vector<CountingInt> A = a;
        std::vector<CountingInt> B = b;

        return hwang_lin_dynamic_merge(A, B);
    }
};

#endif // HWANG_LIN_DYNAMIC_MERGE_HPP
