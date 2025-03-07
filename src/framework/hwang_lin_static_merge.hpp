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
    std::vector<int> merge(const std::vector<int>& a,
                           const std::vector<int>& b) override {

        std::vector<int> A = a;
        std::vector<int> B = b;

        return hwang_lin_static_merge(A, B);
    }
};

#endif // HWANG_LIN_STATIC_MERGE_HPP
