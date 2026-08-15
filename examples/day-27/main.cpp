#include <iostream>

template <typename T>
T clamp_value(T value, T lower, T upper) {
    if (value < lower) return lower;
    if (upper < value) return upper;
    return value;
}

int main() {
    auto integer_result = clamp_value(12, 0, 10);
    auto double_result = clamp_value(12.5, 0.0, 10.0);
    std::cout << integer_result << ' ' << double_result << '\n';
}
