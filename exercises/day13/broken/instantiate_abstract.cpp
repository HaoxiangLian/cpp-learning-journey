// INTENTIONALLY INVALID: this file must fail during compilation.
class Sensor {
public:
    virtual ~Sensor() = default;
    virtual double read() const = 0;
};

int main() {
    Sensor sensor;  // 编译错误：Sensor 是抽象类
    return sensor.read() > 0.0 ? 0 : 1;
}
