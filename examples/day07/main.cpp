#include <cstddef>
#include <iostream>
#include <string>

int main() {
    int readings[]{12, 15, 18};
    constexpr std::size_t reading_count{sizeof(readings) / sizeof(readings[0])};

    int reading_sum{0};
    for (std::size_t index{0}; index < reading_count; ++index) {
        reading_sum += readings[index];
    }

    const char sensor_name[]{"imu"};
    std::string task{"inspect"};
    task += "-done";

    constexpr std::size_t buffer_count{3};
    int* buffer{new int[buffer_count]{2, 4, 6}};
    int dynamic_sum{0};
    for (std::size_t index{0}; index < buffer_count; ++index) {
        dynamic_sum += buffer[index];
    }

    delete[] buffer;
    buffer = nullptr;

    std::cout << "reading sum: " << reading_sum << '\n';
    std::cout << "sensor: " << sensor_name << '\n';
    std::cout << "task: " << task << '\n';
    std::cout << "dynamic sum: " << dynamic_sum << '\n';
    std::cout << std::boolalpha << "released pointer reset: " << (buffer == nullptr) << '\n';
}
