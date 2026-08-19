#include <iostream>

class Sensor {
public:
    explicit Sensor(int id) : id_{id} {}
    virtual ~Sensor() = default;

    int id() const {
        return id_;
    }

private:
    int id_{};
};

class Lidar : public Sensor {
public:
    Lidar(int id, int beam_count)
        : Sensor{id}, beam_count_{beam_count} {}

    int beam_count() const {
        return beam_count_;
    }

private:
    int beam_count_{};
};

class Robot {
public:
    Robot(int sensor_id, int beam_count)
        : front_lidar_{sensor_id, beam_count} {}

    const Lidar& front_lidar() const {
        return front_lidar_;
    }

private:
    Lidar front_lidar_;
};

class Task {
public:
    virtual ~Task() {
        std::cout << "destroy task\n";
    }
};

class ScanTask : public Task {
public:
    ~ScanTask() override {
        std::cout << "destroy scan task\n";
    }
};

int main() {
    Robot robot{42, 32};
    const Lidar& lidar{robot.front_lidar()};
    std::cout << "robot lidar: " << lidar.id() << ' '
              << lidar.beam_count() << '\n';

    Task* task{new ScanTask};
    delete task;
}
