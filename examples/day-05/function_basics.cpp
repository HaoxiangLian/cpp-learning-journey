#include <iostream>

double position_error(double target, double current);

int main() {
    const double target_angle{90.0};
    const double current_angle{82.5};
    const double error{position_error(target_angle, current_angle)};

    std::cout << "error: " << error << '\n';
    return 0;
}

double position_error(double target, double current) {
    return target - current;
}

