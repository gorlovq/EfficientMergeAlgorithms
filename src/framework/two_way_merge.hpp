/*
 * Authors: Sergei Gorlov and Igor Stikentzin.
 * Description: Declares the TwoWayMergeAlgorithm class.
 */

#ifndef TWO_WAY_MERGE_HPP
#define TWO_WAY_MERGE_HPP

#include <string>
#include "merge_algorithm.hpp"
#include "../algorithms/algorithms.hpp"

class TwoWayMergeAlgorithm : public MergeAlgorithm {
public:
    std::string getName() const override {
        return "TwoWayMerge";
    }
    std::vector<CountingInt> merge(const std::vector<CountingInt>& a,
                           const std::vector<CountingInt>& b) override {
        return two_way_merge(a, b);
    }
};

#endif // TWO_WAY_MERGE_HPP
