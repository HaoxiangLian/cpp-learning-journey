#include "linkage_demo.hpp"

#include <iostream>

int main() {
    int max_axes{3};
    const robot::ModeCode first_call{robot::current_mode()};
    const robot::ModeCode second_call{robot::current_mode()};

    std::cout << "local max axes: " << max_axes << '\n';
    std::cout << "shared max axes: " << robot::max_axes << '\n';
    std::cout << "shared mode: " << robot::active_mode << '\n';
    std::cout << "current mode calls: " << first_call << ", " << second_call << '\n';
    std::cout << "inline double: " << robot::double_mode(4) << '\n';
    std::cout << "file locals: " << robot::left_local_value() << ", "
              << robot::right_local_value() << '\n';
}
