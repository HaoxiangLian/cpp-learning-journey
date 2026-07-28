#include <iostream>

int main() {
    double battery_level{};
    int mode{};
    int cycle_count{};

    std::cout << "Battery level (0-100): ";
    std::cin >> battery_level;

    std::cout << "Mode (1: manual, 2: automatic): ";
    std::cin >> mode;

    std::cout << "Run cycles: ";
    std::cin >> cycle_count;

    if (battery_level < 0.0 || battery_level > 100.0 || cycle_count < 0) {
        std::cout << "Invalid input\n";
        return 1;
    }

    if (battery_level < 20.0) {
        std::cout << "Battery too low. Robot stopped.\n";
        return 0;
    } else if (battery_level < 50.0) {
        std::cout << "Battery status: low\n";
    } else {
        std::cout << "Battery status: normal\n";
    }

    switch (mode) {
        case 1:
            std::cout << "Control mode: manual\n";
            break;
        case 2:
            std::cout << "Control mode: automatic\n";
            break;
        default:
            std::cout << "Unknown mode\n";
            return 1;
    }

    for (int cycle{1}; cycle <= cycle_count; ++cycle) {
        std::cout << "Running cycle " << cycle << '\n';
    }

    std::cout << "Task finished\n";
    return 0;
}
