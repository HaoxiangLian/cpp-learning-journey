#include <iostream>
#include <stdexcept>

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

class Controller {
public:
    Controller()
        : first_{"first"}, second_{"second"} {
        std::cout << "Controller body throws\n";
        throw std::runtime_error{"configuration error"};
    }

    ~Controller() {
        std::cout << "~Controller body\n";
    }

private:
    Trace first_;
    Trace second_;
};

int main() {
    try {
        Controller controller;
    } catch (const std::exception& error) {
        std::cout << "caught: " << error.what() << '\n';
    }
}
