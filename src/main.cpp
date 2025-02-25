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
    TwoWayMergeAlgorithm two_way_merge_algorithm;
    HwangLinDynamicMergeAlgorithm hwang_lin_dynamic_merge_algorithm;
    HwangLinKnuthMergeAlgorithm hwang_lin_knuth_merge_algorithm;
    HwangLinStaticMergeAlgorithm hwang_lin_static_merg_algorithm;  
    FractialInsertionMergeAlgorithm fractial_insertion_merge_algorithm;  
    // TODO add simple_binary_merge

    AlgorithmTester tester; 

    MergeTestCase test_case = generate_sorted_vectors(100, 10000000, CornerCaseType::RANDOM);

    tester.testAlgorithm(two_way_merge_algorithm, test_case);
    tester.testAlgorithm(hwang_lin_dynamic_merge_algorithm, test_case);
    tester.testAlgorithm(hwang_lin_knuth_merge_algorithm, test_case);
    tester.testAlgorithm(hwang_lin_static_merg_algorithm, test_case);
    tester.testAlgorithm(fractial_insertion_merge_algorithm, test_case);

    return 0;
}