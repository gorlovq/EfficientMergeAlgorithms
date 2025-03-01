#ifndef ALGORITHM_TESTER_HPP
#define ALGORITHM_TESTER_HPP

#include "merge_algorithm.hpp"
#include "framework.hpp" 
#include "test_scenarious.hpp"
#include <chrono>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>

class AlgorithmTester {
public:
    void addScenario(const TestScenario& scenario) {
        scenarios_.push_back(scenario);
    }

    std::vector<TestScenarioResult> runTests(MergeAlgorithm& algorithm) {
        std::vector<TestScenarioResult> results;
    
        for (const auto& scenario : scenarios_) {
            std::cout << "Running scenario: A = " << scenario.sizeA 
                      << ", B = " << scenario.sizeB 
                      << ", Case = " << static_cast<int>(scenario.caseType) << std::endl;
        
            double total_time = 0.0;
            bool is_correct = true;
        
            for (int i = 0; i < scenario.iterations; i++) {
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
                if (result != test_case.result) {
                    is_correct = false;
                }
            }
        
            double avg_time = total_time / scenario.iterations;
            results.push_back({scenario, avg_time, is_correct});
        }
    
        return results;
    }

    std::string generateReport(const std::vector<TestScenarioResult>& results) {
        std::ostringstream oss;

        // Set floating-point precision for average time
        oss << std::fixed << std::setprecision(6);

        // Define column widths (tweak as needed)
        const int colWidthScenario = 10;
        const int colWidthSizeA    = 8;
        const int colWidthSizeB    = 8;
        const int colWidthCase     = 22; // for longer strings like "FIRST_ALL_SMALLER"
        const int colWidthIter     = 10;
        const int colWidthTime     = 12;
        const int colWidthResult   = 10;

        // Print header
        oss << "Test Report:\n"
            << std::string(80, '-') << "\n";

        // Use std::left to left-align the columns (or std::right for right-alignment).
        oss << std::left
            << std::setw(colWidthScenario) << "Scenario"
            << std::setw(colWidthSizeA)    << "SizeA"
            << std::setw(colWidthSizeB)    << "SizeB"
            << std::setw(colWidthCase)     << "Case"
            << std::setw(colWidthIter)     << "Iter"
            << std::setw(colWidthTime)     << "AvgTime(ms)"
            << std::setw(colWidthResult)   << "Result"
            << "\n";

        oss << std::string(80, '-') << "\n";

        // Print each row
        for (const auto& res : results) {
            oss << std::left
                << std::setw(colWidthScenario) << "Scenario"
                << std::setw(colWidthSizeA)    << res.scenario.sizeA
                << std::setw(colWidthSizeB)    << res.scenario.sizeB
                << std::setw(colWidthCase)     << toString(res.scenario.caseType)  // your helper function
                << std::setw(colWidthIter)     << res.scenario.iterations
                << std::setw(colWidthTime)     << res.averageTime
                << std::setw(colWidthResult)   << (res.allCorrect ? "Correct" : "Incorrect")
                << "\n";
        }

        // Optional: trailing line
        oss << std::string(80, '-') << "\n";

        return oss.str();
    }
private:
    std::vector<TestScenario> scenarios_;
};

#endif // ALGORITHM_TESTER_HPP
