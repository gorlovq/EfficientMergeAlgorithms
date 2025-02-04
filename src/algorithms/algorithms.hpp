#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <algorithm>
#include <vector>
#include <list>
#include <array>
#include <cmath>
#include <functional>

#include "common.hpp"

// Standard merging of 2 sorted arrays with n and m lenghts
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

// Hwang-Lin Merging Algorithm (Art of Computer Programming Volume 3 Sorting & Searching)
template <typename IterContainer>
IterContainer hla(const IterContainer& a, const IterContainer& b) {
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


template <typename IterContainer>
IterContainer hl_static(IterContainer& a, IterContainer& b) {
    int m = static_cast<int>(a.size());
    int n = static_cast<int>(b.size());

    b.reserve(m + n);

    while (m > 0) {
        // Step 1: Calculate t
        int t = static_cast<int>(std::floor(std::log2(static_cast<double>(n) / m)));

        // t < 0 When n is Less Than m
        // t = 0 When n is Equal to m
        if (n < pow2(t) || t <= 0) {
            // Step 6: Insert elements of A into B using binary insertion
            for (int i = 0; i < m; ++i) {
                binary_insertion(b, a[i]);
                ++n;
            }
            break;
        }

        // Step 3: Compare A[m-1] with B[n - 2^t]
        int k = n - pow2(t);

        if (a[m - 1] < b[k]) {
            n = k;
            continue;
        }else {
            // Step 5: Insert A[m-1] into B[k+1 to n]
            int q_start = k + 1;
            int q_end = n;
            auto it = std::upper_bound(b.begin() + q_start, b.begin() + q_end, a[m - 1]);
            int q = static_cast<int>(it - b.begin()) - 1;

            // Insert A[m-1] into B at position q + 1
            b.insert(b.begin() + q + 1, a[m - 1]);

            n = q;
            m -=  1;
        }
    } 

    return b;
}

template <typename IterContainer>
IterContainer hl_dynamic(IterContainer& A, IterContainer& B) {
    int m = static_cast<int>(A.size());
    int n = static_cast<int>(B.size());

    B.reserve(n + m);

    while (m > 0) {
        // Step 1: Compute d
        int d = (n > m && m > 0) 
            ? static_cast<int>(std::floor(std::log2(static_cast<double>(n) / m))) 
            : 0;

        int pow2d = pow2(d);

        int c1 = n - pow2d;
        int c2 = n - ((17 * pow2d) / 14);
        int c3 = n + 1 - ((12 * pow2d) / 7);
        int c4 = n + 1 - ((41 * pow2d) / 28);

        c1 = std::clamp(c1, 0, n - 1);
        c2 = std::clamp(c2, 0, n - 1);
        c3 = std::clamp(c3, 0, n - 1);
        c4 = std::clamp(c4, 0, n - 1);

        // Step 2: Compare A[m - 1] and B[c1]
        if (m >= 1 && c1 >= 0 && c1 < n && A[m - 1] < B[c1]) {
            n = c1;
            continue; // Go to Step 1
        } else if (m >= 2 && c2 >= 0 && c2 < n && A[m - 2] < B[c2]) {
            // Step 3: Compare A[m - 2] and B[c2]
            // Insert A[m - 1] into B[(c1 + 1):n]
            int insert_index = c1 + 1;
            if (insert_index >= B.size()) {
                B.push_back(A[m - 1]);
            } else {
                auto insert_pos = std::upper_bound(B.begin() + insert_index, B.begin() + n, A[m - 1]);
                B.insert(insert_pos, A[m - 1]);
            }

            n = c2; 
            m -= 1;

            continue; // Go to Step 1
        } else if (m >= 3 && c3 >= 0 && c3 < n && A[m - 3] < B[c3]) {
            // Step 4: Compare A[m - 3] and B[c3]
            // Merge A[m - 3 : m - 1] with B[c2 + 1 : n - 1]
            auto a_start = A.begin() + (m - 2);
            auto a_end = A.begin() + m;
            auto b_start = B.begin() + c2 + 1;
            auto b_end = B.begin() + n;

            // Ensure b_start is not beyond b_end
            b_start = (b_start > b_end) ? b_end : b_start;

            std::vector<typename IterContainer::value_type> temp;
            temp.reserve((a_end - a_start) + (b_end - b_start));

            std::merge(a_start, a_end, b_start, b_end, std::back_inserter(temp));

            // Replace B[c2 + 1 : n - 1] with merged temp
            B.erase(b_start, b_end);
            B.insert(B.begin() + c2 + 1, temp.begin(), temp.end());

            n = c3;
            m -= 2;

            continue; // Go to Step 1
        } else if (m >= 4 && c4 >= 0 && c4 <= n) {
            // Step 6: Merge A[m - 4 : m - 1] with B[c4 : n - 1]
            auto q_it = std::upper_bound(B.begin() + c4, B.begin() + n, A[m - 3]);
            int q = static_cast<int>(q_it - B.begin()) - 1;

            if (q_it == B.begin() + n) {
                // A[m - 4] is greater than all elements in B[c4:n - 1]
                // Append remaining elements of A to B
                B.insert(B.end(), A.begin() + m - 4, A.begin() + m);
                n += 4; // Update n with the number of elements added
                m -= 4;
                continue; // Go to Step 1
            } else {
                auto a_start = A.begin() + (m - 4);
                auto a_end = A.begin() + m;
                auto b_start = B.begin() + c4;
                auto b_end = B.begin() + n;

                std::vector<typename IterContainer::value_type> temp;
                temp.reserve((a_end - a_start) + (b_end - b_start));

                std::merge(a_start, a_end, b_start, b_end, std::back_inserter(temp));

                // Replace B[c4 : n - 1] with merged temp
                B.erase(b_start, b_end);
                B.insert(B.begin() + c4, temp.begin(), temp.end());

                n = q + 1;
                m -= 4;
                continue; // Go to Step 1
            }
        } else {
            // Handle remaining elements when m < 4
            for (int i = m - 1; i >= 0; --i) {
                auto insert_pos = std::upper_bound(B.begin(), B.begin() + n, A[i]);
                B.insert(insert_pos, A[i]);
                n += 1;
            }
            m = 0;
            break;
        }
    }

    return B;
}

#endif // ALGORITHMS_H