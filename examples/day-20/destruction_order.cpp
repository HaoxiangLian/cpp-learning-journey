#include <iostream>

class Trace {
public:
    explicit Trace(const char* name)
        : name_{name} {
        std::cout << "construct " << name_ << '\n';
    }

    ~Trace() {
        std::cout << "destroy " << name_ << '\n';
    }

private:
    const char* name_;
};

class Robot {
public:
    Robot()
        : controller_{"controller"}, motor_{"motor"} {
        std::cout << "Robot body\n";
    }

    ~Robot() {
        std::cout << "~Robot body\n";
    }

private:
    Trace controller_;
    Trace motor_;
};

int main() {
    Robot robot;
}
