#include <iostream>

class TraceMember {
public:
    explicit TraceMember(const char* name) {
        std::cout << "construct " << name << '\n';
    }
};

class Robot {
public:
    Robot()
        : motor_{"motor"}, controller_{"controller"} {}

private:
    TraceMember controller_;
    TraceMember motor_;
};

int main() {
    Robot robot;
}
