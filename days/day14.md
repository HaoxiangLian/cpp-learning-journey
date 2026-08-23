# 第 14 天：一份代码怎样适配多种类型

预计用时：120 分钟。标准主线：C++17；`static_assert` 来自 C++11，变量模板简写 `std::is_arithmetic_v` 来自 C++17；`concepts` 明确标为 C++20 补充。

## 🎯 今日攻坚目标

完成本日后，你应能按正文顺序：

1. 把只因类型不同而重复的普通函数改成函数模板，并根据调用实参判断模板参数能否被推导；
2. 沿“模板声明 → 具体调用 → 实例化”的顺序定位错误，说明为什么某些操作只在使用特定类型时才报错；
3. 编写保存不同值类型的类模板，并说明 `Reading<int>` 与 `Reading<double>` 为什么是不同类型；
4. 区分显式特化与“限制可接受类型”这两个目的，并用 C++17 `static_assert` 表达简单要求、读懂 C++20 `concept` 补充写法。

## 🔁 前置知识检查

1. Day 4 中，若 `larger_int(int, int)` 与 `larger_double(double, double)` 的函数体完全相同，函数重载能让调用成功，但哪里仍然重复？
2. Day 3 中，表达式 `left < right` 对一个类型能否成立，取决于该类型是否支持什么操作？
3. Day 8 中，类的对象各自保存成员状态；若希望同一类结构有时保存 `int`、有时保存 `double`，直接把成员类型写死会遇到什么限制？
4. Day 1 中，编译错误与运行时错误的发生阶段有什么区别？

参考方向：普通重载可以有相同名字，却仍要重复函数体；比较表达式要求相应类型支持 `<`；写死成员类型只能保存那一种类型；今天的模板推导、实例化和要求检查都发生在编译阶段，不是程序运行后再选择类型。

## 📖 核心知识重构

今天不从“模板元编程”或冗长编译器报错开始。依赖顺序只有四步：先看重复函数怎样合成一份带占位符的代码；再看占位符变成具体类型时发生什么；然后把同一办法用于类；最后才讨论例外实现和类型要求。

### 机制一：函数模板先描述共同算法，调用再确定模板实参

**实际问题**：下面两个普通函数都返回较大值，唯一差别是参数与返回类型。

```cpp
int larger_int(int left, int right) {
    return left < right ? right : left;
}

double larger_double(double left, double right) {
    return left < right ? right : left;
}
```

把重复的类型位置先写成占位符 `T`：

```cpp
template <typename T>
T larger(T left, T right) {
    return left < right ? right : left;
}

larger(3, 7);       // 两个调用实参都是 int
larger(2.5, 1.25);  // 两个调用实参都是 double
```

先只观察调用，不背术语：

| 调用 | 第一个值的类型 | 第二个值的类型 | `T` 能否得到唯一结果 |
|---|---|---|---|
| `larger(3, 7)` | `int` | `int` | 能，`T` 为 `int` |
| `larger(2.5, 1.25)` | `double` | `double` | 能，`T` 为 `double` |
| `larger(3, 4.5)` | `int` | `double` | 不能，两处对 `T` 的结论冲突 |

普通话结论：我们没有在运行时把一个函数切换成不同模式，而是在源代码中写一份共同规则，让编译器针对调用中的类型确定要使用的具体函数。

**概念落点**：本机制中的术语有明确依赖顺序：先认识“模板和占位符”，再认识“调用怎样填入占位符”。

- [函数模板、模板参数与模板实参（function template, template parameter and template argument）](../docs/glossary/day14.md#函数模板模板参数与模板实参function-template-template-parameter-and-template-argument)：模板是用参数描述一族函数、类或变量等实体的声明；函数模板是定义一族函数的模板，模板参数是该声明中的占位符，模板实参是某次使用时为占位符提供的类型、值或模板。
- [模板实参推导（template argument deduction）](../docs/glossary/day14.md#模板实参推导template-argument-deduction)：函数模板实参推导是编译器比较函数形参类型与调用实参类型，尝试确定未显式给出的模板实参的过程。

在 `template <typename T>` 中，`T` 是类型模板参数；在 `larger<int>(3, 7)` 中，`int` 是显式写出的模板实参。`typename` 与这里可写的 `class` 都表示一个类型模板参数，本日统一使用更直观的 `typename`。

模板参数还可以表示编译期值或另一个模板，但那需要新的语法规则；本日只使用类型模板参数，不要求你提前推理其他形式。

**代码与机制**：调用 `larger(3, 7)` 时，编译器分别比较两个函数形参 `T` 与两个调用实参 `int`，都得到 `T = int`，所以推导成功。调用 `larger(3, 4.5)` 时，一处要求 `T = int`，另一处要求 `T = double`；推导阶段不会先随意选一个共同类型，因此没有匹配函数。

如果设计上确实要按 `double` 计算，可以明确意图：

```cpp
larger<double>(3, 4.5);  // 先明确 T = double，参数 3 再转换成 double
```

这与普通函数重载的最近区别是：重载中的每个普通函数都是单独声明的函数；函数模板描述一族可能的函数。二者可以共存，但候选选择还有更完整的重载规则，本日先保证你能判断最小示例中的推导成功或冲突。

**错误做法与修复**：[`exercises/day14/broken/mixed_template_arguments.cpp`](../exercises/day14/broken/mixed_template_arguments.cpp) 直接调用 `larger(3, 4.5)`。分类：**编译错误**，因为同一个 `T` 推导出冲突类型。修复不是盲目增加更多模板参数，而是先决定业务要比较同类型值还是允许转换；本例可统一写成 `double`，或显式调用 `larger<double>`。

**小检查**：`larger(4, 9)` 中模板参数、模板实参和两个函数调用实参分别是什么？为什么 `larger(4, 9.0)` 不能按同一规则推导出唯一 `T`？

### 机制二：模板实例化后，具体类型必须支持模板体所用操作

**实际问题**：模板声明中的 `T` 还不是某个具体类型。只有当调用需要一个具体版本时，编译器才能判断 `T` 是否真的支持 `<`。

```cpp
struct Label {
    int id;
};

template <typename T>
T larger(T left, T right) {
    return left < right ? right : left;
}

larger(3, 7);                  // 可以：int 支持 <
// larger(Label{1}, Label{2}); // 不可以：本例没有为 Label 定义 <
```

按时间顺序观察：

| 时点 | 编译器此时知道什么 | 当前结果 |
|---|---|---|
| 读到模板声明 | 知道有占位符 `T`，并能检查固定语法 | 尚未要求 `T` 是某个具体类型 |
| 读到 `larger(3, 7)` | 推导出 `T = int`，调用需要函数定义 | 检查使用 `int` 的函数，`<` 有效 |
| 读到 `larger(Label{1}, Label{2})` | 推导出 `T = Label`，调用需要函数定义 | 检查使用 `Label` 的函数，`<` 无效，编译失败 |

普通话结论：一份模板能被声明，不等于每一种类型都能使用它。错误不是程序运行到比较时才出现，而是在编译器尝试形成该具体函数时出现。

**概念落点**：[模板实例化（template instantiation）](../docs/glossary/day14.md#模板实例化template-instantiation)：模板实例化是将一组模板实参用于模板，并在程序需要时形成相应具体函数、类或变量定义的过程；由使用自动触发的实例化称为隐式实例化。

`larger(3, 7)` 使程序需要使用 `int` 的具体函数，是一次常见的隐式实例化；`larger(Label{1}, Label{2})` 则在相应实例化中暴露不受支持的 `<`。

**代码与机制**：模板错误要分两层看：

1. 不依赖 `T` 的固定语法和名称，编译器读模板声明时就能检查；少右花括号等错误不会等到调用。
2. `left < right` 是否有效依赖 `T`。只有为某组模板实参检查具体函数时，编译器才有足够信息判断。

因此阅读长报错时，先找两处：最外层是哪一行调用触发了实例化，最内层是哪一个依赖 `T` 的表达式不成立。中间的“required from”或“instantiated from”是调用链，不必逐字背诵。

模板定义还要在需要隐式实例化的位置可见。初学阶段最稳妥的工程规则是：把短小模板的完整定义放在头文件，而不是只把声明放入头文件、把定义藏在普通 `.cpp` 中。显式实例化可形成其他组织方式，但属于后续构建边界。

标准要求的是相应具体定义和程序行为正确；最终二进制是否为每组实参保留一份独立机器码，可能受内联、相同代码合并和未使用代码删除影响，不能仅凭源码数量断言。

**错误做法与修复**：[`exercises/day14/broken/unsupported_instantiation.cpp`](../exercises/day14/broken/unsupported_instantiation.cpp) 把 `Label` 用于要求 `<` 的模板。分类：**编译错误**。修复方向有三个：只传入满足现有操作的类型；为业务类型提供有明确含义的比较操作；或改写函数接口，让比较规则由调用方明确提供。不要为了“通过模板”给类型伪造没有业务含义的顺序。

**小检查**：若删除 `Label` 调用，只保留 `larger(3, 7)`，为什么模板能够通过编译？这是否证明 `larger` 对所有类型都有效？

### 机制三：类模板让相同对象结构保存不同类型的值

**实际问题**：我们既要保存整数计数，也要保存小数温度。复制两份只有成员类型不同的类，会重复构造函数和访问函数。

```cpp
template <typename T>
class Reading {
public:
    explicit Reading(T value) : value_{value} {}

    T value() const { return value_; }

private:
    T value_{};
};

Reading<int> count{9};
Reading<double> temperature{23.5};
```

先看两个对象，而不是先看抽象语法：

| 对象 | 完整类型 | 成员 `value_` 的类型 | 当前值 |
|---|---|---|---:|
| `count` | `Reading<int>` | `int` | 9 |
| `temperature` | `Reading<double>` | `double` | 23.5 |

普通话结论：两个对象沿用同一份类结构，但完整类型不同；它们各自保存状态，也不会因为来自同一个模板就自动互相赋值。

**概念落点**：[类模板（class template）](../docs/glossary/day14.md#类模板class-template)：类模板是以模板参数表示成员类型或其他可变部分、据此定义一族类的模板。

`Reading` 单独写只是类模板的名字；`Reading<int>` 才是本例可用来声明对象的完整类类型。`int` 决定成员 `value_`、构造函数参数和 `value()` 返回值中的 `T` 各自替换成什么。

**代码与机制**：在本日代码中显式写出类模板实参，按下面三步读：

1. 先找到 `Reading<...>` 尖括号中的模板实参；
2. 再把类模板定义中每个相关 `T` 替换成该类型；
3. 最后像普通类一样判断对象创建、成员状态和函数调用。

类模板的具体类被需要时会实例化；其中某个成员函数的定义通常只在程序需要该成员时实例化。初学者先记住可操作规则：报错若指向类模板成员，先确认完整类型中的实参，再检查该成员对实参类型执行了什么。

`Reading<int>` 与 `Reading<double>` 是不同类型，类似 `Sensor` 与 `Lidar` 是不同类型，但这里没有继承关系，也不会自动形成多态关系。模板解决的是“编译时用类型生成结构相似的代码”，不是 Day 13 的“运行时经虚函数选择实现”。

**错误做法与修复**：下面的赋值不能仅因二者来自同一类模板就成立：

```cpp
Reading<int> count{9};
Reading<double> temperature{23.5};
// temperature = count;  // 不同类型，编译错误
```

分类：**编译错误**。修复是先明确是否真的需要转换；若需要，可读取 `count.value()`、进行明确的数值转换，再构造 `Reading<double>`。不要用强制内存解释把两个无继承关系的类型冒充成同一对象。

**小检查**：`Reading<int>` 中构造函数参数、成员和返回值分别是什么类型？为什么它不能直接赋给 `Reading<double>`？

### 机制四：显式特化处理例外实现，要求检查决定哪些实参可用

**实际问题**：有两个不同目的容易被混在一起。第一个是“绝大多数类型显示 `other`，但 `bool` 显示 `boolean`”；第二个是“`Reading<T>` 只允许数值类型”。先看各自代码结果：

```cpp
template <typename T>
struct TypeName {
    static const char* get() { return "other"; }
};

template <>
struct TypeName<bool> {
    static const char* get() { return "boolean"; }
};

TypeName<int>::get();   // other
TypeName<bool>::get();  // boolean
```

C++17 主线中，简单的类型要求可先写成：

```cpp
template <typename T>
class Reading {
    static_assert(std::is_arithmetic_v<T>,
                  "Reading<T> requires an arithmetic type");
    // ...
};

Reading<int> count{9};
// Reading<std::string> label{"ready"};  // static_assert 失败
```

| 需求 | 代码入口 | 对当前例子的作用 |
|---|---|---|
| 为一个完整类型提供例外实现 | `template <> struct TypeName<bool>` | `bool` 不使用一般的 `other` 版本 |
| 拒绝不符合要求的类型 | `static_assert(std::is_arithmetic_v<T>)` | C++17 中实例化 `Reading<std::string>` 时给出诊断 |

普通话结论：例外实现回答“这个类型要做得不一样”；类型要求回答“这个类型能不能进入”。二者不是同一种修补手段。

**概念落点**：本机制按“一般版本 → 完整例外 → 可接受条件 → C++20 命名条件”的顺序引入术语。

[主模板、显式特化、约束与 `concept`（primary template, explicit specialization, constraint and concept）](../docs/glossary/day14.md#主模板显式特化约束与-conceptprimary-template-explicit-specialization-constraint-and-concept)：主模板是给出一族模板实体一般形式的模板声明；显式特化是为一组完整指定的模板实参另行声明或定义版本，使该组实参不再使用由主模板生成的版本。约束是一组模板实参必须满足的要求；`concept` 是 C++20 用名称表达并可由模板声明引用的约束。

本例第一个 `TypeName<T>` 是主模板，`TypeName<bool>` 是完整指定 `bool` 的显式特化。`Reading` 中的 `static_assert` 是 C++17 可用的简单要求检查，但它不是 C++20 语言中的约束，也不会在重载选择前把候选移除。

**代码与机制**：显式特化必须建立在已经声明的主模板上，并且其声明要在会触发相应隐式实例化的使用之前可见。[`exercises/day14/broken/late_specialization.cpp`](../exercises/day14/broken/late_specialization.cpp) 先使用 `TypeCode<bool>`，随后才补特化，属于**编译错误**；修复是把特化放到第一次相应用途之前，并让所有需要它的翻译单元都能看到声明。

不要为“所有整数、所有可比较类型”等宽泛要求枚举大量显式特化。C++17 可用 `static_assert` 给简单要求提供清楚消息；更复杂的重载筛选还有 SFINAE 等技术，本日不提前展开。

下面只是 **C++20 补充**，不进入本仓库 C++17 构建目标：

```cpp
#include <concepts>

template <std::integral T>
class Counter {
    T value_{};
};
```

`std::integral` 是 C++20 标准库 concept，要求 `T` 是整数类型。与函数体中的 `static_assert` 相比，语言约束能在形成模板候选和检查约束时更早表达“不适用”，通常也能让接口意图与诊断更直接。它不会让运行时多一次类型检查；条件仍在编译阶段处理。

对于函数模板，若只是想为某种参数提供更自然的不同实现，普通函数重载往往比函数模板显式特化更容易阅读；本日用类模板特化隔离基本规则，不扩展到偏特化和复杂选择顺序。

**错误做法与修复**：把 `TypeCode<bool>` 使用后才出现的显式特化当作“后面补上也一样”。分类：**编译错误**。修复是先声明主模板，再声明/定义特化，最后才出现会实例化该类型的使用。若真实目的只是限制可接受类型，则应写明要求，而不是不断添加例外实现。

**小检查**：`TypeName<bool>` 为什么不使用主模板？若目标是让 `Reading<T>` 只接受数值类型，应该优先表达要求还是为每个非数值类型写特化？C++20 `concept` 会在运行时检查吗？

## 💻 最小可运行示例

完整 C++17 代码位于 [`examples/day14/main.cpp`](../examples/day14/main.cpp)。它依次展示函数模板推导、两个不同的类模板具体类型、C++17 `static_assert` 要求，以及 `bool` 的类模板显式特化。

关键调用：

```cpp
std::cout << larger(4, 9) << '\n';

Reading<int> count{9, "items"};
Reading<double> temperature{23.5, "C"};

std::cout << TypeName<int>::get() << '\n';
std::cout << TypeName<bool>::get() << '\n';
```

稳定输出包含：

```text
larger int: 9
larger double: 3.5
count reading: 9 items
temperature reading: 23.5 C
type int: other
type bool: boolean
```

## 🐛 错误代码诊断

### 反例 A：两个调用实参不同，推导会自动选择“更大”的类型

[`exercises/day14/broken/mixed_template_arguments.cpp`](../exercises/day14/broken/mixed_template_arguments.cpp) 对同一个 `T` 得到 `int` 与 `double` 两个冲突结论。分类：**编译错误**。先统一实参类型或显式给出有业务依据的模板实参。

### 反例 B：模板已经声明，所以任何类型都能调用

[`exercises/day14/broken/unsupported_instantiation.cpp`](../exercises/day14/broken/unsupported_instantiation.cpp) 在 `Label` 的实例化中发现 `<` 无效。分类：**编译错误**。模板只对满足其所用操作的类型有效。

### 反例 C：同一类模板生成的完整类型可以直接互相赋值

`Reading<int>` 与 `Reading<double>` 是不同类型；没有相应转换或赋值入口时直接赋值是**编译错误**。应明确读取、转换并创建目标类型，而不是按内存布局强制解释。

### 反例 D：显式特化写在第一次使用之后也来得及

[`exercises/day14/broken/late_specialization.cpp`](../exercises/day14/broken/late_specialization.cpp) 先触发 `bool` 版本，再定义特化。分类：**编译错误**。特化声明必须在相应隐式实例化用途之前可见。

## 🎤 高频面试实战

先独立口述，再到[第 14 天面试答案](../interview/answers/day14.md)逐题核对全部追问。

### 题 1：函数模板怎样生成可调用函数？模板实参推导和实例化分别做什么？

**30 秒口头回答**：函数模板用模板参数描述一族函数。调用时若未显式给出模板实参，编译器先比较函数形参与调用实参进行推导；得到唯一实参并选中模板后，程序需要函数定义时进行实例化，再检查具体类型是否支持模板体中的操作。推导失败和实例化失败都是编译期问题，但原因不同：前者没有确定唯一模板实参，后者是具体版本中的表达式无效。

**完整答题逻辑**：先用 `larger(T, T)` 说明 `T` 是模板参数；再分别追踪 `larger(3, 7)`、`larger(3, 4.5)` 与 `larger(Label{}, Label{})`。第一种推导并实例化成功，第二种推导冲突，第三种推导成功但实例化时发现 `<` 无效。最后说明模板定义通常放在头文件以便需要实例化的翻译单元可见，并把源码规则与最终机器码数量分开。

**可能连续追问**：

- `larger(3, 4.5)` 为什么通常推导失败？
- `larger<double>(3, 4.5)` 与自动推导相比改变了什么？
- 编译器读到函数模板定义时，是否已经生成了所有具体函数？
- 为什么模板报错经常同时指向调用行和模板体中的某一行？
- 为什么函数模板定义通常放在头文件？

**容易失分的说法**：把模板说成运行时类型分支；混淆模板实参与函数调用实参；声称推导会任选公共类型；认为模板声明成功就保证任意实例化成功；把每组实参必然对应一份最终机器码当成标准保证。

### 题 2：类模板和显式特化分别解决什么问题？`concepts` 改善了什么？

**30 秒口头回答**：类模板用模板参数描述一族结构相同的类，`Reading<int>` 与 `Reading<double>` 是不同的完整类型。主模板提供一般实现，显式特化为一组完整实参提供例外实现，且必须在相应隐式实例化用途之前可见。若目的是限制可接受类型，应表达要求；C++17 可用 `static_assert` 做简单检查，C++20 `concepts` 能命名约束并让它参与模板候选与约束检查。

**完整答题逻辑**：先从两个 `Reading` 对象说明模板实参属于完整类型；再用 `TypeName<bool>` 对比一般版本与例外版本；随后分开“实现不同”和“是否允许”两个目的。最后明确 `static_assert` 是 C++17 可用的函数体或类体检查，C++20 `concept` 是语言约束工具，两者都不是运行时类型判断。

**可能连续追问**：

- `Reading<int>` 与 `Reading<double>` 是同一个类型吗？
- 显式特化可以写在主模板声明之前吗？
- 显式特化为什么必须在相应隐式实例化用途之前可见？
- 如果只想接受数值类型，为什么不应为每个非数值类型写显式特化？
- C++17 `static_assert` 与 C++20 `concept` 的边界是什么？

**容易失分的说法**：把模板具体类型说成同一类型的运行时模式；把显式特化当成普通继承；忽略特化位置；用例外实现代替一般要求；把 `concept` 说成 C++17 功能或运行时检查。

## ✍️ 当日练习

完成 [`exercises/day14/README.md`](../exercises/day14/README.md)：6 道判断、5 道推导/实例化分析，以及一个函数模板、类模板和显式特化组合小程序。答案与参考实现位于 [`solutions/day14/README.md`](../solutions/day14/README.md)，请先写出每次调用的 `T`、实例化所需操作和完整类类型，再查看答案。

## 🧠 深度思考题

机器人日志中既有整数计数，也有小数温度和字符串标签。设计一个小接口方案，说明哪些重复代码适合函数模板，哪些状态适合类模板；若 `bool` 的显示文本需要例外，为什么可以使用显式特化；若某个容器只允许数值，为什么应表达要求而不是枚举所有禁止类型。最后说明 C++17 与 C++20 版本各在哪里检查这些条件。

## ✅ 完成标准

- 能指出函数模板中的模板参数、模板实参和函数调用实参，并判断最小调用能否推导。
- 能区分“推导没有唯一结果”和“推导成功但实例化操作无效”两类编译错误。
- 能独立写出一个类模板，并把 `Reading<int>` 与 `Reading<double>` 当作不同完整类型分析。
- 能说明主模板、显式特化和类型要求分别解决什么问题，并识别特化位置错误。
- 能明确 C++17 `static_assert` 与 C++20 `concepts` 的标准版本边界。

规则依据：C++ 公开工作草案的 [templates preamble](https://eel.is/c++draft/temp.pre)、[function-call template argument deduction](https://eel.is/c++draft/temp.deduct.call)、[implicit instantiation](https://eel.is/c++draft/temp.inst)、[explicit specialization](https://eel.is/c++draft/temp.expl.spec) 与 [template constraints](https://eel.is/c++draft/temp.constr) 条款，以及 C++ Core Guidelines 的 [Templates and generic programming](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#S-templates)；公开工作草案不是已发布 ISO 标准本身。
