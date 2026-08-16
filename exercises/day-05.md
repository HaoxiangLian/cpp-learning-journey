# 第 5 天练习：函数的声明、定义、调用与返回

## A. 笔试基础

### 1. 指出声明、定义和调用

```cpp
int twice(int value);
int result{twice(3)};
int twice(int value) { return value * 2; }
```

> 
>
>

### 2. 上例中哪些是形参，哪些是实参

> 
>

### 3. 为什么不能只改变返回类型来重载函数

> 
>

## B. 编程题

定义函数 `double larger(double left, double right)`，返回两个输入中较大的值。在 `main` 中调用并输出结果。

```cpp

```

实际输出：

```text

```

## C. 调试题

构建 `examples/day-05/missing_declaration.cpp`，记录关键诊断。用“提前声明”和“把定义移到调用前”两种方式分别修复。

```text

```

> 
>
>

## D. 面试口述

用 30～60 秒回答：函数声明和函数定义有什么区别？形参与实参又有什么区别？

> 
>
>
>

