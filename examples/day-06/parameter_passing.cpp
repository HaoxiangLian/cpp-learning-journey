#include <iostream>

double clamp(double value, double minimum, double maximum) {
    if (value < minimum) {
        return minimum;
    }
    if (value > maximum) {
        return maximum;
    }
    return value;
}

int clamp(int value, int minimum, int maximum) {
    if (value < minimum) {
        return minimum;
    }
    if (value > maximum) {
        return maximum;
    }
    return value;
}

void apply_offset(double& reading, double offset) {
    reading += offset;
}

void print_reading(const double& reading) {
    std::cout << "reading: " << reading << '\n';
}

int main() {
    double reading{2.5};
    apply_offset(reading, 0.75);
    print_reading(reading);

    const double limited_reading{clamp(reading, 0.0, 3.0)};
    const int limited_retries{clamp(5, 0, 3)};
    std::cout << "limited reading: " << limited_reading << '\n';
    std::cout << "limited retries: " << limited_retries << '\n';
    return 0;
}

