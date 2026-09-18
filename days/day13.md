# 第 13 天：运行时多态与对象模型

预计用时：120 分钟。标准主线：C++17；`override` 来自 C++11。

## 🎯 今日攻坚目标

完成本日后，你应能按正文顺序：

1. 比较同一基类引用调用非虚函数和虚函数的结果，并说明运行时多态成立的条件；
2. 用 `override` 检查派生函数是否真正覆盖基类虚函数，并识别 `const` 等签名不一致；
3. 用纯虚函数声明必须由具体派生类提供的操作，并判断一个类能否直接创建对象；
4. 用 `sizeof`、`alignof` 观察当前实现的对象表示，并区分对齐/填充规则与 vtable 常见实现。

## 🔁 前置知识检查

1. `const Sensor& view{lidar};` 会不会创建独立 `Sensor` 对象？为什么它不会发生对象切片？
2. `Sensor copied{lidar};` 与上面的引用绑定有什么不同？
3. Day 12 的虚析构函数解决了通过基类指针删除完整派生对象的什么问题？

参考方向：引用只绑定到原 `Lidar` 的基类子对象；按值声明会创建独立 `Sensor` 并切掉派生新增状态；虚析构让删除从完整派生对象开始。今天继续回答：**完整派生对象仍然存在时，普通成员函数调用会不会自动选择派生版本？**

## 📖 核心知识重构

Day 12 已经确认：基类引用不会切片，但“完整派生对象仍在”并不等于“所有函数都自动调用派生版本”。今天按四步学习：先比较非虚与虚调用，再让编译器检查覆盖关系，然后用抽象类表达统一接口，最后才讨论编译器可能怎样布置对象。

### 机制一：基类引用只有调用虚函数时才按完整对象选择实现

**实际问题**：`inspect` 只接收 `Sensor&`，实际传入的是 `Lidar`。下面两个函数在基类中的声明只差一个 `virtual`，输出却不同。

```cpp
class Sensor {
public:
    std::string label() const { return "Sensor"; }
    virtual int sample_rate() const { return 10; }
};

class Lidar : public Sensor {
public:
    std::string label() const { return "Lidar"; }
    int sample_rate() const { return 64; }
};

void inspect(const Sensor& sensor) {
    std::cout << sensor.label() << ' '
              << sensor.sample_rate() << '\n';
}

Lidar lidar;
inspect(lidar);  // Sensor 64
```

先沿调用过程观察：

| 观察项 | `sensor.label()` | `sensor.sample_rate()` |
|---|---|---|
| `sensor` 引用的完整对象 | 都是同一个 `Lidar` | 都是同一个 `Lidar` |
| 基类中的函数是否写了 `virtual` | 否 | 是 |
| 最后执行 | `Sensor::label()` | `Lidar::sample_rate()` |
| 是否创建新对象 | 否 | 否 |

普通话结论：引用避免了切片，却不会让普通函数自动选择派生版本；真正改变调用选择规则的是基类函数上的 `virtual`。

**概念落点**：先给函数入口命名，再给调用行为命名。

- [虚函数（virtual function）](../docs/glossary/day13.md#虚函数virtual-function)是在类中首次用 `virtual` 声明、并允许通过基类接口调用时根据完整对象选择相应派生实现的非静态成员函数；派生类中符合规则的对应版本也继续为虚函数。
- 基类里某个函数一旦声明成 `virtual`，派生类对它进行正确重写后，派生类里的这个函数即使不再写 `virtual`，它也仍然是虚函数。
- [运行时多态与动态绑定（runtime polymorphism and dynamic binding）](../docs/glossary/day13.md#运行时多态与动态绑定runtime-polymorphism-and-dynamic-binding)：运行时多态是在==通过基类接口调用虚函数时，根据该接口实际引用或指向的完整对象选择相应函数实现的行为==；这种调用选择称为动态绑定。

这里的“完整对象”就是实际创建的对象。本例创建的是 `Lidar`，`sensor` 只是引用其中的 `Sensor` 基类子对象。

**代码与机制**：要在本例中观察到运行时多态，需要同时满足：

1. 完整的派生对象仍然存在，没有按值切成基类对象；
2. 基类指针或引用实际指向该对象的基类子对象；
3. 基类把当前成员函数声明为 `virtual`；
4. 派生类提供符合覆盖规则的对应函数；
5. 调用没有写成 `sensor.Sensor::sample_rate()` 这种显式限定的基类调用。

非虚函数根据调用表达式中写明的类型选择。本例 `sensor` 写作 `const Sensor&`，所以非虚的 `label()` 选择 `Sensor::label()`。虚函数则根据它实际引用的完整 `Lidar` 选择对应实现。

如果改成按值参数 `void inspect(Sensor sensor)`，参数本身就是一个被切片的新 `Sensor`，完整 `Lidar` 已不在参数中；即使 `sample_rate` 是虚函数，也只能调用该 `Sensor` 对象对应的实现。

到这里不需要 vtable 才能预测结果。标准保证的是调用应表现为选择相应实现；编译器怎样做到，留到机制四作为第二遍补充。

**错误做法与修复**：只把参数从 `Sensor` 值改成 `const Sensor&`，却忘记把需要多态选择的基类函数声明为 `virtual`。代码有明确定义，但仍调用基类普通函数，属于**结果不符合设计预期**。修复是只对确实需要派生行为替换的接口使用虚函数，并在机制二用 `override` 检查派生声明。

**小检查**：为什么同一个 `sensor` 引用调用 `label()` 得到 `Sensor`，调用 `sample_rate()` 却得到 `64`？其中有没有发生切片？
	因为label没有用virtual声明为虚函数，samle_rate用virtual声明成了虚函数，允许基类接口根据传输进的完整派生对象实体选择相应的派生实现函数；没有，因为接口是引用派生实体。
### 机制二：`override` 让编译器检查“看起来相同”是否真的覆盖

**实际问题**：我们想让 `Lidar::sample_rate` 替换基类版本，却漏写了末尾的 `const`。

```cpp
class Sensor {
public:
    virtual int sample_rate() const { return 10; }
};

class Lidar : public Sensor {
public:
    int sample_rate() { return 64; }  // 少了 const
};
```

这两个成员并不是同一个调用入口：

| 调用 | 能调用哪个声明 | 结果 |
|---|---|---:|
| `Lidar lidar; lidar.sample_rate();` | 非 `const` 的 `Lidar::sample_rate()` | 64 |
| `const Sensor& sensor{lidar}; sensor.sample_rate();` | `Sensor::sample_rate() const` | 10 |
| 把派生声明改成 `int sample_rate() override` | 编译器发现没有覆盖 `const` 版本 | 编译错误 |

普通话结论：同名不等于同一个虚函数入口。`override` 不负责开启多态，而是要求编译器证明“这个派生函数确实在覆盖”。

**概念落点**：[覆盖与 `override`（overriding and override specifier）](../docs/glossary/day13.md#覆盖与-overrideoverriding-and-override-specifier)：函数覆盖是派生类成员函数满足基类虚函数的覆盖规则并成为相应对象候选最终实现的关系；`override` 是写在派生函数声明末尾的 C++11 说明符，若该函数没有真正覆盖基类虚函数，程序即为非良构。

“非良构”表示程序不符合语言规则，正常编译必须给出诊断；在本练习中表现为编译错误。

**代码与机制**：正确声明为：

```cpp
class Lidar : public Sensor {
public:
    int sample_rate() const override { return 64; }
};
```

初学阶段逐项核对：函数名、参数列表、末尾 `const`、引用限定（若有）和返回类型兼容性。派生函数即使不重复写 `virtual`，只要真正覆盖了基类虚函数，它仍然是虚函数；推荐写 `override`，因为它把人的设计意图变成编译器检查。

`override` 与函数重载不是一回事。重载是在同一可见范围内提供不同参数入口；覆盖发生在继承层次中，前提是基类入口已经是虚函数并满足覆盖规则。

**错误做法与修复**：[`exercises/day13/broken/mismatched_override.cpp`](../exercises/day13/broken/mismatched_override.cpp) 故意让派生函数漏掉 `const` 却写 `override`。分类：**编译错误**。修复是让派生声明与基类虚函数匹配，而不是删除 `override` 来掩盖错误。

**小检查**：基类写了 `virtual int read() const`，派生类写 `int read()`，为什么同名仍不覆盖？删除 `override` 能否真正修复多态行为？

### 机制三：纯虚函数规定必须提供的行为，抽象类不能直接创建对象

**实际问题**：所有传感器都必须能 `read()`，但“普通 Sensor 应该读出什么”没有合理答案。与其返回伪造的默认值，不如只在基类规定接口，让具体传感器提供实现。

```cpp
class Sensor {
public:
    virtual ~Sensor() = default;
    virtual const char* name() const = 0;
    virtual double read() const = 0;
};

class FixedSensor : public Sensor {
public:
    FixedSensor(const char* name, double value)
        : name_{name}, value_{value} {}

    const char* name() const override { return name_; }
    double read() const override { return value_; }

private:
    const char* name_;
    double value_;
};
```

| 类型 | `name()` 是否已有具体最终实现 | `read()` 是否已有具体最终实现 | 能否直接创建对象 |
|---|---|---|---|
| `Sensor` | 否 | 否 | 不能 |
| `FixedSensor` | 是 | 是 | 能 |

普通话结论：`Sensor` 只规定“每种具体传感器都必须回答什么”，`FixedSensor` 才给出可执行答案。

**概念落点**：[纯虚函数与抽象类（pure virtual function and abstract class）](../docs/glossary/day13.md#纯虚函数与抽象类pure-virtual-function-and-abstract-class)：纯虚函数是用 `= 0` 纯说明符声明的虚函数；抽象类是至少有一个最终覆盖函数仍为纯虚、因而不能直接创建该类对象的类。

**代码与机制**：抽象类不能按值创建对象，也不能作为需要创建值对象的函数参数或返回值；但可以声明指针和引用，因为它们能够指向或引用一个完整的具体派生对象：

```cpp
FixedSensor force{"force", 12.5};
const Sensor& sensor{force};
std::cout << sensor.name() << ": " << sensor.read();
```

抽象类并非“只能写纯虚函数”。它仍可有构造函数、数据成员和已经实现的普通或虚函数。具体派生类只有为继承来的所有纯虚入口提供具体最终实现后，才能直接创建对象。

`= 0` 不能与函数体写在同一条类内声明中。纯虚函数在特定情况下可以另行提供定义，但它仍保持“纯”的性质；初学阶段不要用这个边界替代普通的接口设计。

**错误做法与修复**：[`exercises/day13/broken/instantiate_abstract.cpp`](../exercises/day13/broken/instantiate_abstract.cpp) 直接创建 `Sensor sensor;`。分类：**编译错误**。修复是创建已经实现全部纯虚函数的具体派生对象，再通过 `Sensor&` 或 `Sensor*` 使用统一接口；不要为了通过编译给基类伪造无意义读数。

**小检查**：为什么 `Sensor* pointer;` 可以声明，而 `Sensor sensor;` 不能？若派生类只实现 `read()`、没有实现 `name()`，它能否直接创建对象？

### 机制四：先用 `sizeof` 和 `alignof` 观察，再把 vtable 放在实现边界

**实际问题**：类中只写了一个 `char` 和一个 `int`，对象大小是否一定等于两个成员大小之和？有虚函数后又是否一定只多一个指针？

```cpp
struct ReadingRecord {
    char valid;
    int value;
};

std::cout << sizeof(char) + sizeof(int) << '\n';
std::cout << sizeof(ReadingRecord) << '\n';
std::cout << alignof(ReadingRecord) << '\n';
```

不要先猜固定数字，先分清三个问题：

| 表达式 | 回答什么 | 能否从成员列表直接写死结果 |
|---|---|---|
| `sizeof(char) + sizeof(int)` | 两个成员类型各自大小之和 | 能在当前实现计算 |
| `sizeof(ReadingRecord)` | 一个完整对象占用的字节数，包含必要填充 | 不能跨实现写死 |
| `alignof(ReadingRecord)` | 完整对象放置地址需要满足的对齐要求 | 不能跨实现写死 |

普通话结论：对象中可能有不对应任何普通成员值的间隔字节；它们不是“隐藏的新数据成员”。先查询当前编译器结果，再讨论原因。

**概念落点**：[对齐与填充（alignment and padding）](../docs/glossary/day13.md#对齐与填充alignment-and-padding)：对齐要求是对象类型对其可放置地址施加的限制，可用 `alignof(T)` 查询完整对象情形的要求；填充是实现为满足对齐和数组连续放置等要求而在类对象表示中保留、但不对应普通成员值的字节。

**代码与机制**：C++17 层面应先记住：

1. `sizeof(T)` 给出一个 `T` 完整对象占用的字节数；对类类型，它包含为连续数组元素正确放置所需的填充。
2. `alignof(T)` 查询完整 `T` 对象的对齐要求。
3. 类中填充的数量与位置属于具体实现的属性，不能凭一台机器的结果写成跨平台标准结论。
4. 不满足类型对齐要求却在相应存储上创建对象，会导致未定义行为；未定义行为不保证必然崩溃。

主流编译器常用 **vtable** 和 **vptr** 实现虚调用：可以把 vtable 暂时理解为“编译器为某类准备的一张虚函数入口表”，把 vptr 理解为“对象中帮助找到相应表的隐藏指针”。这能解释为什么多态类在许多平台上会变大，但它只是常见实现模型：C++ 标准没有规定必须存在名为 vtable/vptr 的结构、它位于对象何处、占几个字节，或多态类大小一定增加多少。

编译器在能证明完整对象类型时还可能把虚调用优化成直接调用；只要程序可观察行为仍符合虚函数规则，这不改变源代码层面的结论。

**错误做法与修复**：假定 `sizeof(PolymorphicSensor) == sizeof(int) + sizeof(void*)`，再按猜测偏移用强制类型转换读写对象内部字节。前一个假设不具可移植性，后者还可能违反类型、生命周期或对齐规则并触发**未定义行为**。修复是通过成员函数访问状态，用 `sizeof`/`alignof` 查询当前实现；若进行底层对象表示编程，必须另外证明适用的标准规则，而不能把 vtable 示意图当作保证。

**小检查**：`sizeof` 和 `alignof` 分别回答什么？“常见编译器给多态对象放一个 vptr”属于标准保证还是实现说明？

## 💻 最小可运行示例

完整代码位于 [`examples/day13/main.cpp`](../examples/day13/main.cpp)。它同时展示非虚调用、虚调用、抽象接口和当前实现的大小/对齐观察。

关键代码：

```cpp
Lidar lidar{64};
const Sensor& sensor{lidar};

std::cout << "nonvirtual family: " << sensor.family() << '\n';
std::cout << "virtual kind: " << sensor.kind() << '\n';
std::cout << "sample rate: " << sensor.sample_rate() << '\n';
```

稳定的前三行输出是：

```text
nonvirtual family: sensor
virtual kind: lidar
sample rate: 64
```

示例还会输出 `sizeof` 与 `alignof` 的当前实现结果；数字可以因编译器和平台而不同，不作为跨平台背诵答案。

## 🐛 错误代码诊断

### 反例 A：引用没有切片，所以所有函数都会动态选择

错误。引用只保留完整派生对象；非虚函数仍按调用表达式中写明的类型选择。分类：**代码有定义但结果可能不符合设计预期**。修复是只把需要替换行为的基类接口声明为虚函数。

### 反例 B：签名不匹配却声称覆盖

[`exercises/day13/broken/mismatched_override.cpp`](../exercises/day13/broken/mismatched_override.cpp) 的派生函数漏写 `const` 却标记 `override`。分类：**编译错误**。保留 `override`，修正函数签名。

### 反例 C：直接创建抽象类对象

[`exercises/day13/broken/instantiate_abstract.cpp`](../exercises/day13/broken/instantiate_abstract.cpp) 尝试实例化仍有纯虚函数的 `Sensor`。分类：**编译错误**。应创建具体派生对象并经基类引用或指针使用。

### 反例 D：把 vtable 示意图当成固定内存布局

写死 vptr 位置和对象大小不具可移植性；按猜测偏移访问还可能产生**未定义行为**。标准只要求虚调用表现正确，不规定统一虚表结构。

## 🎤 高频面试实战

先独立口述，再到[第 13 天面试答案](../interview/answers/day13.md)逐题核对全部追问。

### 题 1：运行时多态成立需要哪些条件？`override` 有什么作用？

**30 秒口头回答**：常见运行时多态需要完整派生对象仍存在、基类指针或引用指向它、基类接口是虚函数，并且派生类提供符合规则的覆盖；调用时才会根据完整对象选择相应实现。`override` 不负责让函数变虚，它要求编译器检查派生声明确实覆盖了基类虚函数，可及时发现参数、`const` 或引用限定不匹配。

**完整答题逻辑**：先比较基类引用调用非虚与虚函数；再列出对象、接口、虚函数和覆盖四项条件；用按值切片说明“引用/指针保留完整对象”的必要性；最后说明显式限定基类调用会抑制虚调用，`override` 是编译期检查而非运行时开关。

**可能连续追问**：

- 只有基类引用，没有 `virtual`，会发生动态绑定吗？
- 派生函数不写 `override`，还可能真正覆盖吗？
- 基类是 `virtual void f() const`，派生类写 `void f()` 会怎样？
- 把派生对象按值传给基类参数后，虚函数还能调用派生实现吗？
- 写成 `object.Base::f()` 时还会动态绑定吗？

**容易失分的说法**：把引用与多态画等号；认为 `override` 开启虚调用；忽略切片和签名限定；断言源码中的每个虚调用都必须通过一次虚表间接跳转。

### 题 2：纯虚函数和抽象类有什么作用？vtable 是标准保证吗？

**30 秒口头回答**：纯虚函数用 `= 0` 表示当前类只规定必须提供的虚接口；若类仍有纯虚的最终实现，它就是抽象类，不能直接创建对象，但可通过指针或引用指向具体派生对象。vtable/vptr 是主流编译器实现虚调用的常见方式，不是 C++ 标准规定的数据结构；对象大小、对齐和填充应由 `sizeof`、`alignof` 在具体实现上查询。

**完整答题逻辑**：先说明为什么基类不应伪造通用读数；再区分抽象类型本身不能按值创建与基类指针/引用可以使用；随后把标准保证的动态调用行为与常见 vtable 实现分开；最后用 `sizeof`、`alignof` 说明布局数字依赖实现。

**可能连续追问**：

- 抽象类能否有构造函数、数据成员和已经实现的成员函数？
- 声明抽象类指针和直接创建抽象类对象有什么区别？
- 派生类漏掉一个纯虚函数的实现会怎样？
- C++ 标准是否规定每个多态对象都有一个 vptr？
- 能否断言多态类一定比非多态类大一个指针？

**容易失分的说法**：把抽象类说成“不能有实现”；认为 `= 0` 等于空函数体；把 vtable 名称、位置和大小说成标准保证；把一台机器的 `sizeof` 当作所有平台结论。

## ✍️ 当日练习

完成 [`exercises/day13/README.md`](../exercises/day13/README.md)：5 道判断、4 道调用与对象状态分析，以及一个抽象传感器接口小程序。答案与参考实现位于 [`solutions/day13/README.md`](../solutions/day13/README.md)，请先手写每次调用选择的函数，再运行代码核对。

## 🧠 深度思考题

机器人系统有 `Sensor`、`Lidar` 和 `ForceSensor`。设计一个只读报告接口，使报告函数不需要知道具体传感器类型，却能输出各自名称和读数。说明哪些函数应为纯虚函数，为什么参数使用 `const Sensor&`，怎样避免对象切片，以及为什么不能依据你电脑上的对象大小猜测 vptr 的固定位置。

## ✅ 完成标准

- 能从一段代码判断调用的是非虚函数还是虚函数，并预测基类引用下的输出。
- 能写出匹配的覆盖函数并用 `override` 发现 `const` 等签名错误。
- 能说明抽象类为什么不能直接实例化，以及基类指针/引用为什么仍可使用。
- 能准确区分虚函数的标准行为与 vtable/vptr 常见实现。
- 能用 `sizeof`、`alignof` 查询当前实现，而不把填充和对象大小写成跨平台常量。

规则依据：C++ 公开工作草案的 [virtual functions](https://eel.is/c++draft/class.virtual)、[abstract classes](https://eel.is/c++draft/class.abstract)、[alignment](https://eel.is/c++draft/basic.align) 与 [`sizeof`](https://eel.is/c++draft/expr.sizeof) 条款，以及 C++ Core Guidelines 的 [C.128](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-override)；公开工作草案不是已发布 ISO 标准本身。
