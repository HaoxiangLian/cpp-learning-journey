#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::vector<double> forces{9.0, 12.5, 5.0, 15.0};
    const double limit = 10.0;

    std::sort(forces.begin(), forces.end());

    const auto overload_count = std::count_if(
        forces.begin(), forces.end(),
        [limit](double value) { return value > limit; });

    std::cout << "sorted:";
    for (double value : forces) {
        std::cout << ' ' << value;
    }
    std::cout << "\noverload count = " << overload_count << "\n";
}
