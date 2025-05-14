/*
 * Author: Igor Stikentzin.
 * Description: Declares the FractialInsertionMergeAlgorithm class.
 */

#ifndef FRACTIAL_INSERTION_MERGE_HPP
#define FRACTIAL_INSERTION_MERGE_HPP

#include <string>
#include "merge_algorithm.hpp"
#include "../algorithms/algorithms.hpp"

class FractialInsertionMergeAlgorithm : public MergeAlgorithm {
public:
    std::string getName() const override {
        return "FractialInsertionMerge";
    }
    std::vector<CountingInt> merge(const std::vector<CountingInt>& a,
                           const std::vector<CountingInt>& b) override {
        return fractile_insertion_merge(a, b);
    }
};

#endif // FRACTIAL_INSERTION_MERGE_HPP
