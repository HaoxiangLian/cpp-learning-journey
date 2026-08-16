#include <iostream>

int main() {
    int joint_id{2};
    double target_angle{90.0};
    double current_angle{82.5};
    double error{target_angle - current_angle};

    std::cout << "joint: " << joint_id << '\n';
    std::cout << "first error: " << error << '\n';

    current_angle = 88.0;
    error = target_angle - current_angle;
    std::cout << "second error: " << error << '\n';
    return 0;
}

