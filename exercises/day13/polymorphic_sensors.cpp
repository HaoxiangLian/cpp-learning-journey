#include <iostream>

class Sensor {
public:
    virtual ~Sensor() = default;

    // TODO 1：声明 const char* name() const 和 double read() const 为纯虚函数。
};

// TODO 2：定义 TemperatureSensor，覆盖 name() 与 read()。

// TODO 3：定义 ForceSensor，覆盖 name() 与 read()。

// TODO 4：定义 print_sensor(const Sensor&)，经统一接口输出名称与读数。

struct LayoutProbe {
    char valid;
    int value;
};

int main() {
    std::cout << "完成 TODO 后应输出 temperature: 23.5 和 force: 12.5\n";
    std::cout << "layout sizeof/alignof: " << sizeof(LayoutProbe)
              << '/' << alignof(LayoutProbe) << '\n';
}
