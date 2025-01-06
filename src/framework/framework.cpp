#include "framework.hpp"
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>

enum class CornerCaseType {
    RANDOM,                 // Случайные отсортированные массивы
    FIRST_ALL_SMALLER,      // Все элементы первого массива меньше, чем элементы второго
    FIRST_ALL_GREATER,      // Все элементы первого массива больше, чем элементы второго
    PARTIAL_OVERLAP,        // Частичное пересечение диапазонов
    ONE_ELEMENT_EACH,       // В каждом массиве ровно один элемент
    EQUAL_ARRAYS,           // Оба массива идентичны
    DUPLICATES_IN_BOTH,     // В обоих массивах много дубликатов
    ONE_ARRAY_EMPTY         // Один из массивов пустой
};

void generate_sorted_vectors(
    int size_a,
    int size_b,
    std::vector<int>& a,
    std::vector<int>& b,
    std::vector<int>& result,
    CornerCaseType caseType = CornerCaseType::RANDOM){
    // Seed for random number generation
    srand(static_cast<unsigned int>(time(nullptr)));

    a.resize(size_a);
    b.resize(size_b);

    auto rand_in_range = [&](int minVal, int maxVal){
        return minVal + std::rand() % (maxVal - minVal + 1);
    };

    for (int i = 0; i < size_a; ++i) {
        a[i] = rand() % 10000;
    }

    for (int i = 0; i < size_b; ++i) {
        b[i] = rand() % 10000;
    }

    switch (caseType) {
    case CornerCaseType::RANDOM: 
        {
            for (int i = 0; i < size_a; ++i) {
                a[i] = std::rand() % 10000;
            }
            for (int i = 0; i < size_b; ++i) {
                b[i] = std::rand() % 10000;
            }
        }
        break;

    case CornerCaseType::FIRST_ALL_SMALLER:
        {
            for (int i = 0; i < size_a; ++i) {
                a[i] = rand_in_range(0, 5000);
            }
            for (int i = 0; i < size_b; ++i) {
                b[i] = rand_in_range(6000, 10000);
            }
        }
        break;
    case CornerCaseType::FIRST_ALL_GREATER:
        {
            for (int i = 0; i < size_a; ++i) {
                a[i] = rand_in_range(6000, 10000);
            }
            for (int i = 0; i < size_b; ++i) {
                b[i] = rand_in_range(0, 5000);
            }
        }
    }

    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());

    result.resize(size_a + size_b);
    std::merge(a.begin(), a.end(), b.begin(), b.end(), result.begin());
    std::sort(result.begin(), result.end());
}