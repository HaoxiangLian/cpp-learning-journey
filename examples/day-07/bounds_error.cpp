#include <iostream>
#include <vector>

int main() {
    std::vector<int> sensor_values{10, 20, 30};

    // Intentional error experiment: valid indices are 0, 1, and 2.
    std::cout << sensor_values[3] << '\n';
    return 0;
}
