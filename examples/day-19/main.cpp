#include "joint.hpp"

#include <iostream>

int main() {
    Joint shoulder{"shoulder", -90.0, 120.0};
    shoulder.set_position(15.0).set_position(30.0);

    const Joint& observed{shoulder};
    std::cout << observed.name() << ": " << observed.position() << '\n';

    Joint wrist{"wrist"};
    std::cout << wrist.name() << " limits: " << wrist.lower_limit() << ", "
              << wrist.upper_limit() << '\n';
}
