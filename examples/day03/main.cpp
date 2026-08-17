#include <iostream>

int main() {
    const double temperature_celsius{31.75};
    const int whole_temperature{static_cast<int>(temperature_celsius)};
    const bool sensor_ready{true};
    const bool alarm{sensor_ready && temperature_celsius >= 30.0};

    int accepted_samples{0};
    for (int sample_index{0}; sample_index < 4; ++sample_index) {
        if (sample_index == 1) {
            continue;
        }
        ++accepted_samples;
    }

    const int mode{2};

    std::cout << "whole temperature: " << whole_temperature << '\n';
    if (alarm) {
        std::cout << "alarm: on\n";
    } else {
        std::cout << "alarm: off\n";
    }
    std::cout << "accepted samples: " << accepted_samples << '\n';

    switch (mode) {
    case 1:
        std::cout << "mode: standby\n";
        break;
    case 2:
        std::cout << "mode: inspect\n";
        break;
    default:
        std::cout << "mode: unknown\n";
        break;
    }
}
