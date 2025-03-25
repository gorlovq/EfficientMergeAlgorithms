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
#include <vector>

constexpr int REPORT_WIDTH = 100;

class AlgorithmTester {
public:
explicit AlgorithmTester(
    int reportWidth = 100,
    int colWidthScenario = 10,
    int colWidthSizeA    = 8,
    int colWidthSizeB    = 8,
    int colWidthCase     = 22,
    int colWidthIter     = 10,
    int colWidthTime     = 12,
    int colWidthComp     = 20,
    int colWidthResult   = 10)
    :   reportWidth_(reportWidth),
        colWidthScenario_(colWidthScenario),
        colWidthSizeA_(colWidthSizeA),
        colWidthSizeB_(colWidthSizeB),
        colWidthCase_(colWidthCase),
        colWidthIter_(colWidthIter),
        colWidthTime_(colWidthTime),
        colWidthComp_(colWidthComp),
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

            for (int i = 0; i < scenario.iterations; i++) {
                CountingInt::resetCounter();

                MergeTestCase test_case = generate_sorted_vectors(
                    scenario.sizeA, scenario.sizeB, scenario.caseType,
                    scenario.randomMin, scenario.randomMax,
                    scenario.blockSizeA, scenario.blockSizeB
                );

                auto start = std::chrono::high_resolution_clock::now();
                auto result = algorithm.merge(test_case.a, test_case.b);
                auto end = std::chrono::high_resolution_clock::now();
                double elapsed = std::chrono::duration<double, std::milli>(end - start).count();

                total_time += elapsed;
                total_comparisons += CountingInt::comparisons; // Record comparisons for this iteration.

                if (result != test_case.result) {
                    is_correct = false;
                }
            }

            double avg_time = total_time / scenario.iterations;
            long long avg_comparisons = total_comparisons / scenario.iterations;
            results.push_back({scenario, avg_time, avg_comparisons, is_correct});
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
            << std::setw(colWidthIter_)     << "Iter"
            << std::setw(colWidthTime_)     << "AvgTime(ms)"
            << std::setw(colWidthComp_)     << "AvgComparisons"
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
                << std::setw(colWidthIter_)     << res.scenario.iterations
                << std::setw(colWidthTime_)     << res.averageTime
                << std::setw(colWidthComp_)     << res.averageCompressions
                << std::setw(colWidthResult_)   << (res.allCorrect ? "Correct" : "Incorrect")
                << "\n";
        }

        oss << separator << "\n";

        return oss.str();
    }
private:
    // Column width parameters for the report output table.
    int reportWidth_;
    int colWidthScenario_;
    int colWidthSizeA_;
    int colWidthSizeB_;
    int colWidthCase_;
    int colWidthIter_;
    int colWidthTime_;
    int colWidthComp_;
    int colWidthResult_;

    std::vector<TestScenario> scenarios_;
};

#endif // ALGORITHM_TESTER_HPP
