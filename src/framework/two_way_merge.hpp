#ifndef TWO_WAY_MERGE_HPP
#define TWO_WAY_MERGE_HPP

#include "merge_algorithm.hpp"
#include "../algorithms/algorithms.hpp"

class TwoWayMergeAlgorithm : public MergeAlgorithm {
public:
    std::string getName() const override {
        return "TwoWayMerge";
    }
    std::vector<int> merge(const std::vector<int>& a,
                           const std::vector<int>& b) override {
        return two_way_merge(a, b);
    }
};

#endif // TWO_WAY_MERGE_HPP
