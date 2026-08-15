#include <iostream>

#include "robot/math.hpp"

int main() {
    std::cout << "clamped = "
              << robot::clamp_position(1.8, -1.0, 1.0)
              << '\n';
}
