#ifndef COUNTING_INT_HPP
#define COUNTING_INT_HPP

enum class Slice { A, B };

class CountingInt {
public:
    int value;
    Slice source;

    inline static long long comparisons = 0;

    CountingInt(int v = 0, Slice s = Slice::A) : value(v), source(s) {}

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
