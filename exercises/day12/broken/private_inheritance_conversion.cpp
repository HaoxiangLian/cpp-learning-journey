// INTENTIONALLY INVALID: this file must fail during compilation.
class Sensor {
public:
    int id() const {
        return 7;
    }
};

class Lidar : private Sensor {};

void print_sensor(const Sensor& sensor) {
    (void)sensor;
}

int main() {
    Lidar lidar;
    print_sensor(lidar);  // 编译错误：私有基类转换在这里不可访问
}
