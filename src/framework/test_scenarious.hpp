// test_config.hpp
#ifndef TEST_CONFIG_HPP
#define TEST_CONFIG_HPP

#include "generate_sorted_vectors.hpp"

// Structure for describing a single test scenario
struct TestScenario {
    int sizeA;
    int sizeB;
    CornerCaseType caseType;
    int randomMin;
    int randomMax;
    int blockSizeA;
    int blockSizeB;
    int iterations;
};

// Structure for storing the results of one test scenario.
struct TestScenarioResult {
    TestScenario scenario;           // The test scenario configuration.
    double averageTime;              // Average execution time (in ms).
    long long averageCompressions;   // Average number of compression operations.
    bool isCorrect;                  // Flag indicating if all iterations matched the expected results.
    bool isStable;                   // Flag indicating whether the merge algorithm is stable (i.e., if equal elements preserve their input order).
};

#endif // TEST_CONFIG_HPP
