#include <iostream>
#include <string>

class Sensor {
public:
    virtual ~Sensor() = default;

    std::string family() const {
        return "sensor";
    }

    virtual std::string kind() const {
        return "generic";
    }

    virtual int sample_rate() const = 0;
};

class Lidar : public Sensor {
public:
    explicit Lidar(int sample_rate) : sample_rate_{sample_rate} {}

    std::string family() const {
        return "lidar family";
    }

    std::string kind() const override {
        return "lidar";
    }

    int sample_rate() const override {
        return sample_rate_;
    }

private:
    int sample_rate_{};
};

struct ReadingRecord {
    char valid;
    int value;
};

int main() {
    Lidar lidar{64};
    const Sensor& sensor{lidar};

    std::cout << "nonvirtual family: " << sensor.family() << '\n';
    std::cout << "virtual kind: " << sensor.kind() << '\n';
    std::cout << "sample rate: " << sensor.sample_rate() << '\n';
    std::cout << "record sizeof/alignof: " << sizeof(ReadingRecord)
              << '/' << alignof(ReadingRecord) << '\n';
    std::cout << "sensor sizeof/alignof: " << sizeof(Sensor)
              << '/' << alignof(Sensor) << '\n';
}
