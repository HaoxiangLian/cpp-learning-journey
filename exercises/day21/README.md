# Day 21 练习：线程、锁与原子操作

[返回讲义](../../days/day21.md) · [独立答案](../../solutions/day21/README.md)

先在纸上画出线程和共享对象，再编译验证。不要先打开答案目录。

## A. 概念判断

判断并说明理由：

1. `std::thread` 对象离开作用域时会自动等待关联线程完成。
2. 两个线程只读同一个不再变化的 `int` 会形成数据竞争。
3. `std::lock_guard` 的作用域结束时会自动释放它管理的互斥量。
4. 只要每个线程都使用了锁，就不可能死锁。
5. `volatile int` 可以代替 `std::atomic<int>` 完成线程安全计数。

## B. 执行顺序判断

不运行代码，回答 `join()` 前后哪些读取有可靠含义，并写出最终可保证的输出：

```cpp
int result{0};
std::thread worker{[&result] { result = 7; }};
worker.join();
std::cout << result << '\n';
```

再说明：如果把输出移到 `join()` 之前，为什么不能仅凭“工作线程通常很快”断言结果？

## C. 编程练习

完成 [`safe_counter.cpp`](safe_counter.cpp)：

1. 使用一个 `std::mutex` 保护 `count`。
2. 使用作用域锁对象，不能手写一条容易漏掉的 `unlock()` 路径。
3. 保证程序稳定输出 `count: 4000`。
4. 用 `-std=c++17 -Wall -Wextra -Wpedantic -pthread` 构建。

## 故意错误实验

[`broken/copy_mutex.cpp`](broken/copy_mutex.cpp) 验证互斥量不能复制。它带有故意错误标记，不属于正常构建目标。
