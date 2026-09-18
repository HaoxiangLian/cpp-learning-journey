#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

class Session {
public:
    explicit Session(std::string name) : name_{std::move(name)} {
        std::cout << "open " << name_ << '\n';
    }

    ~Session() {
        std::cout << "close " << name_ << '\n';
    }

private:
    std::string name_;
};

void calibrate() {
    Session sensor{"sensor"};
    std::cout << "calibrating\n";
    throw std::runtime_error{"signal lost"};
}

void set_limit(int& limit, int candidate) {
    if (candidate < 0) {
        throw std::invalid_argument{"negative limit"};
    }
    limit = candidate;
}

int main() {
    try {
        calibrate();
    } catch (const std::exception& error) {
        std::cout << "caught: " << error.what() << '\n';
    }

    int limit{3};
    try {
        set_limit(limit, -1);
    } catch (const std::exception&) {
        std::cout << "limit after failure: " << limit << '\n';
    }
}
