#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include "framework/generate_sorted_vectors.hpp"
#include "framework/hwang_lin_dynamic_merge.hpp"
#include "framework/hwang_lin_static_merge.hpp"
#include "framework/hwang_lin_knuth_merge.hpp"
#include "framework/fractile_insertion_merge.hpp"
#include "framework/simple_kim_kutzner_merge.hpp"
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

    tester.addScenario({100, 1000, CornerCaseType::RANDOM, 0, 1000000, 5, 5});
    tester.addScenario({500, 1000, CornerCaseType::RANDOM, 0, 1000000, 5, 5});
    tester.addScenario({1000, 1000, CornerCaseType::RANDOM, 0, 1000000, 5, 5});
    tester.addScenario({1000, 10000, CornerCaseType::RANDOM, 0, 1000000, 5, 5});

    std::vector<std::unique_ptr<MergeAlgorithm>> algorithms;
    algorithms.push_back(std::make_unique<TwoWayMergeAlgorithm>());
    algorithms.push_back(std::make_unique<HwangLinDynamicMergeAlgorithm>());
    algorithms.push_back(std::make_unique<HwangLinKnuthMergeAlgorithm>());
    algorithms.push_back(std::make_unique<HwangLinStaticMergeAlgorithm>());
    algorithms.push_back(std::make_unique<FractialInsertionMergeAlgorithm>());
    algorithms.push_back(std::make_unique<SimpleKimKutznerMergeAlgorithm>());

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
