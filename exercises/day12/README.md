# 第 12 天练习：继承、切片与虚析构

[返回第 12 天讲义](../../days/day12.md) · [完成后查看答案](../../solutions/day12/README.md)

每题先写出两个问题：“完整对象是什么类型？”“这里创建了新对象，还是只建立引用/指针关系？”

## 一、概念判断题

判断正误并用一句话说明理由。

1. `class Lidar : public Sensor` 表示每个 `Lidar` 对象中包含一个 `Sensor` 基类子对象。
2. 派生类能够直接访问基类的所有 `private` 数据成员。
3. `Sensor copied{lidar};` 是有明确定义的代码，但只建立一个独立 `Sensor` 对象。
4. 把按值参数改成 `const Sensor&` 可以避免切片，也必然让所有同名成员函数动态选择派生版本。
5. 如果可能通过 `Base*` 删除实际创建的派生对象，基类析构函数应支持这种删除契约。

## 二、代码输出与错误分析

1. 对 `Lidar lidar{7, 64}; Sensor sensor{lidar};`，画出 `lidar` 与 `sensor` 各自包含的部分；`sensor` 中是否还有 `beam_count_`？
2. `void inspect(const Sensor&); inspect(lidar);` 是否新建一个独立 `Sensor` 参数对象？为什么不会发生切片？
3. 若 `Task` 析构函数不是虚函数，却执行 `Task* p{new ScanTask}; delete p;`，错误应怎样分类？能否断言只是不调用一条日志？
4. 查看 [`broken/private_inheritance_conversion.cpp`](broken/private_inheritance_conversion.cpp)，解释为什么类外不能把该 `Lidar` 隐式转换为 `Sensor` 引用，以及哪种继承写法符合公开的 “Lidar is-a Sensor” 接口。

## 三、小型编程题：传感器关系与任务析构

打开 [`sensor_hierarchy.cpp`](sensor_hierarchy.cpp)，完成：

- `Lidar` 以公有继承表达它可作为 `Sensor` 使用，并增加束数。
- `Robot` 以成员对象组合一个 `Lidar`，表达“拥有”。
- `Task` 提供适合基类指针删除的析构接口，`ScanTask` 明确覆盖它。
- 输出机器人雷达信息，然后通过 `Task*` 删除 `ScanTask`。

预期输出：

```text
robot lidar: 42 32
destroy scan task
destroy task
```
