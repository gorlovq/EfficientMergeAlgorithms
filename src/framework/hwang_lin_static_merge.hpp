/*
 * Author: Sergei Gorlov.
 * Description: Declares the HwangLinStaticMergeAlgorithm class.
 */

#ifndef HWANG_LIN_STATIC_MERGE_HPP
#define HWANG_LIN_STATIC_MERGE_HPP

#include <string>
#include "merge_algorithm.hpp"
#include "../algorithms/algorithms.hpp"

class HwangLinStaticMergeAlgorithm : public MergeAlgorithm {
public:
    std::string getName() const override {
        return "HwangLinStaticMerge";
    }
    std::vector<CountingInt> merge(const std::vector<CountingInt>& a,
                           const std::vector<CountingInt>& b) override {

        std::vector<CountingInt> A = a;
        std::vector<CountingInt> B = b;

        return hwang_lin_static_merge(A, B);
    }
};

#endif // HWANG_LIN_STATIC_MERGE_HPP
