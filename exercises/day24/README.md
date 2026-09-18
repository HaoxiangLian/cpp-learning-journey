# Day 24 练习：综合验收与传感器任务队列

[返回讲义](../../days/day24.md) · [独立答案](../../solutions/day24/README.md)

先计时独立完成并保存第一次答案，再对照解析。不要先打开答案目录。

## A. 概念判断

判断正误；错误项必须补出成立条件或修复方向：

1. `std::move(owner)` 执行后，`owner` 对象的生命周期立即结束。
2. `vector<unique_ptr<Task>>` 重新分配后，由元素 `get()` 得到的 `Task*` 一定失效。
3. 通过 `Task&` 调用 `run()` 一定执行派生类版本。
4. ASan 和 UBSan 都没有报告，足以证明程序没有未定义行为。
5. 复习时只要重新阅读全部讲义，就等价于主动提取。

## B. 限时代码辨析

在 10 分钟内，不运行代码，写出每一步的对象、责任和最后合法使用点：

```cpp
SensorQueue queue;
auto task = std::make_unique<AverageTask>("room", 20, 24);
Task* observed = task.get();
queue.push(std::move(task));
const auto first = queue.run_all();
// queue 在这里离开作用域
```

回答：

1. `push` 后 `task`、`observed` 和任务对象分别是什么状态？
2. 队列仍存活时，`observed` 能否调用 `run()`？
3. 队列销毁后，`observed` 和 `first` 分别能否使用？
4. 若 `run()` 可能抛异常，当前代码至少能否保证资源不泄漏？哪些业务状态仍要另行约定？

## C. 编程练习

完成 [`sensor_queue.cpp`](sensor_queue.cpp)：

1. 定义 `Result`、抽象 `Task`、`AverageTask` 和 `ThresholdTask`。
2. `SensorQueue` 使用 `vector<unique_ptr<Task>>` 独占管理任务。
3. `push` 明确转移责任；`run_all` 返回独立的 `vector<Result>`。
4. 输出必须是 `room average: 22`、`motor alert: 1`、`processed: 2`。
5. 先以 GCC 和 Clang 的 C++17 模式构建，再用 ASan/UBSan 运行。

## 故意错误实验

[`broken/copy_sensor_queue.cpp`](broken/copy_sensor_queue.cpp) 故意复制装有 `unique_ptr` 的队列。它应在编译阶段失败，不属于正常构建目标。
