#include <iostream>

class BatteryLevel {
public:
    bool set(int candidate) {
        // TODO: 只接受 [0, 100]，非法输入不得改变 percentage_。
        (void)candidate;
        return false;
    }

    int value() const {
        // TODO: 返回当前电量。
        return percentage_;
    }

    bool is_low() const {
        // TODO: 电量小于 30 时返回 true。
        return false;
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
