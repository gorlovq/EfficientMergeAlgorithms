#include <iostream>
#include <vector>
#include <string>
#include "framework/framework.hpp"
#include "framework/hwang_lin_dynamic_merge.hpp"
#include "framework/hwang_lin_static_merge.hpp"
#include "framework/hwang_lin_knuth_merge.hpp"
#include "framework/fractile_insertion_merge.hpp"
#include "framework/algorithm_tester.hpp"
#include "framework/two_way_merge.hpp"


int main() {
    AlgorithmTester tester;
    tester.addScenario({10, 100, CornerCaseType::RANDOM, 0, 100, 10, 10, 5});
    tester.addScenario({249, 200000, CornerCaseType::RANDOM, 0, 300000, 10, 10, 5});
    tester.addScenario({4440, 70000, CornerCaseType::FIRST_ALL_SMALLER, 0, 99000, 10, 10, 5});
    tester.addScenario({2300, 100000, CornerCaseType::PARTIAL_OVERLAP, 0, 999999, 10, 10, 5});
    tester.addScenario({1000, 22000, CornerCaseType::DUPLICATES_IN_BOTH, 0, 100, 10, 10, 3});


    std::vector<std::unique_ptr<MergeAlgorithm>> algorithms;
    algorithms.push_back(std::make_unique<TwoWayMergeAlgorithm>());
    algorithms.push_back(std::make_unique<HwangLinDynamicMergeAlgorithm>());
    algorithms.push_back(std::make_unique<HwangLinKnuthMergeAlgorithm>());
    algorithms.push_back(std::make_unique<HwangLinStaticMergeAlgorithm>());
    algorithms.push_back(std::make_unique<FractialInsertionMergeAlgorithm>());

    // Iterate over each algorithm and run the tests.
    for (auto& alg : algorithms) {
        std::cout << "================================================================================" << std::endl;
        std::cout << "Testing algorithm: " << alg->getName() << std::endl;
        std::cout << "================================================================================" << std::endl;

        auto results = tester.runTests(*alg);
        std::string report = tester.generateReport(results);
        std::cout << report << std::endl;
    }

    return 0;
}
