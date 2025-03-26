#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <algorithm>
#include <vector>
#include <list>
#include <array>
#include <cmath>
#include <functional>

#include "common.hpp"

/*
 * Algorithm: Hwang-Lin Static Merge
 *
 * Publication:
 *   Thanh M., The Design and Analysis of Algorithms For Sort and Merge using Compressions
 *   // Master’s Thesis. – Concordia University, Montreal, Canada. – 1983. – c.35-38.
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

std::vector<int> simple_binary_merge(const std::vector<int>& a, std::vector<int>& b) {
    int m = static_cast<int>(a.size()) - 1;
    int n = static_cast<int>(b.size()) - 1;

    while (m >= 0) {
        auto b_end = b.begin();
        std::advance(b_end, n + 1);

        // binary search for the insert position
        auto it = std::lower_bound(b.begin(), b_end, a[m]);

        n = static_cast<int>(std::distance(b.begin(), it)) + 1;

        b.insert(it, a[m]);
        m--;
    }

    return b;
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
IterContainer hwang_lin_knuth_merge(const IterContainer& a, const IterContainer& b) {
    auto [a_left, a_right, b_left, b_right] = std::array<typename IterContainer::const_iterator, 4>{{a.begin(), a.end(), b.begin(), b.end()}};
    IterContainer r(a.size() + b.size()); // Resulting vector

    typename IterContainer::iterator k_iter = r.end(); // Pointer for insertion

    auto [m, n, t, s] = std::array<int, 4>{{0, 0, 0, 0}};

    while (a_left != a_right && b_left != b_right) {
        // H1
        m = static_cast<int>(std::distance(a_left, a_right));
        n = static_cast<int>(std::distance(b_left, b_right));
        if (m > n) {
            t = std::floor(std::log2(m / n));
            s = pow2(t);

            // H4
            if (*(b_right - 1) < *(a_right - s)) {
                k_iter -= s;
                std::copy(a_right - s, a_right, k_iter); // copy elements from (a_right - s) to a_right to result vector
                a_right -= s;
                continue;
            }
        }
        else {
            t = std::floor(std::log2(n / m));
            s = pow2(t);

            // H2
            if (*(a_right - 1) < *(b_right - s)) {
                k_iter -= s;
                std::copy(b_right - s, b_right, k_iter); // copy elements from (b_right - s) to b_right to result vector
                b_right -= s;
                continue;
            }
        }

        // H3
        if (*(b_right - 1) < *(a_right - 1)) {
            *(--k_iter) = *(a_right - 1);
            a_right--;
            continue;
        }

        // H5
        if (*(a_right - 1) <= *(b_right - 1)) {
            *(--k_iter) = *(b_right - 1);
            b_right--;
            continue;
        }
    }

    if (a_left == a_right) {
        std::copy(b_left, b_right, r.begin());
    } else {
        std::copy(a_left, a_right, r.begin());
    }

    return r;
}

// Binary Merge Algorithm (https://github.com/rklaehn/binary-merge)
template <typename IterContainer>
IterContainer binary_merge(const IterContainer& a, const IterContainer& b) {
    IterContainer r(a.size() + b.size());

    auto a_begin = a.begin();
    auto b_begin = b.begin();
    auto r_begin = r.begin();

    // Определение вложенной функции binary_merge_recursive
    std::function<void(typename IterContainer::const_iterator&,
                       typename IterContainer::const_iterator&,
                       typename IterContainer::iterator&,
                       size_t, size_t)> binary_merge_recursive;

    binary_merge_recursive = [&](typename IterContainer::const_iterator& a_it,
                                 typename IterContainer::const_iterator& b_it,
                                 typename IterContainer::iterator& r_it,
                                 size_t an, size_t bn) {
        if (an == 0) {
            std::copy(b_it, b_it + bn, r_it);
            r_it += bn;
            b_it += bn;
            return;
        } else if (bn == 0) {
            std::copy(a_it, a_it + an, r_it);
            r_it += an;
            a_it += an;
            return;
        } else {
            // neither a nor b are 0
            size_t am = an / 2;
            auto a_mid = a_it + am;

            auto b_mid = std::lower_bound(b_it, b_it + bn, *a_mid);
            size_t bm = std::distance(b_it, b_mid);

            binary_merge_recursive(a_it, b_it, r_it, am, bm);

            if (b_mid != b_it + bn && *b_mid == *a_mid) {
                // same elements
                *r_it++ = *a_mid++;
                *r_it++ = *b_mid++;
                binary_merge_recursive(a_mid, b_mid, r_it, an - am - 1, bn - bm - 1);
            } else {
                // not found
                *r_it++ = *a_mid++;
                binary_merge_recursive(a_mid, b_mid, r_it, an - am - 1, bn - bm);
            }
        }
    };

    binary_merge_recursive(a_begin, b_begin, r_begin, a.size(), b.size());

    return r;
}

/*
 * Algorithm: Hwang-Lin Static Merge
 *
 * Publication:
 *   Thanh M., The Design and Analysis of Algorithms For Sort and Merge using Compressions
 *   // Master’s Thesis. – Concordia University, Montreal, Canada. – 1983. – c.39-43.
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
    int m = static_cast<int>(a.size());
    int n = static_cast<int>(b.size());

    b.reserve(m + n);

    int t = static_cast<int>(std::floor(std::log2(static_cast<double>(n) / m)));

    // Step 1: check m and for empty.
    while (m != 0 && n != 0) {
        // Step 2: Calculate t.

        int pow2t = pow2(t);

        if (n < pow2t) {
            break;
        }

        int k = n - pow2t;
        if (k < 0) k = 0;

        auto a_m = a[m - 1];

        // Step 3: Compare A[m] with B[n - 2^t + 1]
        if (a_m < b[k]) {
            n -= pow2t;
            continue;
        }else {
            // Step 5: Insert A[m] into B[n - 2^t + 2 : n]
            auto pos = std::upper_bound(b.begin() + k + 1, b.begin() + n, a_m);
            b.insert(pos, a_m);

            int q = static_cast<int>(std::distance(b.begin(), pos));

            n = q;
            m--;
        }
    }


    // Step 6: Insert elements of A into B using binary insertion
    for (int i = 0; i < m; ++i) {
        binary_insertion(b, a[i]);
    }

    return b;
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


#endif // ALGORITHMS_H
