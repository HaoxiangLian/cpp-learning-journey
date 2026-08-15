#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Sensor {
public:
    virtual ~Sensor() = default;
    virtual std::string name() const = 0;
    virtual double read() const = 0;
};

class ForceSensor final : public Sensor {
public:
    explicit ForceSensor(double value) : value_{value} {}
    std::string name() const override { return "force"; }
    double read() const override { return value_; }
private:
    double value_{};
};

int main() {
    std::vector<std::unique_ptr<Sensor>> sensors;
    sensors.push_back(std::make_unique<ForceSensor>(3.2));
    for (const auto& sensor : sensors) {
        std::cout << sensor->name() << ": " << sensor->read() << '\n';
    }
}
