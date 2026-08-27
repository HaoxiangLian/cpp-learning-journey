# 第 14 天答案：模板、推导与实例化

[返回练习](../../exercises/day14/README.md) · [返回讲义](../../days/day14.md)

## 一、概念判断题

1. **错误。** 函数模板在编译期描述一族函数；具体调用经模板实参推导和实例化形成所需函数，不是运行时接收任意类型的单个普通函数。
2. **正确。** `T` 占据模板声明中的类型位置，`int` 填入该模板位置，`3` 与 `7` 则向具体函数的形参传值。
3. **错误。** 两个形参位置都参与对同一 `T` 的推导，分别得到 `int` 与 `double`，结论冲突。若显式指定 `T = double`，普通参数转换才可在随后调用匹配中发生。
4. **正确。** 固定语法先接受检查；像 `left < right` 这样依赖 `T` 的表达式，要到具体实参已知并需要实例化时才能判断。
5. **错误。** `Reading<int>` 与 `Reading<double>` 是不同的完整类类型；没有相应转换或赋值入口时不能直接赋值。
6. **正确。** 显式特化处理一组完整模板实参的例外实现；可接受范围属于要求或约束问题，不应靠枚举禁止类型表达。

## 二、推导、实例化与类型分析

1. `first` 调用从两个 `int` 实参推导 `T = int`，实例化所需具体函数后比较得到 `9`。`second` 已显式给出 `T = double`，整数 `2` 随后转换为 `double`，结果为 `9.5`；它不需要再推导 `T`，但仍会需要使用 `double` 的具体函数。
2. 两个形参都写成 `T`，调用却分别给出 `int` 与 `double`，所以无法得到唯一模板实参。可把两个值先统一成 `double` 再自动推导，也可在确有该意图时写 `larger<double>(3, 4.5)`。
3. 两个调用实参都是 `Label`，所以 `T = Label` 没有冲突；实例化函数体时，`left < right` 找不到适用于 `Label` 的比较操作，因而编译失败。
4. `Reading<int>` 的构造参数、`value_` 与返回值都是 `int`；`Reading<double>` 的相应位置都是 `double`。二者是不同类型，`temperature = count` 没有可用赋值入口，不能成立。
5. `constexpr int before_specialization = TypeCode<bool>::value;` 先需要 `bool` 的具体类，随后出现的显式特化已经太晚。顺序应为：先声明/定义主模板，再声明/定义 `bool` 特化，最后出现会使用 `TypeCode<bool>` 的代码。

## 三、编程题参考实现

完整实现见 [`template_readings.cpp`](template_readings.cpp)。函数模板只接受两个同类型值；`Reading<T>` 把完整类型中的实参映射到构造参数、成员与返回值；`static_assert` 在 C++17 下检查简单数值要求；`TypeName<bool>` 在首次使用前完成显式特化。

参考输出：

```text
max count: 9
temperature: 23.5 C
bool label: boolean
```

若把 `Reading<double>` 改成 `Reading<std::string>`，应先看到 `static_assert` 的要求失败；这不是运行时分支。C++20 可进一步用 `concept` 把要求放到模板声明上，但不属于本日 C++17 构建目标。
