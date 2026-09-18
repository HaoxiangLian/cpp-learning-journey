# Day 20 练习：类型推导、值类别与转发

[返回讲义](../../days/day20.md) · [独立答案](../../solutions/day20/README.md)

先独立写出推导过程和预期输出，再编译验证。不要先打开答案目录。

## A. 概念判断

判断并说明理由：

1. `const int source{3}; auto copy = source;` 中 `copy` 仍是 `const int`。
2. `decltype(name)` 与 `decltype((name))` 对具名变量总会得到相同类型。
3. 一个表达式的类型是 `Packet&&`，它在任何地方就一定是右值。
4. `std::move` 自己负责搬运资源，`std::forward` 自己负责复制资源。
5. 模板形参 `T&&` 只有在 `T` 由调用实参推导时才可能是转发引用。

## B. 输出预测

不运行代码，写出三行输出，并分别说明 `a`、`b`、`c` 是否引用 `value`：

```cpp
int value{4};
auto a = value;
auto& b = value;
decltype((value)) c = value;
a = 5;
b = 6;
std::cout << a << ' ' << value << '\n';
c = 7;
std::cout << b << ' ' << value << '\n';
std::cout << std::boolalpha << (&b == &c) << '\n';
```

## C. 编程练习

完成 [`forward_message.cpp`](forward_message.cpp)：

1. 不改两个 `consume` 重载。
2. 补全 `relay`，让具名 `Message` 选择 `const Message&`，临时 `Message{}` 选择 `Message&&`。
3. 再加入一个 `const Message` 调用并解释选择结果。
4. 用 `-std=c++17 -Wall -Wextra -Wpedantic` 构建。

## 故意错误实验

[`broken/bind_auto_reference.cpp`](broken/bind_auto_reference.cpp) 用于验证“非常量左值引用不能绑定临时值”的编译期诊断。它带有故意错误标记，不属于正常构建目标。
