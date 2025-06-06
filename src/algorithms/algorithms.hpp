/*
 * Authors: Sergei Gorlov and Igor Stikentzin.
 * Description: Implements a collection of efficient merge algorithms,
 */

#pragma once

#include <array>
#include <algorithm>
#include <iterator>
#include <vector>
#include <cmath>

#include "common.hpp"


/*
 * Algorithm: Two-way Merge
 *
 * Publication:
 *   Thanh M., The Design and Analysis of Algorithms For Sort and Merge using Compressions
 *   // Master's Thesis. – Concordia University, Montreal, Canada. – 1983. – c.35-38.
 *
 * Implementation:
 *   Developers: Sergei Gorlov, Igor Stikentzin
 *
 * Parameters:
 *   IterContainer& a - container with a sorted sequence of smaller size.
 *                      Elements must be in ascending order.
 *                      IMPORTANT: The container must be accessed starting from its beginning.
 *
 *   IterContainer& b - container with a sorted sequence of larger size.
 *                      Elements must be in ascending order.
 *                      IMPORTANT: The container must be accessed starting from its beginning.
 *
 * Return Value:
 *   IterContainer - merged container containing all elements from a and b, sorted in ascending order.
 *
 * Notes:
 *   - Containers must support the methods size(), begin(), end().
 *   - It is assumed that the containers a and b are already sorted before calling the function.
 *
 */
template <typename IterContainer>
IterContainer two_way_merge(const IterContainer& a, const IterContainer& b) {
    auto [a_left, a_right, b_left, b_right] = std::array<typename IterContainer::const_iterator, 4>{{a.begin(), a.end(), b.begin(), b.end()}};
    IterContainer r(a.size() + b.size()); // Resulting vector

    typename IterContainer::iterator k_iter = r.begin(); // Pointer for insertion

    while (a_left != a_right && b_left != b_right) {
        *k_iter++ = (*a_left <= *b_left) ? *a_left++ : *b_left++;
    }

    if (a_left == a_right) {
        std::copy(b_left, b_right, k_iter);
    } else {
        std::copy(a_left, a_right, k_iter);
    }

    return r;
}

template <typename IterContainer, typename T>
void binary_insertion(IterContainer& arr, const T& elem) {
    typename IterContainer::iterator l = arr.begin();
    typename IterContainer::iterator u = arr.end();

    while (std::distance(l, u) > 0) {
        typename IterContainer::iterator j = l;
        int mid = std::distance(l, u) / 2;
        std::advance(j, mid);

        if (*j < elem) {
            l = std::next(j);
        } else {
            u = j;
        }
    }

    arr.insert(l, elem);
}

/*
 * Algorithm: Hwang-Lin Merge (Knuth)
 *
 * Publication:
 *   Knuth, D. E. Art of Computer Programming Volume 3: Sorting & Searching /
 *   D. E. Knuth. — 2nd ed. — Boston: Addison-Wesley, 1998. — c.203-204 — ISBN 0-201-89685-0.
 *
 * Implementation:
 *   Developer: Igor Stikentzin
 *
 * Parameters:
 *   IterContainer& a - container with a sorted sequence of smaller size.
 *                      Elements must be in ascending order.
 *                      IMPORTANT: The container must be accessed starting from its beginning.
 *
 *   IterContainer& b - container with a sorted sequence of larger size.
 *                      Elements must be in ascending order.
 *                      IMPORTANT: The container must be accessed starting from its beginning.
 *
 * Return Value:
 *   IterContainer - merged container containing all elements from a and b, sorted in ascending order.
 *
 * Notes:
 *   - Containers must support the methods size(), begin(), end().
 *   - It is assumed that the containers a and b are already sorted before calling the function.
 *
 */
template <typename IterContainer>
requires std::random_access_iterator<typename IterContainer::const_iterator>
IterContainer hwang_lin_knuth_merge(const IterContainer& a, const IterContainer& b) {
    using size_t  = typename IterContainer::size_type;
    using iter    = typename IterContainer::const_iterator;
    using value_t = typename IterContainer::value_type;

    iter a_left  = a.begin(), a_right = a.end();
    iter b_left  = b.begin(), b_right = b.end();
    size_t m = a.size();
    size_t n = b.size();

    if (!m) return b;
    if (!n) return a;

    IterContainer out(m + n);
    auto k = out.end();

    while (m && n)
    {
        // H1
        if (m > n) {
            std::swap(a_left,  b_left);
            std::swap(a_right, b_right);
            std::swap(m, n);
        }

        size_t s = bit_floor(n / m);

        // H2 / H4
        if (*(a_right - 1) < *(b_right - s)) {
            k -= s;
            std::copy(b_right - s, b_right, k);
            b_right -= s;  n -= s;
            continue; // back to H1
        }

        // H3 / H5
        value_t a_last = *(a_right - 1);

        iter pos;
        if (s <= 8) {
            pos = b_right;
            do { --pos; } while (a_last < *pos);
            ++pos;
        } else {
            pos = std::upper_bound(b_right - s, b_right, a_last);
        }

        size_t tail = static_cast<size_t>(b_right - pos);
        k -= tail + 1;
        std::copy(pos, b_right, k + 1);
        *k = a_last;

        b_right = pos;
        n -= tail;
        --a_right; --m;
    }

    if (m)  std::copy(a_left, a_right, k - m);
    else    std::copy(b_left, b_right, k - n);

    return out;
}

/*
 * Algorithm: Hwang-Lin Static Merge
 *
 * Publication:
 *   Thanh M., The Design and Analysis of Algorithms For Sort and Merge using Compressions
 *   // Master's Thesis. – Concordia University, Montreal, Canada. – 1983. – c.39-43.
 *
 * Implementation:
 *   Developer: Sergei Gorlov
 *
 * Parameters:
 *   IterContainer& a - container with a sorted sequence of smaller size.
 *                      Elements must be in ascending order.
 *                      IMPORTANT: The container must be accessed starting from its beginning.
 *
 *   IterContainer& b - container with a sorted sequence of larger size.
 *                      Elements must be in ascending order.
 *                      IMPORTANT: The container must be accessed starting from its beginning.
 *
 * Return Value:
 *   IterContainer - merged container containing all elements from a and b, sorted in ascending order.
 *
 * Notes:
 *   - Containers must support the methods size(), reserve(), begin(), end(), insert().
 *   - It is assumed that the containers a and b are already sorted before calling the function.
 *
 */
template <typename IterContainer>
IterContainer hwang_lin_static_merge(IterContainer& a, IterContainer& b) {
    if (a.empty()) {
        return b;
    }
    if (b.empty()) {
        return a;
    }

    // Swap a and b if a is larger than b.
    if (a.size() > b.size()) {
        std::swap(a, b);
    }

    int m = static_cast<int>(a.size());
    int n = static_cast<int>(b.size());

    // Pre-allocate result to avoid reallocations during merge.
    IterContainer results(a.size() + b.size());
    
    typename IterContainer::iterator r_iter = results.end(); // write-pointer starting from the back.

    int t = static_cast<int>(std::floor(std::log2(static_cast<double>(n) / m)));
    int pow2t = pow2(t);

    // Main Hwang-Lin Static loop: extract blocks of size 2^t or insert single elements
    while (m != 0 && n != 0) {
        if (n < pow2t) {
            break;
        }

        int k = n - pow2t;
        if (k < 0) k = 0;

        // Case 1: entire block from b is greater than lastA.
        if (a[m - 1] < b[k]) {
            // Copy the block [k, n) from b into the result
            r_iter -= pow2t;
            std::copy(b.begin() + k, b.begin() + n, r_iter);
            n -= pow2t;
            continue;
        } else {
            // Case 2: need to insert lastA into the correct position within the block
            auto pos = std::upper_bound(
                b.begin() + k + 1, 
                b.begin() + n, 
                a[m - 1]
            );

            // Copy the tail of b from pos to n
            r_iter -= static_cast<int>(std::distance(pos, b.begin() + n));
            std::copy(pos, b.begin() + n, r_iter);
            
            // Insert lastA right before the copied tail
            --r_iter;
            *r_iter = a[m - 1];   

            n = static_cast<int>(std::distance(b.begin(), pos));
            m--;
        }
    }

    // Final merge for remaining elements in a and b, writing from back to front.
    auto a_it = a.begin() + m;
    auto b_it = b.begin() + n;

    // Merge tail segments in reverse order.
    while (a_it != a.begin() && b_it != b.begin()) {
        *--r_iter = (*std::prev(a_it) >= *std::prev(b_it))
            ? *--a_it
            : *--b_it;
    }

    // Copy any leftovers from a or b
    while (a_it != a.begin()) {
        *--r_iter = *--a_it;
    }
    while (b_it != b.begin()) {
        *--r_iter = *--b_it;
    }

    return results;
}


/*
 * Algorithm: Hwang–Lin Static Stable Merge
 *
 * Origin:
 *   Based on:
 *     Thanh M., The Design and Analysis of Algorithms For Sort and Merge using Compressions
 *     // Master's Thesis. – Concordia University, Montreal, Canada. – 1983. – pp.39–43.
 *   Enhancements:
 *     This implementation extends the original static merge to guarantee stability—
 *     i.e. when a[i] == b[j], elements from `a` always precede those from `b`.
 *
 * Implementation:
 *   Developer: Sergei Gorlov
 *
 * Parameters:
 *   IterContainer& a - container with a sorted sequence of smaller size.
 *                      Elements must be in ascending order.
 *                      IMPORTANT: The container must be accessed starting from its beginning.
 *
 *   IterContainer& b - container with a sorted sequence of larger size.
 *                      Elements must be in ascending order.
 *                      IMPORTANT: The container must be accessed starting from its beginning.
 *
 * Returns:
 *   IterContainer – new container of size a.size()+b.size(), sorted ascending,
 *                    with stable ordering: if a[i] == b[j], all from `a` come first.
 *
 * Notes:
 *   - Both inputs must be pre-sorted in non-decreasing order.
 *   - Stability is guaranteed even when input sizes vary.
 *   - The result is written from back to front to avoid extra memory moves.
 */

template <typename IterContainer>
IterContainer hwang_lin_static_stable_merge(IterContainer& a, IterContainer& b) {
    // Return the other container if one is empty.
    if (a.empty()) {
        return b;
    }
    if (b.empty()) {
        return a;
    } 

    // Record sizes of both input ranges.
    int m = static_cast<int>(a.size());
    int n = static_cast<int>(b.size());

    // Allocate output of combined size to avoid reallocations.
    IterContainer result(a.size() + b.size());
    
    // Initialize write iterator at the end of the output.
    auto r_iter = result.end();


    // Choose branch based on which input is larger.
    if (b.size() >= a.size()) {
        // Compute block size parameter t and block length 2^t.
        int t = static_cast<int>(std::floor(std::log2(static_cast<double>(n) / m)));
        int pow2t = pow2(t);

        // Main Hwang–Lin static merge loop.
        while (m != 0 && n != 0) {
            // If remaining b-block is smaller than pow2t, exit loop.
            if (n < pow2t) {
                break;
            }

            // Determine block start in b.
            int k = n - pow2t;
            if (k < 0) k = 0;

            // Case 1: block in b is strictly larger than last element of a.
            if (a[m - 1] <= b[k]) {
                // Copy entire block from b[k..n) to output.
                r_iter -= pow2t;
                std::copy(b.begin() + k, b.begin() + n, r_iter);
                n -= pow2t;
                continue;
            } else {
                // Case 2: insert last element of a into b before first larger element.
                auto pos = std::lower_bound(
                    b.begin() + k + 1, 
                    b.begin() + n, 
                    a[m - 1]
                );

                // Copy tail of b from pos..n to output.
                int tailSize = static_cast<int>(std::distance(pos, b.begin() + n));
                r_iter -= tailSize;
                std::copy(pos, b.begin() + n, r_iter);
                
                // Place a[m-1] immediately before the copied tail.
                *(--r_iter) = a[m - 1];

                // Update counts for next iteration.
                n = static_cast<int>(std::distance(b.begin(), pos));
                m--;
            }
        }
    }else{
        // Compute block size parameter t and block length 2^t.
        int t = static_cast<int>(std::floor(std::log2(static_cast<double>(m) / n)));
        int pow2t = pow2(t);

        // Main Hwang–Lin static merge loop (mirror case).
        while (m != 0 && n != 0) {
            // If remaining a-block is smaller than pow2t, exit loop.
            if (m < pow2t) {
                break;
            }

            // Determine block start in a.
            int k = m - pow2t;
            if (k < 0) k = 0;

            // Case 1: block in a is strictly larger than last element of b.
            if (b[n - 1] < a[k]) {
                // Copy entire block from a[k..m) to output.
                r_iter -= pow2t;
                std::copy(a.begin() + k, a.begin() + m, r_iter);
                m -= pow2t;
                continue;
            } else {
                // Case 2: insert last element of b into a before first larger element.
                auto pos = std::upper_bound(
                    a.begin() + k + 1, 
                    a.begin() + m, 
                    b[n - 1]
                );

                // Copy tail of a from pos..m to output.
                int tailSize = static_cast<int>(std::distance(pos, a.begin() + m));
                r_iter -= tailSize;
                std::copy(pos, a.begin() + m, r_iter);
                
                // Place b[n-1] immediately before the copied tail.
                *(--r_iter) = b[n - 1];

                // Update counts for next iteration.
                m = static_cast<int>(std::distance(a.begin(), pos));
                n--;
            }
        }
    }


    // Perform final reverse merge of remaining elements.
    auto a_it = a.begin() + m;
    auto b_it = b.begin() + n;

    // Merge tail segments in reverse order.
    while (a_it != a.begin() && b_it != b.begin()) {
        *--r_iter = (*std::prev(a_it) > *std::prev(b_it))
            ? *--a_it
            : *--b_it;
    }

    // Copy any leftovers from b.
    while (b_it != b.begin()) {
        *--r_iter = *--b_it;
    }
    // Copy any leftovers from a.
    while (a_it != a.begin()) {
        *--r_iter = *--a_it;
    }

    return result;
}


/*
 * Algorithm: Hwang-Lin Dynamic Merge
 *
 * Publication:
 *   Thanh M. and Bui T. D., An Improvement of The Binary Merge Algorithm
 *   // Concordia University, Montreal, Canada. – 1982. – с.455-462
 *
 * Implementation:
 *   Developer: Sergei Gorlov
 *
 * Parameters:
 *   IterContainer& a - container with a sorted sequence of smaller size.
 *                      Elements must be in ascending order.
 *                      IMPORTANT: The container must be accessed starting from its beginning.
 *
 *   IterContainer& b - container with a sorted sequence of larger size.
 *                      Elements must be in ascending order.
 *                      IMPORTANT: The container must be accessed starting from its beginning.
 *
 * Return Value:
 *   IterContainer - merged container containing all elements from a and b, sorted in ascending order.
 *
 * Notes:
 *   - Containers must support the methods size(), reserve(), begin(), end(), insert().
 *   - It is assumed that the containers a and b are already sorted before calling the function.
 *
 */
template <typename IterContainer>
IterContainer hwang_lin_dynamic_merge(IterContainer& a, IterContainer& b) {
    int m = static_cast<int>(a.size());
    int n = static_cast<int>(b.size());

    // Swap a and b if a is larger than b
    if (m > n) {
        return hwang_lin_dynamic_merge(b, a);
    }

    size_t i = 0;         // index into A
    size_t j = 0;         // index into B

    b.reserve(n + m);

    while (m - i > 0) {
        int remainingA = m - i;
        int remainingB = n - j;

        int d = (remainingB > remainingA)
            ? static_cast<int>(std::floor(std::log2(static_cast<double>(remainingB) / (remainingA))))
            : 0;

        int pow2d = pow2(d);

        int c1 = pow2d;
        int c2 = (17 * pow2d) / 14;
        int c3 = ((12 * pow2d) / 7) - 1;
        int c4 = ((41 * pow2d) / 28) - 1;

        if (m - i < 4) {
            for (; i < m; i++) {
                auto pos = std::upper_bound(b.begin() + j, b.end(), a[i]);
                b.insert(pos, a[i]);
                n = static_cast<int>(b.size());
            }
            break;
        }

        auto a1 = a[i];
        auto a2 = a[i + 1];
        auto a3 = a[i + 2];
        auto a4 = a[i + 3];

        // NODE A.
        if (a1 > b[j + c1 - 1]) {
            j += c1;
            continue;
        }
        // NODE B.
        if (a2 > b[j + c2 - 1]) {
            auto pos = std::upper_bound(b.begin() + j, b.begin() + j + c1, a1);
            b.insert(pos, a1);

            i++;
            j += c2;
            continue;
        }
        // NODE C.
        if (a3 > b[j + c3 - 1]) {
            {
                auto pos = std::upper_bound(b.begin() + j, b.begin() + j + c2, a1);
                b.insert(pos, a1);
            }
            {
                auto pos = std::upper_bound(b.begin() + j, b.begin() + j + c2 + 1, a2);
                b.insert(pos, a2);
            }

            i += 2;
            j += c3;
            continue;
        }
        // NODE D.
        if (a4 > b[j + c4 - 1]) {
            {
                auto pos = std::upper_bound(b.begin() + j, b.begin() + j + c3, a1);
                b.insert(pos, a1);
            }
            {
                auto pos = std::upper_bound(b.begin() + j, b.begin() + j + c3 + 1, a2);
                b.insert(pos, a2);
            }
            {
                auto pos = std::upper_bound(b.begin() + j, b.begin() + j + c3 + 2, a3);
                b.insert(pos, a3);
            }

            i += 3;
            j += c4;
            continue;
        }
        // NODE E.
        else {
            auto pos = std::upper_bound(b.begin() + j, b.begin() + j + c4, a1);
            b.insert(pos, a1);

            pos = std::upper_bound(b.begin() + j, b.begin() + j + c4 + 1, a2);
            b.insert(pos, a2);

            pos = std::upper_bound(b.begin() + j, b.begin() + j + c4 + 2, a3);
            b.insert(pos, a3);

            pos = std::upper_bound(b.begin() + j, b.begin() + j + c4 + 3, a4);
            b.insert(pos, a4);

            int annexed = static_cast<int>(std::distance(b.begin() + j, pos));

            i += 4;
            j += annexed;
            n = static_cast<int>(b.size());
            continue;
        }
    }

    return b;
}

// Fractile insertion (Minimean merging and sorting: An Algorithm, R. Michael Tanner)
template <typename IterContainer>
void fractile_insertion_alg(typename IterContainer::const_iterator a_begin, int m, IterContainer& b) {
    // Step 1
    int n = static_cast<int>(std::distance(b.begin(), b.end()));
    int f = static_cast<int>(std::floor(static_cast<double>(m) / 2.0));
    int k = static_cast<int>(std::floor(static_cast<double>(n) * (static_cast<double>(f)/(static_cast<double>(m) + 1.0))));
    int alpha = static_cast<int>(std::floor((1.0/2.0) * std::log2(static_cast<double>(n) * ((1.0 + static_cast<double>(n))/static_cast<double>(m))) - 1.3));
    int delta = std::pow(2, alpha);

    // Step 2
    if (*(a_begin + f) > *(b.begin() + k)) {
        int prev_k;
        do {
            prev_k = k;
            k += delta;
        } while ((k < n) && (*(a_begin + f) >= *(b.begin() + k)));
        if (k >= n) {
            binary_insert(b, b.begin() + prev_k, b.begin() + n, *(a_begin + f));
        } else {
            binary_insert(b, b.begin() + prev_k, b.begin() + k, *(a_begin + f));
        }

        if (m > 2) {
            fractile_insertion_alg(a_begin, f, b); // left part
            fractile_insertion_alg(a_begin + f + 1, m - f - 1, b); // right part
        } else if (m == 2) {
            fractile_insertion_alg(a_begin, f, b); // left part
        }
        return;
    } else {
        int prev_k;
        do {
            prev_k = k;
            k -= delta;
        } while ((k >= 0) && (*(a_begin + f) <= *(b.begin() + k)));
        if (k < 0) {
            binary_insert(b, b.begin(), b.begin() + prev_k, *(a_begin + f));
        } else {
            binary_insert(b, b.begin() + k, b.begin() + prev_k, *(a_begin + f));
        }

        if (m > 2) {
            fractile_insertion_alg(a_begin, f, b); // left part
            fractile_insertion_alg(a_begin + f + 1, m - f - 1, b); // right part
        } else if (m == 2) {
            fractile_insertion_alg(a_begin, f, b); // left part
        }
        return;
    }
}

/*
 * Algorithm: Fractile insertion
 *
 * Publication:
 *   Tanner, R. M. Minimean Merging and Sorting: An Algorithm / R. M. Tanner //
 *   SIAM Journal on Computing. — 1978. — Vol. 7, No. 1. — с.1–36 — DOI: 10.1137/0207002.
 *
 * Implementation:
 *   Developer: Igor Stikentzin
 *
 * Parameters:
 *   IterContainer& a - container with a sorted sequence of smaller size.
 *                      Elements must be in ascending order.
 *                      IMPORTANT: The container must be accessed starting from its beginning.
 *
 *   IterContainer& b - container with a sorted sequence of larger size.
 *                      Elements must be in ascending order.
 *                      IMPORTANT: The container must be accessed starting from its beginning.
 *
 * Return Value:
 *   IterContainer - merged container containing all elements from a and b, sorted in ascending order.
 *
 * Notes:
 *   - Containers must support the methods size(), reserve(), begin(), end(), insert().
 *   - It is assumed that the containers a and b are already sorted before calling the function.
 *
 */
template <typename IterContainer>
IterContainer fractile_insertion_merge(const IterContainer& a, const IterContainer& b) {
    auto [m, n] = std::array<const int, 2>{{static_cast<int>(std::distance(a.begin(), a.end())), static_cast<int>(std::distance(b.begin(), b.end()))}};
    IterContainer r; // Resulting vector
    r.reserve(m + n);

    if (m <= n) {
        std::copy(b.begin(), b.end(), std::back_inserter(r)); // copy b to result
        fractile_insertion_alg(a.begin(), m, r);
    } else {
        std::copy(a.begin(), a.end(), std::back_inserter(r)); // copy a to result
        fractile_insertion_alg(b.begin(), n, r);
    }

    return r;
}

// SymMerge Algorithm (On a Simple and Stable Merging Algorithm, Pok-Son Kim, Arne Kutzner)
template <typename IterContainer>
void simple_kim_kutzner_alg(IterContainer begin, IterContainer separator, IterContainer end) {
    using diff_t = typename std::iterator_traits<IterContainer>::difference_type;
    diff_t left_size = std::distance(begin, separator);  // |u|
    diff_t right_size = std::distance(separator, end);   // |v|

    // trivial case
    if (left_size == 0 || right_size == 0) return;

    // if |u| or |v| equal 1
    if (left_size == 1) {
        IterContainer it = std::lower_bound(separator, end, *begin);
        std::rotate(begin, std::next(begin), it);
        return;
    }
    if (right_size == 1) {
        IterContainer it = std::upper_bound(begin, separator, *separator);
        std::rotate(it, separator, std::next(separator));
        return;
    }

    // general case
    diff_t total_size = left_size + right_size;
    diff_t mid_off = total_size / 2;
    IterContainer mid = std::next(begin, mid_off);
    diff_t n_off = mid_off + left_size;
    IterContainer n = std::next(begin, n_off);

    diff_t low, high;
    if (left_size <= mid_off) {
        low  = 0;
        high = left_size;
    } else {
        low  = n_off - total_size;
        high = mid_off;
    }

    while (low < high) {
        diff_t t = (low + high) / 2;
        IterContainer l = std::next(begin, t);
        IterContainer r = std::prev(n, t + 1);
        if (!(*r < *l)) {
            low = t + 1;
        } else {
            high = t;
        }
    }

    diff_t s_off = low;
    diff_t e_off = n_off - s_off;
    IterContainer s = std::next(begin, s_off);
    IterContainer e = std::next(begin, e_off);

    if (s_off < left_size && left_size < e_off) {
        std::rotate(s, separator, e);
    }

    if (s_off > 0 && s_off < mid_off) {
        simple_kim_kutzner_alg(begin, s, mid);
    }
    if (e_off > mid_off && e_off < total_size) {
        simple_kim_kutzner_alg(mid, e, end);
    }
}

/*
 * Algorithm: SymMerge
 *
 * Publication:
 *   Kim P.-S., Kutzner A. Stable Minimum Storage Merging by Symmetric Comparisons //
 *   Algorithms – ESA 2004: 12th Annual European Symposium on Algorithms, Bergen, Norway, 14–17 September 2004: Proceedings /
 *   eds. S. Albers, T. Radzik. – Berlin ; Heidelberg : Springer-Verlag, 2004. 
 *   – (Lecture Notes in Computer Science ; vol. 3221). – p. 714–723. – DOI: 10.1007/978-3-540-30140-0_63.
 *
 * Implementation:
 *   Developer: Igor Stikentzin
 *
 * Parameters:
 *   IterContainer& a - container with a sorted sequence of smaller size.
 *                      Elements must be in ascending order.
 *                      IMPORTANT: The container must be accessed starting from its beginning.
 *
 *   IterContainer& b - container with a sorted sequence of larger size.
 *                      Elements must be in ascending order.
 *                      IMPORTANT: The container must be accessed starting from its beginning.
 *
 * Return Value:
 *   IterContainer - merged container containing all elements from a and b, sorted in ascending order.
 *
 * Notes:
 *   - Containers must support the methods size(), reserve(), begin(), end(), insert().
 *   - It is assumed that the containers a and b are already sorted before calling the function.
 *
 */
template <typename IterContainer>
IterContainer simple_kim_kutzner_merge(IterContainer& a, IterContainer& b) {
    auto orig_a_size = a.size();
    a.insert(a.end(),
                std::make_move_iterator(b.begin()),
                std::make_move_iterator(b.end()));
    b.clear();
    simple_kim_kutzner_alg(a.begin(), std::next(a.begin(), orig_a_size), a.end());
    return a;
}

template <typename ContainerIter>
void hwang_lin_static_kutzner(ContainerIter begin, ContainerIter separator, ContainerIter end) {
    using value_t = std::iter_value_t<ContainerIter>;
    using diff_t  = typename std::iterator_traits<ContainerIter>::difference_type;

    diff_t m = std::distance(begin, separator);
    diff_t n = std::distance(separator, end);

    if (m == 0 || n == 0) return; // trivial case

    std::vector<value_t> buffer;
    if (m > n) {
        std::swap(m, n);
        buffer.resize(m);
        std::copy(separator, end, buffer.begin());   // copy A to buffer
    } else {
        buffer.resize(m);
        std::copy(begin, separator, buffer.begin()); // copy A to buffer
        std::copy(separator, end, begin);            // move B to left
    }

    diff_t t = static_cast<diff_t>(std::ceil(std::log2(static_cast<double>(n) / static_cast<double>(m))));
    diff_t pow2t = static_cast<diff_t>(pow2(t));

    // Main Hwang-Lin Static loop: extract blocks of size 2^t or insert single elements
    while ((m && n) && (n >= pow2t)) {
        diff_t k = n - pow2t;

        const value_t& last_a = buffer[m - 1];

        // Case 1: entire block from b is greater than last_a.
        if (last_a < *(begin + k)) {
            // Copy the block [k, n) from b into the result
            end -= pow2t;
            std::copy_backward(begin + k, begin + n, end + pow2t);
            n -= pow2t;
            continue;
        } else {
            // Case 2: need to insert last_a into the correct position within the block
            ContainerIter pos = std::upper_bound(
                begin + k + 1,
                begin + n,
                last_a
            );

            // Copy the tail of b from pos to n
            diff_t tail_size = std::distance(pos, begin + n);
            end -= tail_size;
            std::copy_backward(pos, begin + n, end + tail_size);
            
            // Insert last_a right before the copied tail
            *(--end) = last_a;

            n = std::distance(begin, pos);
            m--;
        }
    }

    // Final merge: A (in buffer[0..m)) and B[0..n)
    // Merge into [end - m - n, end)

    ContainerIter write = end;
    diff_t i = m - 1;
    diff_t j = n - 1;

    while (i >= 0 && j >= 0) {
        if (buffer[i] > *(begin + j)) {
            *(--write) = buffer[i--];
        } else {
            *(--write) = *(begin + j--);
        }
    }
    while (i >= 0) *(--write) = buffer[i--];
    while (j >= 0) *(--write) = *(begin + j--);

    return;
}

template <typename IterContainer>
IterContainer hwang_lin_static_kutzner_merge(IterContainer& a, IterContainer& b) {
    auto orig_a_size = a.size();
    auto orig_b_size = b.size();

    if (orig_a_size >= orig_b_size) {
        a.insert(a.end(),
                std::make_move_iterator(b.begin()),
                std::make_move_iterator(b.end()));
        b.clear();
        hwang_lin_static_kutzner(a.begin(), std::next(a.begin(), orig_a_size), a.end());
        return a;
    }

    b.insert(b.end(),
                std::make_move_iterator(a.begin()),
                std::make_move_iterator(a.end()));
    a.clear();
    hwang_lin_static_kutzner(b.begin(), std::next(b.begin(), orig_b_size), b.end());
    return b;
}

// Unstable Core Algorithm (On optimal and efficient in place merging, Pok-Son Kim, Arne Kutzner)
template <typename ContainerIter>
void unstable_core_kim_kutzner(ContainerIter begin, ContainerIter separator, ContainerIter end) {
    using value_t = std::iter_value_t<ContainerIter>;
    using diff_t  = typename std::iterator_traits<ContainerIter>::difference_type;

    diff_t m = std::distance(begin, separator);
    diff_t n = std::distance(separator, end);

    // trivial case
    if (m == 0 || n == 0) return;

    diff_t delta = 0;
    // diff_t k = static_cast<diff_t>(std::floor(std::sqrt(static_cast<double>(m))));
    diff_t k = std::min(m, static_cast<diff_t>(std::floor(std::sqrt(static_cast<double>(m)) * 16.0)));
    ContainerIter block_end = m % k == 0 ? std::next(begin, k) : std::next(begin, m % k);

    while (true) {
        ContainerIter b = std::lower_bound(separator, end, *(block_end - 1));
        ContainerIter to = b - std::distance(block_end, separator);
    
        if (to > separator) {
            std::rotate(block_end - 1, separator, b);
        } else {
            std::rotate(block_end - 1, separator, b);
            delta = (std::distance(separator, b) + delta) % k;
        }

        hwang_lin_static_kutzner(begin, block_end - 1, to - 1);

        separator = b;
        begin = to;

        if (!(begin < separator)) break;

        ContainerIter t = begin + (k - delta);
        ContainerIter e = separator - delta;
        ContainerIter start_min;

        if (delta > 0) {
            start_min = search_minimal_block(k, t, e, e);
        } else {
            start_min = search_minimal_block(k, begin + k, e, begin);
            t = begin;
        }

        if (start_min == e) {
            if (!std::equal(t, t + delta, e)) {
                block_swap(t, e, delta);
                std::rotate(begin, t, begin + k);
            }
        } else if (start_min != t) {
            if (!std::equal(t, t + k, start_min)) {
                block_swap(t, start_min, k);
                std::rotate(begin, t, t + k);
            }
        }

        block_end = begin + k;
    }
}

/*
 * Algorithm: Unstable Core Algorithm
 *
 * Publication:
 *   Kim P.-S., Kutzner A. On optimal and efficient in place merging //
 *   SOFSEM 2006: Theory and Practice of Computer Science, Merín, Czech Republic, January 21–27, 2006 /
 *   Eds. J. Wiedermann et al. – Berlin: Springer, 2006.
 *   – (Lecture Notes in Computer Science; Vol. 3831). – p. 350–359. – DOI: 10.1007/11611257_33.
 *
 * Implementation:
 *   Developer: Igor Stikentzin
 *
 * Parameters:
 *   IterContainer& a - container with a sorted sequence of smaller size.
 *                      Elements must be in ascending order.
 *                      IMPORTANT: The container must be accessed starting from its beginning.
 *
 *   IterContainer& b - container with a sorted sequence of larger size.
 *                      Elements must be in ascending order.
 *                      IMPORTANT: The container must be accessed starting from its beginning.
 *
 * Return Value:
 *   IterContainer - merged container containing all elements from a and b, sorted in ascending order.
 *
 * Notes:
 *   - Containers must support the methods size(), reserve(), begin(), end(), insert().
 *   - It is assumed that the containers a and b are already sorted before calling the function.
 *
 */
template <typename IterContainer>
IterContainer unstable_core_kim_kutzner_merge(IterContainer& a, IterContainer& b) {
    auto orig_a_size = a.size();
    a.insert(a.end(),
                std::make_move_iterator(b.begin()),
                std::make_move_iterator(b.end()));
    b.clear();
    unstable_core_kim_kutzner(a.begin(), std::next(a.begin(), orig_a_size), a.end());
    return a;
}
