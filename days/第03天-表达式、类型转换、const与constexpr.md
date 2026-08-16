# 第 3 天：表达式、类型转换、const 与 constexpr

预计用时：60～90 分钟  
今日目标：正确计算编码器角度，理解整数除法、类型转换以及 `const` 和 `constexpr` 的区别。

## 1. 先观察一个常见计算错误

[unit_conversion.cpp](../examples/day-03/unit_conversion.cpp) 把编码器计数换算为转数和角度：

```cpp
constexpr int counts_per_revolution{4096};
const int counts{1024};

double wrong_turns{};
wrong_turns = counts / counts_per_revolution;

const double turns{
    static_cast<double>(counts) / counts_per_revolution};
```

构建运行后，关键输出为：

```text
wrong turns: 0
turns: 0.25
angle: 90
```

错误不是发生在保存到 `double` 时，而是除法已经先按两个 `int` 操作数计算完了。

## 2. 关键术语索引

| 术语 | 本课用它解决什么问题 |
|---|---|
| [表达式](#表达式) | 一段代码怎样计算出值 |
| [整数除法](#整数除法) | 为什么 `1024 / 4096` 得到 0 |
| [类型转换](#类型转换) | 怎样改变参与运算的类型 |
| [const](#const) | 怎样表达对象初始化后不再修改 |
| [constexpr](#constexpr) | 怎样要求值能在编译期确定 |

### 表达式

**表达式是一段经过求值会产生结果、并且可能产生副作用的代码。**

在 `target - current` 中，两个变量是操作数，`-` 是运算符，求值结果是一个数值。赋值表达式还会修改左侧对象，这种可观察修改属于副作用。

运算符优先级影响代码怎样分组，但不能依靠直觉猜测复杂表达式。求职笔试中，先加括号写出分组，再逐步计算。

### 整数除法

**两个整数类型的操作数执行 `/` 时进行整数除法，结果向零截断。**

`1024 / 4096` 先得到整数 `0`，之后把 `0` 转成 `double` 仍然是 `0.0`。接收结果的变量类型不会倒过来改变前面已经完成的运算。

### 类型转换

**类型转换是把一个类型的值转换成另一个类型的值。**

编译器按语言规则自动进行的是隐式转换；代码明确写出转换要求的是显式转换。本课使用：

```cpp
static_cast<double>(counts)
```

它先产生对应的 `double` 值，随后 `double / int` 会把另一个操作数也转换为 `double`，最终执行浮点除法。

显式转换不是“保证安全”。如果把超出范围或带小数的值转成整数，仍可能丢失信息。它只让转换意图更清楚。

### const

**`const` 限定的对象在初始化后不能再通过该对象被修改。**

`const int counts{1024};` 表示这次测量值在后续计算中保持不变。[const_assignment_error.cpp](../examples/day-03/const_assignment_error.cpp) 再次给它赋值会编译失败。

`const` 不等于“编译期常量”。它可以使用运行时得到的值初始化，例如读取输入后保存为不可修改的测量快照。

### constexpr

**`constexpr` 变量必须由常量表达式初始化，因此其值能够在编译期确定，并且该变量也是 `const` 的。**

每转固定有 `4096` 个计数适合写成 `constexpr`。来自传感器的实时计数在编译时未知，只能是普通对象或 `const` 运行时快照。

## 3. 按求值顺序理解正确写法

```cpp
static_cast<double>(counts) / counts_per_revolution
```

1. 读取 `counts` 的整数值 `1024`；
2. 显式转换为 `double` 值 `1024.0`；
3. 另一个操作数转换为 `4096.0`；
4. 执行浮点除法，得到 `0.25`；
5. 用结果初始化 `turns`。

不要把整行笼统描述为“转成小数”。必须指出转换发生在除法之前还是之后。

## 4. 笔试高频点

### 预测 1

```cpp
double result{};
result = 5 / 2;
```

结果是 `2.0`，不是 `2.5`。`5 / 2` 先完成整数除法。

### 预测 2

```cpp
double result{5.0 / 2};
```

结果是 `2.5`，因为至少一个操作数是 `double`。

### 判断

“所有 `const` 对象的值都能在编译期得到。”错误。`constexpr` 对初始化表达式提出编译期要求，普通 `const` 没有这个普遍保证。

## 5. 面试回答

### 问题：`const` 和 `constexpr` 有什么区别？

> `const` 主要限制对象初始化后不能再通过它修改，但初始值可以来自运行期。`constexpr` 变量必须由常量表达式初始化，要求值可以在编译期确定，而且它本身也是 `const`。因此实时输入可以初始化 `const`，通常不能初始化 `constexpr`。

常见追问：

1. 为什么把整数除法结果保存到 `double` 仍可能是 0？
2. `static_cast` 是否能保证转换不丢失信息？

## 6. 今日任务与完成标准

完成 [第 3 天练习](../exercises/day-03.md)，并确认：

- [ ] 能逐步解释 `1024 / 4096` 为什么得到 0；
- [ ] 能在正确位置使用 `static_cast<double>`；
- [ ] 能区分隐式转换和显式转换；
- [ ] 能区分 `const` 与 `constexpr`；
- [ ] 能完成两道输出预测并口述面试答案。
