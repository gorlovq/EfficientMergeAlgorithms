#pragma once

#include <bit>
#include <algorithm>


// Helper function: bit_floor for compilers without <bit>
#if __cpp_lib_bitops < 201907L
constexpr std::size_t bit_floor(std::size_t x) noexcept {
    return x ? std::size_t{1} << (std::bit_width(x) - 1) : 0;
}
#else
using std::bit_floor;
#endif

constexpr int pow2(int t) { return 1<<t; } // Raise a number to the power of 2 using a bitwise operator

// Insert element in contaiiner in specified range
template <typename IterContainer>
void binary_insert(
    IterContainer& c,
    typename IterContainer::const_iterator begin,
    typename IterContainer::const_iterator end,
    typename IterContainer::value_type element
) {
    auto inser_it = std::lower_bound(begin, end, element);
    c.insert(inser_it, element);
}
