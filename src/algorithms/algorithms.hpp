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
    // Swap a and b if a is larger than b.
    if (a.size() > b.size()) {
        std::swap(a, b);
    }

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

#endif // ALGORITHMS_H
