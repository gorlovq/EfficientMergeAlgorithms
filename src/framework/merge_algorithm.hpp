#ifndef MERGE_ALGORITHM_HPP
#define MERGE_ALGORITHM_HPP

#include <string>
#include <vector>

class MergeAlgorithm {
public:
    virtual std::string getName() const = 0;
    virtual std::vector<int> merge(const std::vector<int>& a, const std::vector<int>& b) = 0;
    virtual ~MergeAlgorithm() = default;
};

#endif // MERGE_ALGORITHM_HPP