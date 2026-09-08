# Day 23 练习：综合辨析与条件化回答

[返回讲义](../../days/day23.md) · [独立答案](../../solutions/day23/README.md)

先写对象表和条件表，再运行代码；不要先打开答案目录。

## A. 概念判断

判断并补全成立条件：

1. `std::move(pointer)` 会立即移动 `pointer` 管理的对象。
2. `vector<unique_ptr<T>>` 重新分配后，先前 `get()` 返回的 `T*` 必然失效。
3. 通过 `Base&` 调用成员函数一定发生动态绑定。
4. `unordered_map` 的查找一定比 `map` 快。
5. 只要一个共享变量附近出现 mutex，就不会发生数据竞争。

## B. 代码与状态判断

不运行代码，分别写出 `owner`、`tasks[0]` 和 `Task` 对象的状态：

```cpp
auto owner = std::make_unique<Add>(2);
std::vector<std::unique_ptr<Task>> tasks;
tasks.push_back(std::move(owner));
Task* observed = tasks.front().get();
tasks.reserve(100);
const int result = observed->run();
tasks.clear();
```

回答：`reserve` 后 `observed` 为什么仍可用？`clear` 后为什么不能再用？若保存的是 `auto* element = &tasks.front()`，结论有什么不同？

## C. 编程练习

完成 [`task_queue.cpp`](task_queue.cpp)：

1. 定义抽象 `Task` 与至少一个派生任务，通过虚函数得到结果。
2. `TaskQueue` 用 `std::vector<std::unique_ptr<Task>>` 独占管理任务。
3. `push` 接收并移动一个 `unique_ptr<Task>`；`run_all` 返回所有结果。
4. 稳定输出 `results: 4 7`，不得手写 `delete`。
5. 用 `-std=c++17 -Wall -Wextra -Wpedantic` 构建，并用 ASan/UBSan 再运行一次。

## 故意错误实验

[`broken/copy_unique_tasks.cpp`](broken/copy_unique_tasks.cpp) 验证装有 `unique_ptr` 的容器不能复制。它带有故意错误标记，不属于正常构建目标。

