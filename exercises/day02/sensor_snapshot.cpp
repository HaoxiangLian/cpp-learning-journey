#include <iostream>

int main() {
    // TODO: 按题目要求选择类型和初始值。
    const int sensor_id{0};
    int sample_count{0};
    double position_meter{0.0};

    std::cout << "sensor: " << sensor_id << '\n';
    std::cout << "sample " << sample_count << " position: "
              << position_meter << " m\n";

    // TODO: 对允许变化的对象赋值。

    std::cout << "sample " << sample_count << " position: "
              << position_meter << " m\n";
    return 0;
}
