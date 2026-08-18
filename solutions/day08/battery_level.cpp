#include <iostream>

class BatteryLevel {
public:
    bool set(int candidate) {
        if (candidate < 0 || candidate > 100) {
            return false;
        }
        percentage_ = candidate;
        return true;
    }

    int value() const {
        return percentage_;
    }

    bool is_low() const {
        return percentage_ < 30;
    }

private:
    int percentage_{0};
};

int main() {
    BatteryLevel battery;
    const bool accepted{battery.set(25)};
    const bool rejected{!battery.set(120)};

    std::cout << std::boolalpha;
    std::cout << "accepted: " << accepted << '\n';
    std::cout << "rejected invalid: " << rejected << '\n';
    std::cout << "battery: " << battery.value() << '\n';
    std::cout << "low: " << battery.is_low() << '\n';
}
