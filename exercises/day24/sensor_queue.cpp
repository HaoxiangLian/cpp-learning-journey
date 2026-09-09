#include <iostream>
#include <memory>
#include <string>
#include <vector>

struct Result {
    std::string name;
    int value;
};

class Task {
public:
    virtual ~Task() = default;
    virtual Result run() const = 0;
};

// TODO: 实现 AverageTask 与 ThresholdTask。

class SensorQueue {
public:
    // TODO: 实现 push、run_all 与 size。

private:
    std::vector<std::unique_ptr<Task>> tasks_;
};

int main() {
    SensorQueue queue;
    // TODO: 创建两个任务、加入队列、运行并输出结果与数量。
}
