#ifndef ALGORITHM_TESTER_HPP
#define ALGORITHM_TESTER_HPP

#include "merge_algorithm.hpp"
#include "framework.hpp" 
#include <chrono>
#include <iostream>

class AlgorithmTester {
public:
    void testAlgorithm(MergeAlgorithm& algorithm, const MergeTestCase& testCase) {
        std::cout << "Testing algorithm: " << algorithm.getName() << std::endl;

        auto start = std::chrono::high_resolution_clock::now();
        auto result = algorithm.merge(testCase.a, testCase.b);
        auto end = std::chrono::high_resolution_clock::now();

        bool correct = (result == testCase.result);
        double ms = std::chrono::duration<double, std::milli>(end - start).count();

        std::cout << "Result: " << (correct ? "correct" : "incorrect")
                  << ". Time: " << ms << " ms" << std::endl << std::endl;
     }
};

#endif // ALGORITHM_TESTER_HPP
