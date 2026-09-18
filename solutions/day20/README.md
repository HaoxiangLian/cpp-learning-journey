# Day 20 练习答案与解析

[返回练习](../../exercises/day20/README.md) · [返回讲义](../../days/day20.md)

## A. 概念判断

1. **错误。** 按值写 `auto copy = source;` 时，推导结果不保留顶层 `const`，所以 `copy` 是可修改的 `int`。
2. **错误。** 对未加括号的名字，`decltype(name)` 得到声明类型；若 `name` 是具名变量，`(name)` 是左值表达式，`decltype((name))` 得到相应左值引用类型。
3. **错误。** 类型与值类别是两个问题；尤其一个具有名字的右值引用变量表达式本身是左值。
4. **错误。** `std::move` 只把表达式转换为可匹配右值入口的形式，`std::forward` 只按模板推导结果保留原来的值类别；真正的复制或移动由随后选中的操作完成。
5. **正确。** `T&&` 还必须处在 `T` 可由调用实参推导的语境；类中的普通 `Widget&&` 或已确定 `T` 的 `Box<T>&&` 不是转发引用。

## B. 输出预测

```text
5 6
7 7
true
```

`a` 是独立 `int` 对象；`b` 和 `c` 都引用 `value`。因此改 `a` 不改 `value`，通过 `b` 或 `c` 写入都会改变同一对象。

## C. 编程练习

参考实现：[`forward_message.cpp`](forward_message.cpp)。预期输出：

```text
read message
move message
read message
```

模板内部的形参 `message` 有名字，所以直接写 `consume(message)` 总按左值选择重载。`std::forward<T>(message)` 根据调用时推导出的 `T` 恢复原来的类别：普通具名对象与 `const` 具名对象走读取入口，临时对象走移动入口。

## 故意错误解析

`auto& reference = 42;` 要让可修改左值引用绑定到临时整数，程序非良构，必须由编译器拒绝。若只需只读观察，可写 `const auto& reference = 42;`；若要保存独立值，优先写 `auto value = 42;`。
