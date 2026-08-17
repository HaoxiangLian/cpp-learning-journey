#include <iostream>

int main() {
    const double temperature_celsius{31.8};

    // 已给出第一步：显式转换。运行前先解释这里为什么得到 31。
    const int display_temperature{static_cast<int>(temperature_celsius)};

    const bool sensor_ready{true};
    // TODO: 把临时阈值改为题目要求的 30.0。
    const bool alarm{sensor_ready && temperature_celsius < 0.0};

    int processed_samples{0};
    // TODO: 遍历 0 到 4，跳过索引 2，其余计数。

    const int mode{2};

    std::cout << "display: " << display_temperature << '\n';
    if (alarm) {
        std::cout << "alarm: on\n";
    } else {
        std::cout << "alarm: off\n";
    }
    std::cout << "processed samples: " << processed_samples << '\n';

    // TODO: 增加 mode 1 和 mode 2 的 case。
    switch (mode) {
    default:
        std::cout << "mode: unknown\n";
        break;
    }
}
