#include "int_buffer.hpp"

#include <iostream>

int sum(const IntBuffer& buffer) {
    int result{0};
    for (std::size_t index{0}; index < buffer.size(); ++index) {
        result += buffer.at(index);
    }
    return result;
}

int main() {
    IntBuffer samples{3};
    samples.at(0) = 10;
    samples.at(1) = 20;
    samples.at(2) = 30;
    std::cout << "sum: " << sum(samples) << '\n';
}
