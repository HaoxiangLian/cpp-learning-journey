# 第 3 天练习：表达式、类型转换、const 与 constexpr

## A. 笔试基础

### 1. 预测结果

```cpp
double a{};
a = 7 / 2;
double b{7.0 / 2};
```

`a`：

> 

`b`：

> 

原因：

> 
>

### 2. `const` 和 `constexpr` 各自限制什么

> 
>
>

### 3. `static_cast<double>(count) / total` 中，转换为何必须发生在除法之前

> 
>

## B. 编程题

已知编码器每转 `8192` 个计数，当前读数为 `2048`。编写程序输出转数和角度，要求结果分别为 `0.25` 和 `90`。

```cpp

```

实际输出：

```text

```

## C. 调试题

构建 `examples/day-03/const_assignment_error.cpp`，记录关键诊断，说明为什么初始化合法而后续赋值非法。

```text

```

> 
>

## D. 面试口述

用 30～60 秒回答：`const` 与 `constexpr` 有什么区别？随后解释为什么把整数除法结果保存到 `double` 仍可能得到 0。

> 
>
>
>

