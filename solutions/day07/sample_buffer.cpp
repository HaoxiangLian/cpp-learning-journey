#include <cstddef>
#include <iostream>
#include <string>

int main() {
    std::string sensor{"lidar"};
    constexpr std::size_t count{4};
    int* samples{new int[count]{3, 6, 9, 12}};

    int sum{0};
    for (std::size_t index{0}; index < count; ++index) {
        sum += samples[index];
    }

    delete[] samples;
    samples = nullptr;

    std::cout << sensor << " sum: " << sum << '\n';
    std::cout << std::boolalpha << "released: " << (samples == nullptr) << '\n';
}
