#include "int_buffer.hpp"

#include <iostream>

void print_first(IntBuffer buffer) {
    std::cout << "parameter copy: " << buffer[0] << '\n';
}

int main() {
    IntBuffer original{3};
    original[0] = 10;
    original[1] = 20;
    original[2] = 30;

    IntBuffer constructed{original};
    constructed[0] = 99;

    IntBuffer assigned{1};
    assigned = original;
    assigned[1] = 77;
    assigned = assigned;

    std::cout << "original: " << original[0] << ", " << original[1] << '\n';
    std::cout << "constructed: " << constructed[0] << '\n';
    std::cout << "assigned: " << assigned[1] << '\n';
    print_first(original);
}
