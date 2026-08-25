#include <iostream>

class Sensor {
public:
    virtual ~Sensor() = default;
    virtual const char* name() const = 0;
    virtual double read() const = 0;
};

class TemperatureSensor : public Sensor {
public:
    explicit TemperatureSensor(double value) : value_{value} {}

    const char* name() const override {
        return "temperature";
    }

    double read() const override {
        return value_;
    }

private:
    double value_{};
};

class ForceSensor : public Sensor {
public:
    explicit ForceSensor(double value) : value_{value} {}

    const char* name() const override {
        return "force";
    }

    double read() const override {
        return value_;
    }

private:
    double value_{};
};

void print_sensor(const Sensor& sensor) {
    std::cout << sensor.name() << ": " << sensor.read() << '\n';
}

struct LayoutProbe {
    char valid;
    int value;
};

int main() {
    TemperatureSensor temperature{23.5};
    ForceSensor force{12.5};

    print_sensor(temperature);
    print_sensor(force);

    std::cout << "layout sizeof/alignof: " << sizeof(LayoutProbe)
              << '/' << alignof(LayoutProbe) << '\n';
}
