auto pow2 = [](int n) -> int {return 1 << n;}; // Raise a number to the power of 2 using a bitwise operator

// Insert element in contaiiner in specified range
template <typename IterContainer>
void binary_insert(
    IterContainer& c,
    typename IterContainer::const_iterator begin,
    typename IterContainer::const_iterator end,
    typename IterContainer::value_type element
) {
    auto inser_it = std::lower_bound(begin, end, element);
    c.insert(inser_it, element);
}