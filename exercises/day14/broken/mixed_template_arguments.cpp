// INTENTIONALLY INVALID: function-template deduction finds conflicting T types.
#include <iostream>

template <typename T>
T larger(T left, T right) {
    return left < right ? right : left;
}

int main() {
    const auto value = larger(3, 4.5);
    std::cout << value << '\n';
}
