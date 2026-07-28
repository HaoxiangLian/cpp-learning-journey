#include <iostream>
#include <string>

int main() {
    std::string robot_name{};
    int joint_count{};
    double target_angle{};

    std::cout << "Robot name (no spaces): ";
    std::cin >> robot_name;

    std::cout << "Joint count: ";
    std::cin >> joint_count;

    std::cout << "Target angle for each joint: ";
    std::cin >> target_angle;

    double total_target_angle{joint_count * target_angle};

    std::cout << "\nRobot: " << robot_name << '\n';
    std::cout << "Joints: " << joint_count << '\n';
    std::cout << "Target angle: " << target_angle << '\n';
    std::cout << "Total target angle: " << total_target_angle << '\n';

    return 0;
}
