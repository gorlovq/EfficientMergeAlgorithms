#ifndef COUNTING_INT_HPP
#define COUNTING_INT_HPP

class CountingInt {
public:
    int value;

    inline static long long comparisons = 0;

    CountingInt(int v = 0) : value(v) {}

    static void resetCounter() {
        comparisons = 0;
    }
};

// Only operator< increments the counter.
inline bool operator<(const CountingInt& lhs, const CountingInt& rhs) {
    ++CountingInt::comparisons;
    return lhs.value < rhs.value;
}

inline bool operator==(const CountingInt& lhs, const CountingInt& rhs) {
    ++CountingInt::comparisons;
    return lhs.value == rhs.value;
}

inline bool operator!=(const CountingInt& lhs, const CountingInt& rhs) {
    ++CountingInt::comparisons;
    return lhs.value != rhs.value;
}

inline bool operator>(const CountingInt& lhs, const CountingInt& rhs) {
    ++CountingInt::comparisons;
    return lhs.value > rhs.value;
}

inline bool operator<=(const CountingInt& lhs, const CountingInt& rhs) {
    ++CountingInt::comparisons;
    return lhs.value <= rhs.value;
}

inline bool operator>=(const CountingInt& lhs, const CountingInt& rhs) {
    ++CountingInt::comparisons;
    return lhs.value >= rhs.value;
}

#endif // COUNTING_INT_HPP
