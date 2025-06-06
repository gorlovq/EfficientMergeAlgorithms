#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include "framework/generate_sorted_vectors.hpp"
#include "framework/hwang_lin_dynamic_merge.hpp"
#include "framework/hwang_lin_dynamic_stable_merge.hpp"
#include "framework/hwang_lin_static_merge.hpp"
#include "framework/hwang_lin_static_kutzner_merge.hpp"
#include "framework/hwang_lin_static_stable_merge.hpp"
#include "framework/hwang_lin_knuth_merge.hpp"
#include "framework/fractile_insertion_merge.hpp"
#include "framework/simple_kim_kutzner_merge.hpp"
#include "framework/unstable_core_kim_kutzner_merge.hpp"
#include "framework/algorithm_tester.hpp"
#include "framework/two_way_merge.hpp"

enum class OutputFormat {
    Console,
    CsvFile
};

int main(int argc, char* argv[]) {
    OutputFormat output = OutputFormat::Console;
    std::string outputDirName;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--csv" && i + 1 < argc) {
            outputDirName = argv[++i];
            output = OutputFormat::CsvFile;
        }
    }

    if (output == OutputFormat::CsvFile) {
        if (!std::filesystem::exists(outputDirName)) {
            std::cerr << "Error: directory doesn't exist " << outputDirName << std::endl;
            return 1;
        }
    }

    AlgorithmTester tester;

    // ====================================================================
    // TEST CASES: Comprehensive evaluation of merge algorithms across a wide range of array sizes
    // Array sizes range from 10^2 to 10^5 for both dimensions
    // This provides a thorough assessment of algorithm performance across different scale factors
    // ====================================================================

    // Fixed first array (M=10^2) with varying second array (N) sizes from 10^2 to 10^5
    //tester.addScenario({100000, 1000000, CornerCaseType::RANDOM, 0, 1000000, 5, 5});
    //tester.addScenario({100000, 1000000, CornerCaseType::DUPLICATES_IN_BOTH, 0, 1000000, 5, 5});
    //tester.addScenario({1000000, 1000000, CornerCaseType::EQUAL_ARRAYS, 0, 1000000, 5, 5});
    //tester.addScenario({100000, 1000000, CornerCaseType::FIRST_ALL_SMALLER, 0, 1000000, 5, 5});
    //tester.addScenario({100000, 1000000, CornerCaseType::FIRST_ALL_GREATER, 0, 1000000, 5, 5});
    //tester.addScenario({100000, 1000000, CornerCaseType::PARTIAL_OVERLAP, 0, 1000000, 5, 5});

    for (int m = 5000; m <= 100000; m += 5000) {
        for (int n = 5000; n <= 100000; n += 5000) {
            tester.addScenario({m, n, CornerCaseType::RANDOM, 0, 1000000, 5, 5});
        }
    }

    for (int m = 1000; m <= 5000; m += 1000) {
        for (int n = 1000; n <= 5000; n += 1000) {
            tester.addScenario({m, n, CornerCaseType::RANDOM, 0, 1000000, 5, 5});
        }
    }

    std::vector<std::unique_ptr<MergeAlgorithm>> algorithms;
    algorithms.push_back(std::make_unique<TwoWayMergeAlgorithm>());
    //algorithms.push_back(std::make_unique<HwangLinDynamicMergeAlgorithm>());
    //algorithms.push_back(std::make_unique<HwangLinDynamicStableMergeAlgorithm>());
    algorithms.push_back(std::make_unique<HwangLinKnuthMergeAlgorithm>());
    //algorithms.push_back(std::make_unique<HwangLinStaticMergeAlgorithm>());
    algorithms.push_back(std::make_unique<HwangLinStaticKutznerMergeAlgorithm>());
    //algorithms.push_back(std::make_unique<HwangLinStaticStableMergeAlgorithm>());
    algorithms.push_back(std::make_unique<FractialInsertionMergeAlgorithm>());
    algorithms.push_back(std::make_unique<SimpleKimKutznerMergeAlgorithm>());
    algorithms.push_back(std::make_unique<UnstableCoreKimKutznerMergeAlgorithm>());

    const std::string separator(REPORT_WIDTH, '=');

    // Iterate over each algorithm and run the tests.
    for (auto& alg : algorithms) {
        std::cout << separator << std::endl;
        std::cout << "Testing algorithm: " << alg->getName() << std::endl;
        std::cout << separator << std::endl;

        auto results = tester.runTests(*alg);
        std::string report = tester.generateReport(results);
        std::cout << report << std::endl;

        if (output == OutputFormat::CsvFile) {
            std::string filePath = outputDirName;
            if (filePath.back() != '/' && filePath.back() != '\\') {
                filePath += '/';
            }

            filePath += alg->getName() + ".csv";

            tester.generateCSV(filePath, { results });
        }
    }

    return 0;
}
