#ifndef ALGORITHM_TESTER_HPP
#define ALGORITHM_TESTER_HPP

#include "merge_algorithm.hpp"
#include "generate_sorted_vectors.hpp"
#include "test_scenarious.hpp"
#include "counting_int.hpp"
#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <fstream>
#include <vector>

constexpr int REPORT_WIDTH = 110;

class AlgorithmTester {
public:
explicit AlgorithmTester(
    int reportWidth = 100,
    int colWidthScenario = 10,
    int colWidthSizeA    = 8,
    int colWidthSizeB    = 8,
    int colWidthCase     = 22,
    int colWidthTime     = 12,
    int colWidthComp     = 14,
    int colWidthStable   = 10,
    int colWidthResult   = 10)
    :   reportWidth_(reportWidth),
        colWidthScenario_(colWidthScenario),
        colWidthSizeA_(colWidthSizeA),
        colWidthSizeB_(colWidthSizeB),
        colWidthCase_(colWidthCase),
        colWidthTime_(colWidthTime),
        colWidthComp_(colWidthComp),
        colWidthStable_(colWidthStable),
        colWidthResult_(colWidthResult)
    {}

    void addScenario(const TestScenario& scenario) {
        scenarios_.push_back(scenario);
    }

    std::vector<TestScenarioResult> runTests(MergeAlgorithm& algorithm) {
        std::vector<TestScenarioResult> results;

        for (const auto& scenario : scenarios_) {
            std::cout << "Running scenario: A = " << scenario.sizeA
                      << ", B = " << scenario.sizeB
                      << ", Case = " << toString(scenario.caseType) << std::endl;

            double total_time = 0.0;
            long long total_comparisons = 0;
            bool is_correct = true;
            bool is_stable = true;

            MergeTestCase test_case = generate_sorted_vectors(
                scenario.sizeA, scenario.sizeB, scenario.caseType,
                scenario.randomMin, scenario.randomMax,
                scenario.blockSizeA, scenario.blockSizeB
            );

            CountingInt::resetCounter();

            auto start = std::chrono::high_resolution_clock::now();
            auto result = algorithm.merge(test_case.a, test_case.b);
            auto end = std::chrono::high_resolution_clock::now();
            double elapsed = std::chrono::duration<double, std::milli>(end - start).count();

            total_time += elapsed;
            total_comparisons += CountingInt::comparisons;

            if (result != test_case.result) {
                is_correct = false;
            }

            bool iteration_stable = true;
            for (size_t j = 1; j < result.size(); j++) {
                if (result[j - 1].value == result[j].value) {
                    if (result[j - 1].source == result[j].source) {
                        // For elements from the same slice, ensure original order is preserved.
                        if (result[j - 1].index > result[j].index) {
                            iteration_stable = false;
                            break;
                        }
                    } else {
                        // For elements from different slices, element from A must come first.
                        if (result[j - 1].source == Slice::B && result[j].source == Slice::A) {
                            iteration_stable = false;
                            break;
                        }
                    }
                }
            }
            if (!iteration_stable) {
                is_stable = false;
            }


            double avg_time = total_time;
            long long avg_comparisons = total_comparisons;
            results.push_back({scenario, avg_time, avg_comparisons, is_correct, is_stable});
        }

        return results;
    }

    std::string generateReport(const std::vector<TestScenarioResult>& results) {
        std::ostringstream oss;

        // Set floating-point precision for average time
        oss << std::fixed << std::setprecision(6);

        const std::string separator(REPORT_WIDTH, '-');

        oss << "Test Report:\n" << separator << "\n";

        // Use std::left to left-align the columns (or std::right for right-alignment).
        oss << std::left
            << std::setw(colWidthScenario_) << "Scenario"
            << std::setw(colWidthSizeA_)    << "SizeA"
            << std::setw(colWidthSizeB_)    << "SizeB"
            << std::setw(colWidthCase_)     << "Case"
            << std::setw(colWidthTime_)     << "Time(ms)"
            << std::setw(colWidthComp_)     << "Comparisons"
            << std::setw(colWidthStable_)   << "Stable"
            << std::setw(colWidthResult_)   << "Result"
            << "\n";

        oss << separator << "\n";

        // Print each row
        for (const auto& res : results) {
            oss << std::left
                << std::setw(colWidthScenario_) << "Scenario"
                << std::setw(colWidthSizeA_)    << res.scenario.sizeA
                << std::setw(colWidthSizeB_)    << res.scenario.sizeB
                << std::setw(colWidthCase_)     << toString(res.scenario.caseType)
                << std::setw(colWidthTime_)     << res.time
                << std::setw(colWidthComp_)     << res.compressions
                << std::setw(colWidthStable_)   << (res.isStable ? "Stable" : "Unstable")
                << std::setw(colWidthResult_)   << (res.isCorrect ? "Correct" : "Incorrect")
                << "\n";
        }

        oss << separator << "\n";

        return oss.str();
    }

    void generateCSV(const std::string& filename, const std::vector<TestScenarioResult>& results) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: unable to open file " << filename << " for writing." << std::endl;
            return;
        }

        // Write CSV header.
        file << "TestCase,M,N,Case,Time(ms),Comparisons,Stable,Correct\n";

        // Write each iteration result as a row.
        for (const auto& res : results) {
            // Use a helper function to convert case type to string (e.g., toString(res.scenario.caseType)).
            file << "Scenario" << ","  // You might want to number or name your scenarios.
                 << res.scenario.sizeA << ","
                 << res.scenario.sizeB << ","
                 << toString(res.scenario.caseType) << ","
                 << res.time << ","
                 << res.compressions << ","
                 << (res.isStable ? "Stable" : "Unstable") << ","
                 << (res.isCorrect ? "Correct" : "Incorrect") << "\n";
        }
        file.close();
    }
private:
    // Column width parameters for the report output table.
    int reportWidth_;
    int colWidthScenario_;
    int colWidthSizeA_;
    int colWidthSizeB_;
    int colWidthCase_;
    int colWidthTime_;
    int colWidthComp_;
    int colWidthStable_;
    int colWidthResult_;

    std::vector<TestScenario> scenarios_;
};

#endif // ALGORITHM_TESTER_HPP
