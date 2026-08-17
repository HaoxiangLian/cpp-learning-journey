#include <iostream>

int main() {
    const int sensor_id{3};
    int sample_count{0};
    double temperature_celsius{21.75};

    std::cout << "sensor: " << sensor_id << '\n';
    std::cout << "sample " << sample_count << ": "
              << temperature_celsius << " C\n";

    sample_count = 1;
    temperature_celsius = 22.125;

    std::cout << "sample " << sample_count << ": "
              << temperature_celsius << " C\n";
    return 0;
}
