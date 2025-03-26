#ifndef COUNTING_INT_HPP
#define COUNTING_INT_HPP

// Enum for indicating the source slice.
enum class Slice { A, B };

class CountingInt {
public:
    int value;      // The integer value of the element.
    Slice source;   // The source slice of the element (e.g., Slice::A for the first array, Slice::B for the second).
    int index;      // The index of the element in its input sequence.

    inline static long long comparisons = 0; // Static counter for tracking the number of comparisons.

    CountingInt(int v = 0, Slice s = Slice::A, int idx = 0) : value(v), source(s), index(idx) {}

    static void resetCounter() {
        comparisons = 0;
    }
};

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
