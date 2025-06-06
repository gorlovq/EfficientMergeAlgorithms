/*
 * Author: Igor Stikentzin. 
 * Description: Common utilities and definitions for efficient merge algorithms.
 */

#pragma once

#include <bit>
#include <algorithm>
#include <functional>
#include <iterator>


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
int binary_insert(
    IterContainer& c,
    typename IterContainer::const_iterator begin,
    typename IterContainer::const_iterator end,
    typename IterContainer::value_type element
) {
    auto insert_it = std::lower_bound(begin, end, element);
    c.insert(insert_it, element);
    return static_cast<int>(std::distance(c.cbegin(), insert_it));
}

template <class It>
void block_swap(It left, It right,
                typename std::iterator_traits<It>::difference_type k) {
    for (typename std::iterator_traits<It>::difference_type i = 0; i < k; ++i, ++left, ++right)
        std::iter_swap(left, right);
}

template <class It, class Comp = std::less<>>
It search_minimal_block(std::ptrdiff_t k,
                        It t,
                        It e,
                        It extra,
                        Comp comp = Comp{}) {
    It best = extra;
    auto best_first = *best;
    auto best_last  = *(best + std::min(k, std::distance(best, e)) - 1);

    const std::ptrdiff_t limit = std::distance(t, e);
    for (std::ptrdiff_t offset = 0; offset + k <= limit; offset += k) {
        It cur = t + offset;
        auto cur_first = *cur;
        auto cur_last  = *(cur + k - 1);

        if (comp(cur_first, best_first) ||
           (!comp(best_first, cur_first) && comp(cur_last, best_last))) {
            best = cur;
            best_first = cur_first;
            best_last  = cur_last;
        }
    }

    return best;
}

// Insert element and copy elements before insertion point using lower_bound.
// Used when merging A and B (A < B) to place elements from A before equal elements from B.
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

// Insert element and copy elements before insertion point using upper_bound.
// Used when merging A and B (A > B) to place elements from B after equal elements from A.
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
