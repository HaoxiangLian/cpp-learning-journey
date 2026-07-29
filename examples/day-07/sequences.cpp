#include <cstddef>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

void print_angles(const std::vector<double>& angles);
double average_angle(const std::vector<double>& angles);

int main() {
    double home_angles[4]{0.0, 15.0, -20.0, 45.0};

    std::cout << "Raw array extent: " << std::size(home_angles) << '\n';
    std::cout << "home_angles[2]: " << home_angles[2] << '\n';

    std::string robot_name{"Arm-A"};
    std::cout << "Robot name: " << robot_name << '\n';
    std::cout << "Characters:";
    for (char character : robot_name) {
        std::cout << ' ' << character;
    }
    std::cout << '\n';

    std::vector<double> commanded_angles{10.0, 20.0, 30.0};
    std::cout << "Vector size before: " << commanded_angles.size() << '\n';

    commanded_angles.push_back(40.0);
    std::cout << "Vector size after: " << commanded_angles.size() << '\n';
    std::cout << std::boolalpha
              << "Capacity covers size: "
              << (commanded_angles.capacity() >= commanded_angles.size())
              << '\n';

    print_angles(commanded_angles);
    std::cout << "Average command: " << average_angle(commanded_angles) << '\n';

    std::size_t requested_index{4};
    if (requested_index < commanded_angles.size()) {
        std::cout << commanded_angles[requested_index] << '\n';
    } else {
        std::cout << "Index " << requested_index << " is outside [0, "
                  << commanded_angles.size() << ")\n";
    }

    return 0;
}

void print_angles(const std::vector<double>& angles) {
    std::cout << "Commands:";
    for (double angle : angles) {
        std::cout << ' ' << angle;
    }
    std::cout << '\n';
}

double average_angle(const std::vector<double>& angles) {
    if (angles.empty()) {
        return 0.0;
    }

    double total{0.0};
    for (double angle : angles) {
        total = total + angle;
    }

    return total / static_cast<double>(angles.size());
}
