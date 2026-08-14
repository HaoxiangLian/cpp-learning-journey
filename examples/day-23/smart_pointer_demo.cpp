#include <iostream>
#include <memory>
#include <string>
#include <utility>

struct Motor {
    explicit Motor(std::string motor_name) : name{std::move(motor_name)} {
        std::cout << "construct " << name << '\n';
    }

    ~Motor() {
        std::cout << "destroy " << name << '\n';
    }

    std::string name;
};

void observe(const Motor* motor) {
    if (motor != nullptr) {
        std::cout << "observe " << motor->name << '\n';
    }
}

int main() {
    auto owner = std::make_unique<Motor>("joint-A");
    observe(owner.get());

    auto next_owner = std::move(owner);
    std::cout << std::boolalpha
              << "old owner is empty: " << (owner == nullptr) << '\n';
    next_owner.reset();

    std::weak_ptr<Motor> watcher;
    {
        auto first = std::make_shared<Motor>("joint-B");
        std::shared_ptr<Motor> second = first;
        watcher = first;

        std::cout << "strong owners: " << first.use_count() << '\n';
        if (auto locked = watcher.lock()) {
            std::cout << "locked " << locked->name << '\n';
        }
    }

    std::cout << "expired: " << watcher.expired() << '\n';
}
