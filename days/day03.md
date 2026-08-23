# 第 3 天：表达式、转换与控制流

预计用时：120 分钟。标准主线：C++17。

## 🎯 今日攻坚目标

完成本日后，你应能按正文顺序：

1. 按运算符的分组与求值步骤预测表达式结果；
2. 判断一次类型转换会不会丢失数值信息；
3. 利用 `&&` 和 `||` 的短路规则保护除法、指针访问等前置条件；
4. 写出执行路径清楚、能够正常结束的分支和循环。

## 🔁 前置知识检查

先独立回答，再回看第 2 天：

1. `int count = 0;` 是初始化还是赋值？判断依据是什么？
2. `int temperature{21.75};` 为什么不能通过 C++17 编译？
3. `const` 是否等于“值一定在编译期已知”？

参考方向：对象正在创建，所以第一行是初始化；列表初始化拒绝窄化；`const` 约束普通修改，不自动保证编译期求值。

## 📖 核心知识重构

今天不把运算符、转换和控制语句拆成孤立语法表。我们沿着一条传感器处理路径回答三个连续问题：一行代码怎样算出结果，结果怎样进入另一种类型，以及程序怎样根据结果选择下一步。

### 机制一：先确定表达式怎样分组，再确定每一步使用什么类型

**实际问题**：同样是几个数字，`2 + 3 * 4` 得到 `14`，而 `(2 + 3) * 4` 得到 `20`；`5 / 2` 又不是 `2.5`。如果只凭“从左往右算”的直觉，代码输出很容易误判。

先把三行代码拆成可以直接核对的步骤：

| 代码 | 先做什么 | 最终结果 |
|---|---|---:|
| `2 + 3 * 4` | 先算 `3 * 4` | `14` |
| `(2 + 3) * 4` | 括号要求先算 `2 + 3` | `20` |
| `5 / 2` | 两边都是 `int`，执行整数除法 | `2` |

普通话结论：先看代码怎样分组，再看参与这一步运算的类型；结果接下来存进 `double`，也不会回头重算已经完成的整数除法。

**概念落点**：

- [表达式（expression）](../docs/glossary/day03.md#表达式expression)是规定一次求值的语法构造；求值可以计算一个值、调用函数或修改已有对象。
- [运算符（operator）](../docs/glossary/day03.md#运算符operator)是表达式中规定对一个或多个操作数执行何种运算的语法记号或关键字。

在 `sample_count + 1` 中，`+` 是运算符，左右两侧是操作数，整个片段是表达式。变量名、字面量或函数调用也可以是表达式，不要求一定出现算术符号。

**代码与机制**：预测结果时按以下顺序检查：

1. 用括号或运算符优先级确定语法怎样分组；
2. 确认每个操作数的类型；
3. 应用对应类型的运算规则；
4. 最后再看结果进入什么上下文。

```cpp
int first{2 + 3 * 4};      // 等价分组：2 + (3 * 4)，结果 14
int second{(2 + 3) * 4};   // 括号改变分组，结果 20
int quotient{5 / 2};       // 两个操作数都是 int，结果 2
double copied{5 / 2};      // 先做 int 除法得到 2，再初始化 double，结果 2.0
double precise{5.0 / 2};   // 至少一个操作数是 double，结果 2.5
```

运算符优先级决定的是**语法分组**，不能被扩大解释成所有子表达式的求值先后顺序。今天只依赖语言明确规定的顺序，例如机制三中的 `&&`、`||` 短路规则；其他复杂顺序问题留到后续表达式语义中再处理。

**错误做法与修复**：

```cpp
int total{5};
int count{2};
double average{total / count};  // 有定义，但得到 2.0，不是 2.5
```

分类：**有明确定义但结果不符合预期**。赋给 `double` 不会倒推前面的除法重新计算。需要小数结果时，先让除法进入浮点运算：

```cpp
double average{static_cast<double>(total) / count};  // 2.5
```

**小检查**：`double result{7 / 2 + 0.5};` 得到多少？应先在哪一步确定整数除法已经发生？

### 机制二：转换改变值的类型表示，但不会替你判断业务是否允许损失

**实际问题**：温度传感器给出 `31.75`，显示屏可能只展示整数部分。转换本身可以是需求，也可能是误把小数塞进整数。代码必须区分“上下文悄悄转换”和“开发者明确表达转换”。

```cpp
double temperature{31.75};

int implicit_whole = temperature;                 // 隐式转换，得到 31
int explicit_whole{static_cast<int>(temperature)}; // 显式表达意图，也得到 31
```

两行都得到 `31`。区别不是结果是否一定安全，而是第二行把“我要从 `double` 变成 `int`”明确写在了代码中。是否允许丢掉 `.75`，仍要由业务规则回答。

**概念落点**：[类型转换（type conversion）](../docs/glossary/day03.md#类型转换type-conversion)是按照 C++ 语言规则把一个类型的值转换成另一类型的值的过程。转换由初始化、赋值、运算或函数调用等上下文自动触发时称为隐式转换；代码明确使用 `static_cast<T>(...)` 请求时属于显式转换。

**代码与机制**：对这个可由 `int` 表示的正数，浮点到整数的转换会丢弃小数部分，也就是向零截断。它不是四舍五入。显式写出 `static_cast<int>` 使转换位置更醒目，但没有证明丢失小数符合需求。

还要守住两个边界：

- 如果截断后的浮点值超出目标整数类型可表示范围，C++ 不提供可依赖的结果；程序会触发第 1 天定义的[未定义行为](../docs/glossary/day01.md#未定义行为undefined-behavior)，不能声称一定崩溃。
- 第 2 天的列表初始化仍会帮助拒绝明显窄化：`int whole{temperature};` 是编译错误，即使当前运行值看起来不大。

当“整数显示”确实是需求时，可以把范围检查和转换意图放在一起：

```cpp
double temperature{31.75};

if (temperature >= -100.0 && temperature <= 100.0) {
    int whole{static_cast<int>(temperature)};
    std::cout << whole << '\n';
}
```

这里的范围是示例业务约束，并不是 `int` 的标准范围。

**错误做法与修复**：为了消除窄化编译错误，机械地加上 `static_cast<int>`，却没有确认范围和精度需求。这会把编译器揭示的设计冲突变成**有明确定义但结果可能不符合预期**，极端越界时还可能是未定义行为。修复顺序是：先确定目标类型和允许的损失，再检查范围，最后显式转换。

**小检查**：为什么 `static_cast<int>(-3.8)` 对这个值产生 `-3` 而不是 `-4`？“显式”能否推出“安全”？

### 机制三：条件先转换为真假，`&&` 与 `||` 再按短路规则决定是否继续

**实际问题**：只有采样数不为零时才能计算平均值；只有传感器已经就绪时才应检查读数。条件不只是“算出一个布尔值”，还可以保护右侧操作的前置条件。

```cpp
int sample_count{0};
int total{10};
bool usable{sample_count != 0 && total / sample_count >= 2};
```

按顺序读：先检查 `sample_count != 0`，结果为假；对 `&&` 来说，左边已经为假，整句不可能为真，所以右边的除法不会执行。`&&` 可先读成“并且”，`||` 可先读成“或者”。

**概念落点**：[短路求值（short-circuit evaluation）](../docs/glossary/day03.md#短路求值short-circuit-evaluation)是 `&&` 和 `||` 从左操作数开始求值，并在左操作数已经决定整个结果时不求值右操作数的规则。

`&&` 先判断左侧。若 `sample_count != 0` 为 `false`，整个合取条件不可能为真，右侧除法不会执行；因此这一写法用左侧保护了整数除法的前置条件。

对 `||` 则相反：

```cpp
bool may_continue{manual_override || temperature < 80.0};
```

若 `manual_override` 已经为 `true`，整个析取条件已经为真，右侧不会求值。

**代码与机制**：`if` 的条件会被按规则解释为 `bool`。整数 `0` 可作为假，非零值可作为真，但业务状态优先使用 `bool`，这样名称与类型共同表达意图：

```cpp
bool sensor_ready{true};
double temperature{31.75};

if (sensor_ready && temperature >= 30.0) {
    std::cout << "alarm\n";
}
```

C++17 标准保证内置 `&&` 和 `||` 的上述从左开始与短路行为。不要把这一保证泛化到任意运算符，也不要在复杂条件右侧塞入大量状态修改；即使结果有定义，也会让路径难以推理。

**错误做法与修复**：

```cpp
bool sensor_ready{false};

if (sensor_ready = true) {  // 赋值表达式把对象改成 true
    std::cout << "ready\n";
}
```

分类：**有明确定义但结果不符合预期**。赋值后的值为 `true`，所以分支会进入；编译器可能给出警告，但不能把警告当作语言要求的编译错误。若只想检查状态，直接写 `if (sensor_ready)`；若确需比较，则写 `if (sensor_ready == true)`，但前一种更直接。

**小检查**：当 `sample_count` 为 `0` 时，`sample_count != 0 && total / sample_count > 2` 的右侧是否求值？把左右条件交换为什么会破坏保护关系？

### 机制四：控制流把表达式结果变成可追踪的执行路径

**实际问题**：传感器程序既要按告警条件二选一，也要按模式代码多选一，还要重复处理若干采样。把所有逻辑塞进嵌套条件会让遗漏路径和错误循环很难定位。

先按“要走几条路”选择最直观的工具：

| 任务 | 先想到的结构 |
|---|---|
| 条件成立走 A，否则走 B | `if / else` |
| 一个离散模式值对应多条分支 | `switch` |
| 重复若干次或持续到条件改变 | `for / while` |

这些语句做的共同事情，是决定当前语句执行完后，下一条执行哪一句。

**概念落点**：[控制流（control flow）](../docs/glossary/day03.md#控制流control-flow)是程序执行时按照语句规则选择下一条要执行语句的过程。表达式提供条件结果，`if`、`switch` 和循环语句据此组织路径。

**代码与机制**：常见结构各自解决不同问题。

```cpp
// 二选一：条件为真执行第一条路径，否则执行第二条路径
if (temperature >= 30.0) {
    std::cout << "high\n";
} else {
    std::cout << "normal\n";
}
```

```cpp
// 离散整数模式的多选一
switch (mode) {
case 1:
    std::cout << "standby\n";
    break;
case 2:
    std::cout << "inspect\n";
    break;
default:
    std::cout << "unknown\n";
    break;
}
```

`switch` 的条件适用于整数或枚举等离散值；每个 `case` 值必须唯一。`break` 结束当前 `switch`。省略 `break` 后继续执行下一个标签是有定义的贯穿行为，不是未定义行为；只有确实需要贯穿时，才应在 C++17 中用 `[[fallthrough]]` 明确表达。

```cpp
int accepted{0};

for (int index{0}; index < 4; ++index) {
    if (index == 1) {
        continue;  // 跳过本轮剩余语句，再执行 ++index 并检查条件
    }
    ++accepted;
}
```

已知重复次数或有清晰计数器时，`for` 通常最紧凑；以某个持续条件为中心时，`while` 更自然。`continue` 只跳过本轮，`break` 结束最内层循环。循环设计还必须回答“状态怎样向终止条件推进”，否则可能永不终止。

**错误做法与修复**：两个 `case` 使用相同常量值：

```cpp
switch (mode) {
case 1:
    break;
case 1:  // 故意错误：重复 case
    break;
}
```

分类：**编译错误**，程序非良构。修复是为不同业务模式分配不同值，或合并本来就相同的路径。另一个常见问题是循环条件依赖 `index`，循环体却从不改变它；这可能产生不终止的运行结果，不应笼统称为编译错误或未定义行为。

**小检查**：`continue` 出现在 `for` 循环时，本轮之后会先执行循环体剩余部分、迭代表达式还是直接离开循环？

## 💻 最小可运行示例

完整代码位于 [`examples/day03/main.cpp`](../examples/day03/main.cpp)。

```cpp
#include <iostream>

int main() {
    const double temperature_celsius{31.75};
    const int whole_temperature{static_cast<int>(temperature_celsius)};
    const bool sensor_ready{true};
    const bool alarm{sensor_ready && temperature_celsius >= 30.0};

    int accepted_samples{0};
    for (int sample_index{0}; sample_index < 4; ++sample_index) {
        if (sample_index == 1) {
            continue;
        }
        ++accepted_samples;
    }

    const int mode{2};

    std::cout << "whole temperature: " << whole_temperature << '\n';
    if (alarm) {
        std::cout << "alarm: on\n";
    } else {
        std::cout << "alarm: off\n";
    }
    std::cout << "accepted samples: " << accepted_samples << '\n';

    switch (mode) {
    case 1:
        std::cout << "mode: standby\n";
        break;
    case 2:
        std::cout << "mode: inspect\n";
        break;
    default:
        std::cout << "mode: unknown\n";
        break;
    }
}
```

预期输出：

```text
whole temperature: 31
alarm: on
accepted samples: 3
mode: inspect
```

观察重点：转换位置显式；告警条件利用 `&&`；循环用 `continue` 跳过一个索引；`switch` 的每条路径用 `break` 收束。

## 🐛 错误代码诊断

### 反例 A：重复 `case`

[`exercises/day03/broken/duplicate_case.cpp`](../exercises/day03/broken/duplicate_case.cpp) 被显著标记为故意错误，不进入正常构建目标。

分类：**编译错误**。同一个 `switch` 中两个 `case` 使用相同值，程序非良构。验证脚本只在编译确实失败时通过，防止错误示例意外变成正常目标。

### 反例 B：整数除法之后才进入 `double`

```cpp
int total{5};
int count{2};
double average{total / count};
```

分类：**有明确定义但结果不符合预期**。结果为 `2.0`。修复为在除法前显式把一个操作数转换成 `double`。

### 反例 C：保护条件放错顺序

```cpp
bool large_average{total / count > 2 && count != 0};
```

当 `count == 0` 时，左侧整数除法已经先发生，会触发**未定义行为**；右侧检查来不及保护。修复为 `count != 0 && total / count > 2`。标准不规定未定义行为必然崩溃。

## 🎤 高频面试实战

本节保留当日主问题的答题框架。先独立口述，再到[第 3 天面试答案](../interview/answers/day03.md)逐题核对所有连续追问。

### 题 1：隐式转换和显式转换有什么区别？`static_cast` 是否代表安全？

**30 秒口头回答**：隐式转换由初始化、赋值、运算或调用等上下文按语言规则触发；显式转换由程序员写出转换意图，例如 `static_cast<int>(value)`。显式写法更容易审查，但不自动保证安全：浮点转整数可能丢失小数，超出目标整数可表示范围还会产生未定义行为，因此要先确认业务精度并检查范围。

**完整答题逻辑**：先按“谁触发”区分，再用 `double` 到 `int` 说明结果规则；接着说明列表初始化会拒绝某些窄化；最后给出显式转换的边界。不要把所有隐式转换都判错，也不要把强制转换当作错误消除器。

**可能连续追问**：

- `double value{5 / 2};` 为什么不是 `2.5`？
- `static_cast<int>(-3.8)` 得到什么？
- `int value{3.5};` 为什么与 `int value = 3.5;` 的诊断不同？
- 浮点值超出 `int` 范围时能否依赖某个固定结果？

**容易失分的说法**：显式转换一定安全；整数除法的结果会因为接收者是 `double` 而重新变成小数；越界转换必然得到最大整数或必然崩溃。

### 题 2：短路求值怎样影响代码正确性？

**30 秒口头回答**：内置 `&&` 和 `||` 从左操作数开始求值。`&&` 左侧为假时不求值右侧，`||` 左侧为真时不求值右侧。它们不仅计算真假，还能让左侧建立右侧的前置条件，例如先检查除数不为零；左右交换可能改变是否执行右侧，从而改变正确性。

**完整答题逻辑**：先分别说清两个停止条件，再给 `count != 0 && total / count > 2`；最后说明短路保证只应在相应运算规则范围内使用，复杂状态修改不宜藏在条件中。

**可能连续追问**：

- `false && function()` 是否调用函数？
- `true || function()` 是否调用函数？
- 为什么交换两个看似等价的条件可能引入除零？
- `if (ready = true)` 是什么错误，是否一定编译失败？
- `switch` 忘记 `break` 是否属于未定义行为？

**容易失分的说法**：右侧先算完再被丢弃；所有逻辑表达式都保证从左到右；把赋值条件说成必然编译错误；把 `switch` 贯穿说成未定义行为。

## ✍️ 当日练习

完成 [`exercises/day03/README.md`](../exercises/day03/README.md)：

- 5 道概念判断；
- 3 组代码输出或错误分析；
- 一道传感器读数筛选小程序。

先写预测和错误类别，再运行合法代码。完成后再查看 [`solutions/day03/README.md`](../solutions/day03/README.md)。

## 🧠 深度思考题

要处理一条温度读数：只有传感器就绪、读数位于业务范围且模式允许时才输出整数显示值。请设计条件顺序，并说明哪些检查必须发生在转换之前、哪些条件适合短路保护、何时用 `if` 而不是 `switch`。不要只给最终代码，要解释每条路径为什么安全。

## ✅ 完成标准

- 能按“分组 → 操作数类型 → 运算规则 → 使用上下文”预测常见表达式。
- 能解释隐式与显式转换的区别，并说明 `static_cast` 不自动保证安全。
- 能用 `&&` 或 `||` 的短路规则保护一个前置条件。
- 能为二选一、离散多选和计数重复分别选择 `if`、`switch` 与循环。
- 能区分重复 `case` 的编译错误、整数除法的意外结果和除零的未定义行为。
