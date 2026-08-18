# 第 9 天练习：构造、析构与初始化顺序

[返回第 9 天讲义](../../days/day09.md) · [完成后查看答案](../../solutions/day09/README.md)

## 一、概念判断

1. 数据成员直到进入构造函数体才开始初始化。
2. 成员初始化列表的书写顺序决定非静态数据成员的真实初始化顺序。
3. 非静态数据成员通常按声明顺序初始化，并按逆序析构。
4. `explicit SensorId(int)` 仍允许写 `SensorId id{3};`。
5. 析构函数体结束后，成员子对象会按相应逆序继续析构。

## 二、代码分析

### A：初始化还是赋值

```cpp
class A {
public:
    A(int value) { value_ = value; }
private:
    int value_{10};
};
```

`value_` 经历了哪些步骤？怎样改成一次直接初始化？

### B：声明顺序

```cpp
class Reading {
public:
    explicit Reading(int raw) : raw_{raw}, doubled_{raw_ * 2} {}
private:
    int doubled_;
    int raw_;
};
```

哪个成员先初始化？读取 `raw_` 是否安全？仅交换列表项目能否修复？

### C：explicit

```cpp
class SensorId {
public:
    explicit SensorId(int value) : value_{value} {}
private:
    int value_;
};

SensorId first{3};
// SensorId second = 3;
```

哪一行合法？取消注释后属于哪类错误？

## 三、编程题：任务会话

补全 [`task_session.cpp`](task_session.cpp)：

- 使用 `explicit TaskSession(int)` 和成员初始化列表建立 `id_`、`active_`；
- 构造时输出 `session 42 opened`；
- `finish()` 只负责把活动状态改为 `false` 并输出完成信息；
- 查询函数为 `const`；
- 析构时输出 `session 42 closed`；
- 不允许类外直接修改状态。

严格编译：

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror task_session.cpp -o task_session
./task_session
```

## 四、故意错误观察

[`broken/implicit_conversion.cpp`](broken/implicit_conversion.cpp) 预期编译失败。说明为什么 `explicit` 拒绝 `schedule(3)`，再写出保留 `explicit` 的修复调用。
