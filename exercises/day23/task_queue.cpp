#include <iostream>
#include <memory>
#include <vector>

class Task {
public:
    virtual ~Task() = default;
    virtual int run() const = 0;
};

// TODO: 定义一个派生任务。

class TaskQueue {
public:
    // TODO: 实现 push 与 run_all。

private:
    std::vector<std::unique_ptr<Task>> tasks_;
};

int main() {
    // TODO: 加入两个任务并输出 results: 4 7。
}

