#include <iostream>

int main() {
    constexpr double target_angle{90.0};
    double current_angle{84.0};
    bool sensor_ready{true};

    for (int cycle{0}; cycle < 4; ++cycle) {
        const double error{target_angle - current_angle};
        const bool target_reached{
            sensor_ready && error <= 2.0 && error >= -2.0};

        std::cout << "cycle " << cycle << ": ";
        if (!sensor_ready) {
            std::cout << "STOP\n";
        } else if (target_reached) {
            std::cout << "REACHED\n";
        } else if (error > 0.0) {
            std::cout << "MOVE_POSITIVE\n";
        } else {
            std::cout << "MOVE_NEGATIVE\n";
        }

        current_angle += 2.0;
    }
    return 0;
}

