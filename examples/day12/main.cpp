#include <iostream>

class Sensor {
public:
    explicit Sensor(int id) : id_{id} {}

    Sensor(const Sensor&) = default;
    Sensor& operator=(const Sensor&) = default;
    Sensor(Sensor&&) = default;
    Sensor& operator=(Sensor&&) = default;
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

void print_by_value(Sensor sensor) {
    std::cout << "by value id: " << sensor.id() << '\n';
}

void print_by_reference(const Sensor& sensor) {
    std::cout << "by reference id: " << sensor.id() << '\n';
}

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
    Lidar lidar{7, 64};
    print_by_value(lidar);      // 新建独立 Sensor 参数，发生切片
    print_by_reference(lidar);  // 不复制完整对象
    std::cout << "lidar beams: " << lidar.beam_count() << '\n';

    Task* task{new ScanTask};
    delete task;
}
