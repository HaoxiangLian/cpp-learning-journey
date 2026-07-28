#include <iostream>

void try_boost(int percent);
void normalize_percent(int& percent);
int energy_cost(int cycles, int per_cycle = 3);
double energy_cost(double cycles, double per_cycle);
int recursive_sum(int value);

int main() {
    int battery{108};
    normalize_percent(battery);
    std::cout << "Battery after clamp: " << battery << '\n';

    try_boost(battery);
    std::cout << "Battery after value call: " << battery << '\n';

    std::cout << "Energy default: " << energy_cost(4) << '\n';
    std::cout << "Energy custom: " << energy_cost(4, 5) << '\n';
    std::cout << "Energy precise: " << energy_cost(2.5, 1.2) << '\n';
    std::cout << "Recursive sum: " << recursive_sum(4) << '\n';
    return 0;
}

void try_boost(int percent) {
    percent = percent + 10;
    std::cout << "Inside value parameter: " << percent << '\n';
}

void normalize_percent(int& percent) {
    if (percent < 0) {
        percent = 0;
    } else if (percent > 100) {
        percent = 100;
    }
}

int energy_cost(int cycles, int per_cycle) {
    return cycles * per_cycle;
}

double energy_cost(double cycles, double per_cycle) {
    return cycles * per_cycle;
}

int recursive_sum(int value) {
    if (value <= 0) {
        return 0;
    }

    return value + recursive_sum(value - 1);
}
