# 第 8 天练习：类、封装与不变量

[返回第 8 天讲义](../../days/day08.md) · [完成后查看答案](../../solutions/day08/README.md)

先写答案和理由，再运行可运行代码。故意错误文件只用于观察编译诊断，不要加入正常目标。

## 一、概念判断

判断正误并用一句话说明理由：

1. 类和对象是同一个概念，只是两种叫法。
2. 同一个类的两个对象各自拥有自己的非静态数据成员。
3. 只要把数据成员设为 `private`，类不变量就一定不会被破坏。
4. `class` 的成员默认是 `private`，`struct` 的成员默认是 `public`。
5. `const` 成员函数绝对不能产生任何副作用。

## 二、代码分析

### A：对象状态是否共享

```cpp
class Counter {
public:
    void increase() { ++value_; }
    int value() const { return value_; }
private:
    int value_{0};
};

Counter first;
Counter second;
first.increase();
```

`first.value()` 和 `second.value()` 分别是多少？为什么？

### B：访问控制

```cpp
class Battery {
private:
    int percentage_{100};
};

Battery battery;
// battery.percentage_ = 20;
```

如果取消注释，错误发生在编译、链接还是运行阶段？`private` 是否意味着调试器无法看到该成员？

### C：const 调用

```cpp
class Sensor {
public:
    int id() { return id_; }
private:
    int id_{7};
};

const Sensor sensor;
// sensor.id();
```

为什么取消注释后不合法？在不改变查询语义的前提下应该怎样修复？

## 三、编程题：受约束的电量对象

补全 [`battery_level.cpp`](battery_level.cpp) 中的 `BatteryLevel`：

- 类不变量：`percentage_` 始终位于 `[0, 100]`；
- `set(int)` 接受合法值并返回 `true`，拒绝非法值且保持原状态；
- `value()` 与 `is_low()` 必须能由 `const BatteryLevel` 调用；
- 数据成员保持 `private`，不要增加无条件写入接口；
- 程序最终应输出 `accepted: true`、`rejected invalid: true`、`battery: 25`、`low: true`。

严格编译建议：

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror battery_level.cpp -o battery_level
./battery_level
```

## 四、故意错误观察

[`broken/private_access.cpp`](broken/private_access.cpp) 直接修改私有成员，预期在编译阶段失败。先解释错误类别，再查看编译器诊断；不要把它加入正常构建。
