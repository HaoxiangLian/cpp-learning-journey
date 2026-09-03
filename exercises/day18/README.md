# Day 18 练习：智能指针与所有权选择

先完成本目录中的 `smart_pointer_choices.cpp`，再阅读 `solutions/day18/`。练习代码只给出问题和检查，不包含答案。

## A. 概念判断

判断并说明理由：

1. `unique_ptr` 被移动后，源指针对象已经不存在。
2. 两个 `shared_ptr` 互相保存对方，就一定会在离开作用域时销毁对象。
3. `weak_ptr::lock()` 失败时会返回空 `shared_ptr`。
4. `shared_ptr::use_count()` 适合作为多线程程序决定“现在只有我一个拥有者”的同步条件。

## B. 输出预测

不运行程序，先写出 `smart_pointer_choices.cpp` 中三行输出，再编译核对。特别说明每次 `reset()` 后哪一个对象仍然存在。

## C. 编程练习

完成两个函数：

- `transfer_job`：接收并返回 `unique_ptr<Job>`，把任务编号加一；
- `observe_job`：接收 `weak_ptr<Job>`，若对象仍存在就返回编号，否则返回 `-1`。

额外挑战：设计 `Folder` 的父子关系，子节点用 `weak_ptr` 指回父节点，并说明若改用 `shared_ptr` 会发生什么。

`broken/copy_unique_ptr.cpp` 是故意无法通过编译的反例，用来确认“独占所有权不能复制”由编译器阻止。
