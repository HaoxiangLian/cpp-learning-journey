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

// TODO 1：定义 Lidar，使用 public 继承 Sensor，并增加 beam_count_。

// TODO 2：定义 Robot，把 Lidar 作为成员，表达“机器人拥有一个雷达”。

class Task {
public:
    // TODO 3：声明适合通过 Task* 删除派生对象的析构函数，并输出 destroy task。
};

// TODO 4：定义 ScanTask，析构时输出 destroy scan task。

int main() {
    std::cout << "完成 TODO 后应输出 robot lidar: 42 32\n";
    // TODO 5：创建 Robot；再通过 Task* 删除一个 new ScanTask。
}
