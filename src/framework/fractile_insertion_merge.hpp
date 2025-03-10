#ifndef FRACTIAL_INSERTION_MERGE_HPP
#define FRACTIAL_INSERTION_MERGE_HPP

#include "merge_algorithm.hpp"
#include "../algorithms/algorithms.hpp"

class FractialInsertionMergeAlgorithm : public MergeAlgorithm {
public:
    std::string getName() const override {
        return "FractialInsertionMerge";
    }
    std::vector<int> merge(const std::vector<int>& a,
                           const std::vector<int>& b) override {
        return fractile_insertion_merge(a, b);
    }
};

#endif // FRACTIAL_INSERTION_MERGE_HPP
