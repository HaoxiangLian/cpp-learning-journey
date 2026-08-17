#include <iostream>

int main() {
    const int sensor_id{12};
    int sample_count{0};
    double position_meter{1.25};

    std::cout << "sensor: " << sensor_id << '\n';
    std::cout << "sample " << sample_count << " position: "
              << position_meter << " m\n";

    sample_count = 1;
    position_meter = 1.5;

    std::cout << "sample " << sample_count << " position: "
              << position_meter << " m\n";
    return 0;
}
