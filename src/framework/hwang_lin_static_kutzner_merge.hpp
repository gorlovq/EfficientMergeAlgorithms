/*
 * Author: Igor Stikentzin.
 * Description: Declares the HwangLinStaticKutznerMergeAlgorithm class.
 */

#pragma once

#include <string>
#include "merge_algorithm.hpp"
#include "../algorithms/algorithms.hpp"

class HwangLinStaticKutznerMergeAlgorithm : public MergeAlgorithm {
public:
    std::string getName() const override {
        return "HwangLinStaticKutznerMergeAlgorithm";
    }
    std::vector<CountingInt> merge(const std::vector<CountingInt>& a,
                           const std::vector<CountingInt>& b) override {

        std::vector<CountingInt> A = a;
        std::vector<CountingInt> B = b;

        return hwang_lin_static_kutzner_merge(A, B);
    }
};
