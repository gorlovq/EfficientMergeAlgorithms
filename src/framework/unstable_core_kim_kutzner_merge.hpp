/*
 * Author: Igor Stikentzin.
 * Description: Declares the UnstableCoreKimKutznerMergeAlgorithm class.
 */

#pragma once

#include <string>
#include "merge_algorithm.hpp"
#include "../algorithms/algorithms.hpp"

class UnstableCoreKimKutznerMergeAlgorithm : public MergeAlgorithm {
public:
    std::string getName() const override {
        return "UnstableCoreKimKutznerMerge";
    }
    std::vector<CountingInt> merge(const std::vector<CountingInt>& a,
                           const std::vector<CountingInt>& b) override {

        std::vector<CountingInt> A = a;
        std::vector<CountingInt> B = b;

        return unstable_core_kim_kutzner_merge(A, B);
    }
};
