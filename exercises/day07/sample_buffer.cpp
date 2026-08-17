#include <cstddef>
#include <iostream>
#include <string>

int main() {
    std::string sensor{"lidar"};
    constexpr std::size_t count{4};

    // TODO: 创建并初始化动态数组。
    int* samples{nullptr};

    // TODO: 在 [0, count) 内求和。
    int sum{0};

    // TODO: 匹配释放并将 samples 设为 nullptr。

    std::cout << sensor << " sum: " << sum << '\n';
    std::cout << std::boolalpha << "released: " << (samples == nullptr) << '\n';
}
