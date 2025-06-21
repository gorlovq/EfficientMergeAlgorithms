/*
 * Author: Sergei Gorlov.
 * Description: Declares the HwangLinStaticMergeAlgorithm class.
 */

#ifndef STD_MERGE_HPP
#define STD_MERGE_HPP

#include <string>
#include "merge_algorithm.hpp"
#include "../algorithms/algorithms.hpp"

class StdMergeAlgorithm : public MergeAlgorithm {
public:
    std::string getName() const override {
        return "StdMerge";
    }
    std::vector<CountingInt> merge(const std::vector<CountingInt>& a,
                           const std::vector<CountingInt>& b) override {
        std::vector<CountingInt> result;
        result.resize(a.size() + b.size());
        std::merge(a.begin(), a.end(), b.begin(), b.end(), result.begin());
        return result;
    }
};

#endif // STD_MERGE_HPP
