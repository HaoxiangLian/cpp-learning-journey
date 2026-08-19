# 第 10 天：复制资源为什么危险

预计用时：120 分钟。标准主线：C++17。

## 🎯 今日攻坚目标

能够区分拷贝构造与拷贝赋值，解释默认逐成员拷贝对裸资源所有权的风险，并为独占动态数组类实现析构、深拷贝构造和能安全处理旧状态/自赋值的拷贝赋值。

## 🔁 前置知识检查

1. `new int[n]` 必须与哪种释放形式配对？重复释放同一动态数组属于什么错误？
2. 析构函数体在对象销毁过程中的作用是什么？
3. `T second{first};` 创建的是新对象，还是给已有对象赋值？

参考方向：数组用 `delete[]`；重复释放属于未定义行为；析构函数执行类级结束逻辑；声明并初始化 `second` 是新对象初始化，这正是今天区分两种拷贝入口的起点。

## 📖 核心知识重构

只含 `int`、`double`、`std::string` 等值成员的类，编译器生成的逐成员拷贝通常能表达合理语义。但当类用裸指针独占动态数组时，“复制指针值”不会复制数组；若两个对象都认为自己负责 `delete[]`，所有权契约就矛盾了。

### 机制一：默认逐成员拷贝会复制指针值，不会复制所指资源

**实际问题**：`SampleBuffer` 析构时释放 `data_`。如果直接写 `SampleBuffer second{first};` 而类没有自定义拷贝操作，编译器可能生成逐成员拷贝：`size_` 和 `data_` 的值都被复制。此时两个对象的 `data_` 指向同一数组。

```cpp
class BrokenBuffer {
public:
    explicit BrokenBuffer(std::size_t count)
        : size_{count}, data_{new int[count]{}} {}

    ~BrokenBuffer() { delete[] data_; }

private:
    std::size_t size_;
    int* data_;
};

BrokenBuffer first{3};
BrokenBuffer second{first};
```

先画复制后的关系：

| 对象 | `size_` | `data_` 指向 |
|---|---:|---|
| `first` | `3` | 数组 A |
| `second` | `3` | 仍是同一个数组 A |

默认逐成员复制只把地址值复制给 `second.data_`，不会自动新建“数组 B”。但两个析构函数都执行 `delete[] data_`，于是同一个数组会被释放两次。

**概念落点**：

- [资源所有权（resource ownership）](../docs/glossary/day10.md#资源所有权resource-ownership)是对象或程序组件承担在约定时机结束所管理资源的有效使用并执行恰好一次相应释放操作的责任关系。
- [浅拷贝与深拷贝（shallow copy and deep copy）](../docs/glossary/day10.md#浅拷贝与深拷贝shallow-copy-and-deep-copy)：在本课程资源类语境中，浅拷贝只复制资源句柄或指针值，使多个对象指向同一底层资源；深拷贝则为目标建立独立资源并复制资源内容。

**代码与机制**：若没有用户声明的相应拷贝构造函数，语言可能隐式声明并定义一个逐成员拷贝版本。对 `std::size_t`，复制数值；对 `int*`，复制指针值。它不会沿指针自动遍历数组，更不知道该指针是拥有、共享还是观察关系。

离开作用域时两个析构函数都会对同一动态数组执行 `delete[]`。第一次释放后，另一个对象的指针悬空；第二次释放不是“什么也不做”，而是**重复释放导致的未定义行为**。程序不保证崩溃、报错或稳定输出。

注意：浅拷贝并非天然错误。如果类型明确表达共享资源，并以正确机制协调最后一次释放，多个对象指向同一资源可能是设计目标；智能共享所有权在第 18 天学习。本日类承诺独占数组，因此拷贝必须产生独立数组，或明确禁止拷贝。

**错误做法与修复**：故意危险文件 [`exercises/day10/broken/shallow_copy.cpp`](../exercises/day10/broken/shallow_copy.cpp) 可以通过许多编译器的语法检查，但绝不能运行。分类：**双重释放/未定义行为**。修复选择有三种：实现深拷贝、明确删除拷贝操作，或改用能正确表达所有权的标准库成员。

**小检查**：默认复制 `int* data_` 时，复制的是指针对象保存的值，还是 `data_` 指向的整个数组？

### 机制二：拷贝构造函数为新对象建立独立资源

**实际问题**：我们希望 `SampleBuffer copied{original};` 得到相同样本内容，但修改 `copied` 不影响 `original`，且两个对象都能独立析构。

```cpp
SampleBuffer(const SampleBuffer& other)
    : size_{other.size_},
      data_{size_ > 0 ? new int[size_]{} : nullptr} {
    if (size_ > 0) {
        std::copy(other.data_, other.data_ + size_, data_);
    }
}
```

按三步读：先取得 `other.size_`，再为新对象申请自己的数组，最后逐个复制元素。完成后两个数组内容相同，但地址不同；修改副本不会改动原对象。

| 比较项 | `original` | `copied` |
|---|---|---|
| 元素内容 | 例如 `{2, 4, 6}` | 同样是 `{2, 4, 6}` |
| 底层数组 | 数组 A | 新建的数组 B |
| 最终释放 | 自己释放 A | 自己释放 B |

**概念落点**：[拷贝构造函数（copy constructor）](../docs/glossary/day10.md#拷贝构造函数copy-constructor)是用同类对象初始化一个新对象的构造函数，其首个参数通常写作 `const T&`，并负责建立新对象的独立有效状态。

**代码与机制**：`other` 使用 `const&`，因为复制不应修改源对象，也要避免为了传参再产生一次同类拷贝。新对象先复制元素数量，再申请自己的数组，最后复制元素内容。完成后两对象的 `size_` 相同、元素值相同，但 `data_` 指向不同分配。

若分配 `new int[size_]` 失败，普通 `new` 会抛出异常，构造函数体不会正常完成，新 `SampleBuffer` 对象也不会成为一个已完成构造的对象。原对象未被修改。异常传播与构造失败清理在第 19 天系统展开；今天只依赖“先为新对象建立自己的资源，成功后它才成为独立所有者”。

拷贝构造常见于：

```cpp
SampleBuffer copied{original}; // 用已有对象初始化新对象
```

按值传参和按值返回也可能涉及拷贝，但 C++17 在若干情形强制或允许拷贝消除，因此面试时不要断言“每次按值返回一定调用拷贝构造”。应先说语义需要从源初始化目标，再补充拷贝消除边界。

**错误做法与修复**：

```cpp
SampleBuffer(const SampleBuffer& other)
    : size_{other.size_}, data_{other.data_} {}  // 仍是浅拷贝
```

分类：代码可编译，但对独占资源类会产生**共享同一地址并最终重复释放的未定义行为风险**。修复是独立分配并复制内容，或明确删除拷贝构造函数。

**小检查**：深拷贝完成后，源对象与新对象的元素值应有什么关系？`data_` 指针值又应有什么关系？

### 机制三：拷贝赋值必须先处理目标旧资源，并考虑自赋值

**实际问题**：`target = source;` 中，`target` 已经完成初始化，甚至已经拥有另一块数组。若直接覆盖 `target.data_`，旧数组会泄漏；若先删除旧数组再从 `source` 复制，而 `target` 与 `source` 是同一个对象，又会读取已释放数据。

先看赋值前后目标要承担什么：

```cpp
SampleBuffer source{3};
SampleBuffer target{2};
target = source;
```

| 时刻 | `source` 管理 | `target` 管理 |
|---|---|---|
| 赋值前 | 源数组 A | 旧数组 B |
| 准备新状态 | A 保持不变 | 先建立替代数组 C |
| 提交后 | 仍管理 A | 释放 B，改为管理 C |

普通话结论：拷贝构造面对“新对象还没有旧东西”，拷贝赋值面对“目标早已存在，必须安全处理它原来的资源”。

**概念落点**：

- [拷贝赋值运算符（copy assignment operator）](../docs/glossary/day10.md#拷贝赋值运算符copy-assignment-operator)是把一个已有同类对象的状态复制到另一个已经完成初始化的对象中的非静态成员函数，常见签名为 `T& operator=(const T&)`。
- [Rule of Three（三法则）](../docs/glossary/day10.md#rule-of-three三法则)是 C++11 之前形成、在现代 C++ 中仍用于理解资源类的设计规则：若类需要自定义析构函数、拷贝构造函数或拷贝赋值运算符中的一个，通常应一起审视并正确设计三者。

对本日只保存 `int` 的缓冲区，可以先申请替代数组、复制数据，成功后再释放旧数组：

```cpp
SampleBuffer& operator=(const SampleBuffer& other) {
    if (this == &other) {
        return *this;
    }

    int* replacement{
        other.size_ > 0 ? new int[other.size_]{} : nullptr
    };
    if (other.size_ > 0) {
        std::copy(other.data_, other.data_ + other.size_, replacement);
    }

    delete[] data_;
    data_ = replacement;
    size_ = other.size_;
    return *this;
}
```

**代码与机制**：

1. `this == &other` 检测 `object = object`，直接保持原状态。
2. 先申请 `replacement`；若分配失败，`data_` 尚未改变，目标仍保持旧状态。
3. 本例元素是 `int`，复制元素不会抛出用户代码异常；完成复制后才释放旧数组。
4. 接管新数组并更新大小，返回 `*this` 以支持普通赋值表达式约定。

这段顺序为本例提供清晰的“先准备、后提交”路径。对任意会在复制时抛异常的元素类型，仅用裸指针还需要额外清理保护；第 19 天再讨论异常安全等级。本日不要把这个 `int` 特例夸大成万能模板。

Rule of Three 不是标准语法强制：编译器不会因为你写了析构函数就自动要求另外两个函数体。但资源类若只写析构而接受默认拷贝，往往正好落入浅拷贝风险。现代工程更常让 `std::vector`、`std::string` 或智能指针成员管理资源，从而优先 Rule of Zero；移动与 Rule of Five/Zero 在第 11 天展开。

**错误做法与修复**：

```cpp
SampleBuffer& operator=(const SampleBuffer& other) {
    delete[] data_;
    data_ = new int[other.size_];
    // 若 this == &other，此时 other.data_ 也已经悬空。
    return *this;
}
```

自赋值时读取已释放资源会导致**释放后使用/未定义行为**；分配失败时目标旧状态也已经丢失。修复是显式处理自赋值，并采用先准备替代资源、成功后再提交的顺序。

**小检查**：为什么 `target = target;` 不能被当成“不可能发生”？在上面正确实现中，它会改变目标吗？

## 💻 最小可运行示例

完整代码位于 [`examples/day10/main.cpp`](../examples/day10/main.cpp)。

```cpp
#include <algorithm>
#include <cstddef>
#include <iostream>

class SampleBuffer {
public:
    explicit SampleBuffer(std::size_t size)
        : size_{size}, data_{size_ > 0 ? new int[size_]{} : nullptr} {}

    ~SampleBuffer() {
        delete[] data_;
    }

    SampleBuffer(const SampleBuffer& other)
        : size_{other.size_},
          data_{size_ > 0 ? new int[size_]{} : nullptr} {
        if (size_ > 0) {
            std::copy(other.data_, other.data_ + size_, data_);
        }
    }

    SampleBuffer& operator=(const SampleBuffer& other) {
        if (this == &other) {
            return *this;
        }

        int* replacement{
            other.size_ > 0 ? new int[other.size_]{} : nullptr
        };
        if (other.size_ > 0) {
            std::copy(other.data_, other.data_ + other.size_, replacement);
        }

        delete[] data_;
        data_ = replacement;
        size_ = other.size_;
        return *this;
    }

    bool set(std::size_t index, int value) {
        if (index >= size_) return false;
        data_[index] = value;
        return true;
    }

    int value(std::size_t index) const {
        return index < size_ ? data_[index] : 0;
    }

private:
    std::size_t size_;
    int* data_;
};

int main() {
    SampleBuffer original{3};
    original.set(0, 10);
    original.set(1, 20);

    SampleBuffer copied{original};
    copied.set(0, 99);

    SampleBuffer assigned{1};
    assigned = original;
    assigned.set(1, 77);
    const SampleBuffer* same_object{&assigned};
    assigned = *same_object;  // 经别名测试自赋值路径

    std::cout << "original[0]: " << original.value(0) << '\n';
    std::cout << "copied[0]: " << copied.value(0) << '\n';
    std::cout << "original[1]: " << original.value(1) << '\n';
    std::cout << "assigned[1]: " << assigned.value(1) << '\n';
}
```

预期输出：

```text
original[0]: 10
copied[0]: 99
original[1]: 20
assigned[1]: 77
```

修改两个副本后原对象保持不变，证明三者拥有独立数组；程序结束时各析构函数只释放自己的数组。

## 🐛 错误代码诊断

### 反例 A：默认浅拷贝

[`exercises/day10/broken/shallow_copy.cpp`](../exercises/day10/broken/shallow_copy.cpp) 故意保留默认指针拷贝。它可能编译成功，但运行会让两个析构函数释放同一数组，属于**未定义行为**，禁止作为正常目标运行。

### 反例 B：明确禁止拷贝

[`exercises/day10/broken/copy_disabled.cpp`](../exercises/day10/broken/copy_disabled.cpp) 用 `= delete` 明确禁止拷贝，却仍尝试创建副本。分类：**编译错误**。这不是坏设计；当类型在业务上不可复制时，禁止拷贝比伪造深拷贝更准确。

## 🎤 高频面试实战

先独立口述，再到[第 10 天面试答案](../interview/answers/day10.md)逐题核对全部追问。

### 题 1：拷贝构造函数和拷贝赋值运算符何时调用？默认逐成员拷贝为什么会伤害资源类？

**30 秒口头回答**：拷贝构造用于从已有同类对象初始化一个新对象，如 `T b{a}`；拷贝赋值用于把源状态复制到已经存在的目标，如 `b = a`。编译器生成的默认操作通常逐成员复制，复制裸指针时只复制地址值，不复制所指资源。若两个对象都把该地址当作独占资源并在析构时释放，就会产生别名修改、悬空和双重释放风险。

**完整答题逻辑**：先以“目标是否已经存在”区分两个入口；再解释默认逐成员语义；随后把指针值与动态数组分开；最后按独占、共享、不可拷贝三种业务语义说明可选设计。

**可能连续追问**：

- `T b = a;` 是拷贝构造还是先默认构造再赋值？
- 默认拷贝一个 int* 成员时究竟复制什么？
- 按值传参一定能观察到一次拷贝构造调用吗？
- 两个对象共享同一地址一定是错误吗？
- 可以用 = delete 禁止拷贝吗？适合什么类型？

**容易失分的说法**：把所有等号都叫拷贝赋值；说编译器会自动复制指针指向的数组；断言按值传递总有一次物理拷贝；认为禁止拷贝一定是不完整实现。

### 题 2：什么是浅拷贝、深拷贝和 Rule of Three？如何写安全的拷贝赋值？

**30 秒口头回答**：在独占资源类中，浅拷贝只复制句柄或指针值，多个对象指向同一资源；深拷贝为目标创建独立资源并复制内容。Rule of Three 提醒：若自定义析构、拷贝构造或拷贝赋值中的一个，通常要共同审视三者。拷贝赋值还要处理目标旧资源、自赋值和失败路径，常用先准备新资源、成功后再替换旧状态的顺序。

**完整答题逻辑**：先绑定本题的独占语义再定义深浅拷贝；列出三条特殊成员路径；解释 `target = source` 的旧状态；分析 `this == &source`；最后限定本日 `int` 数组实现的异常边界，并引出下一天的 Rule of Zero/Five。

**可能连续追问**：

- 深拷贝是否永远优于浅拷贝？
- 为什么拷贝赋值通常返回 T&？
- 自赋值如果不检查一定会出错吗？
- 为什么应先分配新资源，再释放目标旧资源？
- Rule of Three 是 C++ 标准强制语法吗？

**容易失分的说法**：脱离所有权语义背定义；忘记目标已有资源；声称一种自赋值写法适用于所有实现；把三法则说成编译器强制或让所有类机械手写三者。

## ✍️ 当日练习

完成 [`exercises/day10/README.md`](../exercises/day10/README.md)：5 道判断、3 组拷贝入口与资源错误分析，以及一个由“禁止拷贝”起步、最终实现深拷贝的 `OwnedSamples` 小类。完成后查看 [`solutions/day10/README.md`](../solutions/day10/README.md)。

## 🧠 深度思考题

一个 `CameraFrame` 很大，业务中既有“修改互不影响的独立副本”，也有“多个模块只读观察同一帧”的需求。分别设计独占深拷贝、共享只读和不可拷贝三种语义，说明每种方案由谁释放、复制成本如何、调用点怎样表达意图。不要把“共享一个指针”直接当成完整所有权方案。

## ✅ 完成标准

- 能以目标是否已存在区分拷贝构造与拷贝赋值。
- 能解释默认逐成员拷贝为何只复制裸指针值。
- 能根据独占语义实现独立动态数组深拷贝。
- 能在拷贝赋值中处理旧资源、自赋值和先准备后提交顺序。
- 能准确表述 Rule of Three 是设计提醒而非语言强制，并知道 Rule of Zero/Five 将在下一天展开。

规则依据：C++ 公开工作草案的 [copy/move constructors](https://eel.is/c++draft/class.copy.ctor) 与 [copy/move assignment](https://eel.is/c++draft/class.copy.assign) 条款，以及 C++ Core Guidelines 的 [C.21](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rc-five)；公开草案不是已发布 ISO 标准本身。
