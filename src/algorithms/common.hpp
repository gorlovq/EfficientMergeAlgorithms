/*
 * Author: Igor Stikentzin. 
 * Description: Common utilities and definitions for efficient merge algorithms.
 */

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

// Insert element and copy elements before insertion point
template <typename IterContainer>
typename IterContainer::iterator insert_and_copy(
    typename IterContainer::iterator start,
    typename IterContainer::iterator end,
    typename IterContainer::iterator& r_iter,
    const typename IterContainer::value_type& value
) {
    auto pos = std::lower_bound(start, end, value);
    std::copy(start, pos, r_iter);
    r_iter += std::distance(start, pos);
    *r_iter++ = value;
    return pos;
}

// Insert element and copy elements before insertion point
template <typename IterContainer>
typename IterContainer::iterator insert_and_copy_upper_bound(
    typename IterContainer::iterator start,
    typename IterContainer::iterator end,
    typename IterContainer::iterator& r_iter,
    const typename IterContainer::value_type& value
) {
    auto pos = std::upper_bound(start, end, value);
    std::copy(start, pos, r_iter);
    r_iter += std::distance(start, pos);
    *r_iter++ = value;
    return pos;
}

// Insert element and copy elements before insertion point
template <typename IterContainer>
typename IterContainer::iterator insert_and_copy_lower_bound(
    typename IterContainer::iterator start,
    typename IterContainer::iterator end,
    typename IterContainer::iterator& r_iter,
    const typename IterContainer::value_type& value
) {
    auto pos = std::lower_bound(start, end, value);
    std::copy(start, pos, r_iter);
    r_iter += std::distance(start, pos);
    *r_iter++ = value;
    return pos;
}