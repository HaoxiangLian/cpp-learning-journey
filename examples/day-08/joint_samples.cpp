#include <array>
#include <iostream>
#include <string>
#include <vector>

int main() {
    const std::string robot_name{"UR5e"};
    const std::array<double, 3> home_angles{0.0, -90.0, 90.0};
    std::vector<double> samples{1.0, 2.0, 3.0};
    samples.push_back(4.0);

    double sum{};
    for (const double sample : samples) {
        sum += sample;
    }
    const double average{sum / static_cast<double>(samples.size())};

    std::cout << "robot: " << robot_name << '\n';
    std::cout << "home joint 2: " << home_angles[1] << '\n';
    std::cout << "sample count: " << samples.size() << '\n';
    std::cout << "average: " << average << '\n';
    return 0;
}

