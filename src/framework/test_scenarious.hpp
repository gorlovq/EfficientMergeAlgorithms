// test_config.hpp
#ifndef TEST_CONFIG_HPP
#define TEST_CONFIG_HPP

#include <vector>
#include "framework.hpp"  // Для CornerCaseType и MergeTestCase

// Структура описания одного тестового сценария
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

// Структура для хранения результатов одного тестового сценария
struct TestScenarioResult {
    TestScenario scenario;
    double averageTime;   // Среднее время выполнения (в мс)
    bool allCorrect;      // Все итерации дали корректный результат?
};

#endif // TEST_CONFIG_HPP
