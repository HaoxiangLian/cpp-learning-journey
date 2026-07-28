#include <iostream>

int remaining_cycles(int completed_cycles, int target_cycles);
bool can_continue(int battery_percent, bool emergency_stop);
void print_decision(bool allowed, int remaining);

int main() {
    int completed_cycles{3};
    int target_cycles{8};
    int battery_percent{42};
    bool emergency_stop{false};

    int remaining{
        remaining_cycles(completed_cycles, target_cycles)
    };
    bool allowed{
        can_continue(battery_percent, emergency_stop)
    };

    print_decision(allowed, remaining);
    return 0;
}

int remaining_cycles(int completed_cycles, int target_cycles) {
    if (completed_cycles >= target_cycles) {
        return 0;
    }

    return target_cycles - completed_cycles;
}

bool can_continue(int battery_percent, bool emergency_stop) {
    return battery_percent >= 20 && !emergency_stop;
}

void print_decision(bool allowed, int remaining) {
    std::cout << "Remaining cycles: " << remaining << '\n';

    if (allowed) {
        std::cout << "Decision: continue\n";
    } else {
        std::cout << "Decision: stop\n";
    }
}
