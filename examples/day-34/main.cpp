#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>

double clamp_position(double value, double low, double high) {
    if (low > high) {
        throw std::invalid_argument("low must not exceed high");
    }
    if (value < low) {
        return low;
    }
    if (value > high) {
        return high;
    }
    return value;
}

void expect_near(double actual, double expected, const std::string& name) {
    if (std::fabs(actual - expected) > 1e-9) {
        throw std::runtime_error(name + " failed");
    }
    std::cout << "[PASS] " << name << "\n";
}

int main() {
    expect_near(clamp_position(0.2, -1.0, 1.0), 0.2, "inside");
    expect_near(clamp_position(-2.0, -1.0, 1.0), -1.0, "below");
    expect_near(clamp_position(2.0, -1.0, 1.0), 1.0, "above");
    expect_near(clamp_position(1.0, -1.0, 1.0), 1.0, "boundary");
}
