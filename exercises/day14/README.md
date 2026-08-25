# 第 14 天练习：模板、推导与实例化

[返回第 14 天讲义](../../days/day14.md) · [完成后查看答案](../../solutions/day14/README.md)

每道代码题先写三行：模板参数怎样得到模板实参、哪一行触发具体版本、该类型必须支持哪些操作。不要先在长报错中猜结论。

## 一、概念判断题

判断正误并用一句话说明理由。

1. 一份函数模板在程序运行时就是一个能够接收任意类型的普通函数。
2. 在 `template <typename T>` 中 `T` 是模板参数；在 `larger<int>(3, 7)` 中 `int` 是模板实参，`3` 和 `7` 是函数调用实参。
3. 对 `template <typename T> T larger(T, T)`，调用 `larger(3, 4.5)` 时编译器一定先把 `3` 转成 `double`，再完成推导。
4. 模板定义的固定语法可以先通过检查，但某个依赖 `T` 的表达式仍可能在特定实例化时产生编译错误。
5. `Reading<int>` 与 `Reading<double>` 来自同一类模板，所以是同一个类型，可以直接互相赋值。
6. “为 `bool` 提供不同实现”和“只允许数值类型”是两个目的，前者可考虑显式特化，后者应表达模板要求。

## 二、推导、实例化与类型分析

1. 阅读下面代码，写出两个调用中 `T` 的结果、返回值，并指出哪个步骤属于推导、哪个步骤会需要实例化：

   ```cpp
   template <typename T>
   T larger(T left, T right) {
       return left < right ? right : left;
   }

   auto first = larger(2, 9);
   auto second = larger<double>(2, 9.5);
   ```

2. 查看 [`broken/mixed_template_arguments.cpp`](broken/mixed_template_arguments.cpp)，说明为什么错误发生在推导阶段，并给出两种能表达明确类型意图的修改方向。
3. 查看 [`broken/unsupported_instantiation.cpp`](broken/unsupported_instantiation.cpp)，说明为什么 `T` 可以确定，却仍然编译失败；指出模板体中的具体失败表达式。
4. 对 `Reading<int> count{9};` 与 `Reading<double> temperature{23.5};`，逐一写出构造参数、成员 `value_` 和访问函数返回值的类型，并判断 `temperature = count;` 是否成立。
5. 查看 [`broken/late_specialization.cpp`](broken/late_specialization.cpp)，按文件顺序指出哪一行先需要了 `TypeCode<bool>`，为什么后面的显式特化位置不合法，以及应按什么顺序组织三部分代码。

## 三、小型编程题：通用读数与类型名称

打开 [`template_readings.cpp`](template_readings.cpp)，完成：

- 定义 `larger(T, T)` 函数模板，返回两个同类型值中较大的一个；
- 定义 `Reading<T>` 类模板，保存一个 `T` 数值和一个 `std::string` 单位，并提供只读访问函数；
- 在 `Reading<T>` 中用 `static_assert(std::is_arithmetic_v<T>, ...)` 表达 C++17 主线下的简单数值要求；
- 定义 `TypeName<T>` 主模板，使 `get()` 返回 `"other"`；
- 为 `TypeName<bool>` 写显式特化，使 `get()` 返回 `"boolean"`；
- 在 `main` 中只使用同类型实参调用 `larger`，并创建 `Reading<double>`。

预期稳定输出：

```text
max count: 9
temperature: 23.5 C
bool label: boolean
```

本练习使用 C++17 构建，不要把 C++20 `concept` 代码混入提交；请在纸上另写一遍对应的 `template <std::integral T>` 补充形式。
