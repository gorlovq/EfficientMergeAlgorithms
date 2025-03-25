// test_config.hpp
#ifndef TEST_CONFIG_HPP
#define TEST_CONFIG_HPP

#include "framework.hpp"  // Для CornerCaseType и MergeTestCase

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

// Structure for storing the results of one test scenario
struct TestScenarioResult {
    TestScenario scenario;
    double averageTime;   // Average execution time (in ms)
    long long averageCompressions;
    bool allCorrect;      // Flag indicating if all iterations matched the expected results
};

#endif // TEST_CONFIG_HPP
