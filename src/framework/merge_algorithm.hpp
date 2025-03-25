#ifndef MERGE_ALGORITHM_HPP
#define MERGE_ALGORITHM_HPP

#include <string>
#include <vector>
#include "counting_int.hpp"

class MergeAlgorithm {
public:
    virtual std::string getName() const = 0;
    virtual std::vector<CountingInt> merge(const std::vector<CountingInt>& a, const std::vector<CountingInt>& b) = 0;
    virtual ~MergeAlgorithm() = default;
};

#endif // MERGE_ALGORITHM_HPP
