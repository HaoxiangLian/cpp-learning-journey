# 第 7 天练习：枚举、命名空间、类型别名与 struct

## A. 笔试基础

### 1. `struct SensorState { ... };` 是否已经创建对象？为什么

> 
>

### 2. `using SensorId = int;` 是否创建了不同于 `int` 的新类型

> 
>

### 3. `enum class` 相比裸整数状态有什么好处

> 
>
>

## B. 编程题

在命名空间 `sensor` 中定义：

- `using SensorId = int;`
- `enum class Health { ok, warning, fault };`
- `struct SensorState`，包含编号、读数和健康状态。

创建一个对象并输出其成员。

```cpp

```

实际输出：

```text

```

## C. 调试题

构建 `examples/day-07/scoped_enum_error.cpp`，记录诊断。分别用“保留枚举类型”和“明确转换为整数”两种意图修复。

```text

```

> 
>
>

## D. 面试口述

用 30～60 秒回答：为什么状态值更适合使用 `enum class`？类型别名是否提供相同的类型安全？

> 
>
>
>

