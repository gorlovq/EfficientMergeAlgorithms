/*
 * Authors: Sergei Gorlov and Igor Stikentzin.
 * Description: Implements a collection of efficient merge algorithms,
 */

#pragma once

#include <iterator>
#include <array>
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

    // Pre-allocate result to avoid reallocations during merge.
    IterContainer results(a.size() + b.size());
    
    typename IterContainer::iterator r_iter = results.begin(); // write-pointer starting from the front.

    while (m - i > 0) {
        int remainingA = m - i;
        int remainingB = n - j;

        int d = (remainingB > remainingA)
            ? static_cast<int>(std::floor(std::log2(static_cast<double>(remainingB) / (remainingA))))
            : 0;

        if (d <= 0) {
            break;
        }

        int pow2d = pow2(d);

        int c1 = pow2d;
        int c2 = (17 * pow2d) / 14;
        int c3 = ((12 * pow2d) / 7) - 1;
        int c4 = ((41 * pow2d) / 28) - 1;

        if (remainingA < 4) {
            break;
        }

        // Ensure we don't access out of bounds
        if (i + 3 >= m || j + c4 >= n) {
            break;
        }

        auto a1 = a[i];
        auto a2 = a[i + 1];
        auto a3 = a[i + 2];
        auto a4 = a[i + 3];

        // NODE A.
        if (j + c1 <= n && a1 > b[j + c1 - 1]) {
            std::copy(b.begin() + j, b.begin() + j + c1, r_iter);
            r_iter += c1;
            j += c1;
            continue;
        }

        // NODE B.
        if (j + c2 <= n && a2 > b[j + c2 - 1]) {
            // Find position for a1
            auto pos = std::upper_bound(b.begin() + j, b.begin() + j + c1, a1);
            
            // Copy elements from b before a1
            std::copy(b.begin() + j, pos, r_iter);
            r_iter += std::distance(b.begin() + j, pos);
            
            // Insert a1
            *r_iter++ = a1;
            
            // Copy remaining elements from b
            std::copy(pos, b.begin() + j + c2, r_iter);
            r_iter += std::distance(pos, b.begin() + j + c2);
            
            i++;
            j += c2;
            continue;
        }

        // NODE C.
        if (j + c3 <= n && a3 > b[j + c3 - 1]) {
            // Find position for a1
            auto pos1 = std::upper_bound(b.begin() + j, b.begin() + j + c2, a1);
            
            // Copy elements from b before a1
            std::copy(b.begin() + j, pos1, r_iter);
            r_iter += std::distance(b.begin() + j, pos1);
            
            // Insert a1
            *r_iter++ = a1;
            
            // Find position for a2
            auto pos2 = std::upper_bound(pos1, b.begin() + j + c2 + 1, a2);
            
            // Copy elements between a1 and a2
            std::copy(pos1, pos2, r_iter);
            r_iter += std::distance(pos1, pos2);
            
            // Insert a2
            *r_iter++ = a2;
            
            // Copy remaining elements from b
            std::copy(pos2, b.begin() + j + c3, r_iter);
            r_iter += std::distance(pos2, b.begin() + j + c3);
            
            i += 2;
            j += c3;
            continue;
        }

        // NODE D.
        if (j + c4 <= n && a4 > b[j + c4 - 1]) {
            // Find position for a1
            auto pos1 = std::upper_bound(b.begin() + j, b.begin() + j + c3, a1);
            
            // Copy elements from b before a1
            std::copy(b.begin() + j, pos1, r_iter);
            r_iter += std::distance(b.begin() + j, pos1);
            
            // Insert a1
            *r_iter++ = a1;
            
            // Find position for a2
            auto pos2 = std::upper_bound(pos1, b.begin() + j + c3 + 1, a2);
            
            // Copy elements between a1 and a2
            std::copy(pos1, pos2, r_iter);
            r_iter += std::distance(pos1, pos2);
            
            // Insert a2
            *r_iter++ = a2;
            
            // Find position for a3
            auto pos3 = std::upper_bound(pos2, b.begin() + j + c3 + 2, a3);
            
            // Copy elements between a2 and a3
            std::copy(pos2, pos3, r_iter);
            r_iter += std::distance(pos2, pos3);
            
            // Insert a3
            *r_iter++ = a3;
            
            // Copy remaining elements from b
            std::copy(pos3, b.begin() + j + c4, r_iter);
            r_iter += std::distance(pos3, b.begin() + j + c4);
            
            i += 3;
            j += c4;
            continue;
        }

        // NODE E.
        // Find position for a1
        auto pos1 = std::upper_bound(b.begin() + j, b.begin() + j + c4, a1);
        
        // Copy elements from b before a1
        std::copy(b.begin() + j, pos1, r_iter);
        r_iter += std::distance(b.begin() + j, pos1);
        
        // Insert a1
        *r_iter++ = a1;
        
        // Find position for a2
        auto pos2 = std::upper_bound(pos1, b.begin() + j + c4 + 1, a2);
        
        // Copy elements between a1 and a2
        std::copy(pos1, pos2, r_iter);
        r_iter += std::distance(pos1, pos2);
        
        // Insert a2
        *r_iter++ = a2;
        
        // Find position for a3
        auto pos3 = std::upper_bound(pos2, b.begin() + j + c4 + 2, a3);
        
        // Copy elements between a2 and a3
        std::copy(pos2, pos3, r_iter);
        r_iter += std::distance(pos2, pos3);
        
        // Insert a3
        *r_iter++ = a3;
        
        // Find position for a4
        auto pos4 = std::upper_bound(pos3, b.begin() + j + c4 + 3, a4);
        
        // Copy elements between a3 and a4
        std::copy(pos3, pos4, r_iter);
        r_iter += std::distance(pos3, pos4);
        
        // Insert a4
        *r_iter++ = a4;
        
        // Copy remaining elements from b
        std::copy(pos4, b.begin() + j, r_iter);
        r_iter += std::distance(pos4, b.begin() + j);
        
        i += 4;
        j += std::distance(b.begin() + j, pos4);
    }

    // Handle remaining elements from a
    while (i < m) {
        auto pos = std::upper_bound(b.begin() + j, b.end(), a[i]);
        
        // Copy elements from b before a[i]
        std::copy(b.begin() + j, pos, r_iter);
        r_iter += std::distance(b.begin() + j, pos);
        
        // Insert a[i]
        *r_iter++ = a[i];
        
        j = std::distance(b.begin(), pos);
        i++;
    }

    // Copy any remaining elements from b
    if (j < n) {
        std::copy(b.begin() + j, b.end(), r_iter);
    }

    return results;
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
    }

    // Step 3
    if (*(a_begin + f) < *(b.begin() + k)) {
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

    // Not considered in original paper case, when A[f] == B[k]
    if (*(a_begin + f) == *(b.begin() + k)) {
        b.insert(b.begin() + k, *(a_begin + f));

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
template <typename ContainerIter>
void simple_kim_kutzner_alg(ContainerIter begin, ContainerIter separator, ContainerIter end) {
    using diff_t = typename std::iterator_traits<ContainerIter>::difference_type;
    diff_t left_size = std::distance(begin, separator);  // |u|
    diff_t right_size = std::distance(separator, end);   // |v|

    // trivial case
    if (left_size == 0 || right_size == 0) return;

    // if |u| or |v| equal 1
    if (left_size == 1) {
        ContainerIter it = std::lower_bound(separator, end, *begin);
        std::rotate(begin, std::next(begin), it);
        return;
    }
    if (right_size == 1) {
        ContainerIter it = std::upper_bound(begin, separator, *separator);
        std::rotate(it, separator, std::next(separator));
        return;
    }

    // general case
    diff_t total_size = left_size + right_size;
    diff_t mid_off = total_size / 2;
    ContainerIter mid = std::next(begin, mid_off);
    diff_t n_off = mid_off + left_size;
    ContainerIter n = std::next(begin, n_off);

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
        ContainerIter l = std::next(begin, t);
        ContainerIter r = std::prev(n, t + 1);
        if (!(*r < *l)) {
            low = t + 1;
        } else {
            high = t;
        }
    }

    diff_t s_off = low;
    diff_t e_off = n_off - s_off;
    ContainerIter s = std::next(begin, s_off);
    ContainerIter e = std::next(begin, e_off);

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
