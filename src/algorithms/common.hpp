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
void floating_hole(It blockEnd,
                   It first2,
                   typename std::iterator_traits<It>::difference_type len) {
    if (len == 0) return;
    std::rotate(blockEnd, first2, first2 + len);  // [blockEnd, first2) <- left, [first2, first2+len) -> right
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
    for (It cur = t; std::distance(cur, e) >= k; cur += k) {
        It cur_last  = cur  + k - 1;
        It best_last = best + k - 1;
        if (comp(*cur, *best) || (!comp(*best, *cur) && comp(*cur_last, *best_last)))
            best = cur;
    }
    return best;
}
