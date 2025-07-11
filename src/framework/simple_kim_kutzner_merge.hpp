/*
 * Author: Igor Stikentzin.
 * Description: Declares the SimpleKimKutznerMergeAlgorithm class.
 */

#pragma once

#include <string>
#include "merge_algorithm.hpp"
#include "../algorithms/algorithms.hpp"

class SimpleKimKutznerMergeAlgorithm : public MergeAlgorithm {
public:
    std::string getName() const override {
        return "SimpleKimKutznerMerge";
    }
    std::vector<CountingInt> merge(const std::vector<CountingInt>& a,
                           const std::vector<CountingInt>& b) override {

        std::vector<CountingInt> A = a;
        std::vector<CountingInt> B = b;

        return simple_kim_kutzner_merge(A, B);
    }
};
