#include "reading.hpp"

#include <iostream>

int main() {
    const Reading force{7, 12.5};
    const Reading unknown{0, 3.0};

    std::cout << std::boolalpha;
    std::cout << "force valid: " << is_valid(force) << '\n';
    std::cout << "unknown valid: " << is_valid(unknown) << '\n';
}
