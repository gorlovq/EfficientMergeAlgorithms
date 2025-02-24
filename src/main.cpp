#include <iostream>
#include <vector>
#include "framework/framework.hpp"
#include "framework/hwang_lin_dynamic_merge.hpp"
#include "framework/hwang_lin_static_merge.hpp"
#include "framework/hwang_lin_knuth_merge.hpp"
#include "framework/fractile_insertion_merge.hpp"
#include "framework/algorithm_tester.hpp"
#include "framework/two_way_merge.hpp"


int main() {
    TwoWayMergeAlgorithm TwoWayMergeAlgorithm;
    HwangLinDynamicMergeAlgorithm HwangLinDynamicMergeAlgorithm;
    HwangLinKnuthMergeAlgorithm HwangLinKnuthMergeAlgorithm;
    HwangLinStaticMergeAlgorithm HwangLinStaticMergeAlgorithm;  
    FractialInsertionMergeAlgorithm FractialInsertionMergeAlgorithm;  
    // TODO add simple_binary_merge

    AlgorithmTester tester; 

    MergeTestCase testCase = generate_sorted_vectors(100, 10000000, CornerCaseType::RANDOM);

    tester.testAlgorithm(TwoWayMergeAlgorithm, testCase);
    tester.testAlgorithm(HwangLinDynamicMergeAlgorithm, testCase);
    tester.testAlgorithm(HwangLinKnuthMergeAlgorithm, testCase);
    tester.testAlgorithm(HwangLinStaticMergeAlgorithm, testCase);
    tester.testAlgorithm(FractialInsertionMergeAlgorithm, testCase);

    return 0;
}