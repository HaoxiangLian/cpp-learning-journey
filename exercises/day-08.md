# 第 8 天练习：string、array 与 vector

## A. 笔试基础

### 1. 为下列数据选择 `string`、`array` 或 `vector`

- 机器人名称：
- 固定六个关节零位：
- 数量持续增长的力传感采样：

> 
>
>

### 2. 长度为 4 的容器有哪些合法索引？`size()` 是多少

> 
>

### 3. `operator[]` 与 `.at()` 的越界行为有什么区别

> 
>
>

## B. 编程题

编写函数 `double average(const std::vector<double>& values)`：非空时返回平均值，空容器时返回 `0.0`。在 `main` 中分别测试空容器和四个样本。

```cpp

```

实际输出：

```text

```

## C. 调试题

构建并运行 `examples/day-08/at_out_of_range.cpp`，记录异常信息，写出合法索引范围并修复。

```text

```

> 
>

## D. 面试口述

用 30～60 秒回答：`std::array` 与 `std::vector` 怎样选择？随后解释 `[]` 与 `.at()` 的差别。

> 
>
>
>

