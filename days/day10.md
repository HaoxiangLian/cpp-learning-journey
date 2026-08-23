# 第 10 天：复制资源为什么危险

预计用时：120 分钟。标准主线：C++17。

## 🎯 今日攻坚目标

完成本日后，你应能按正文顺序：

1. 先根据目标对象是否已经存在，区分拷贝构造与拷贝赋值，并说明二者的作用；
2. 解释编译器生成的逐成员拷贝为什么只复制裸指针保存的地址；
3. 为独占动态数组类实现建立独立资源的深拷贝构造；
4. 为已有目标实现能处理旧资源、自赋值和失败路径的拷贝赋值，并说明 Rule of Three 的用途。

## 🔁 前置知识检查

1. `new int[n]` 必须与哪种释放形式配对？重复释放同一动态数组属于什么错误？
2. 析构函数体在对象销毁过程中的作用是什么？
3. `T second{first};` 创建的是新对象，还是给已有对象赋值？

参考方向：数组用 `delete[]`；重复释放属于未定义行为；析构函数执行类级结束逻辑；声明并初始化 `second` 是新对象初始化，这正是今天区分两种拷贝入口的起点。

## 📖 核心知识重构

第 9 天学习了“怎样创建一个对象、怎样结束一个对象”。今天只增加一个新问题：**已经有一个对象时，怎样得到内容相同的另一个对象？**

先从只保存整数的简单类开始，认清复制的两个入口；再把成员换成拥有动态数组的裸指针，观察默认行为为什么会出错。学习顺序是：

1. 复制对象有什么用，拷贝构造与拷贝赋值分别何时发生；
2. 编译器生成的逐成员拷贝具体复制什么；
3. 独占资源类怎样用深拷贝修复拷贝构造；
4. 拷贝赋值为什么还要处理目标旧资源、自赋值和失败路径。

### 机制一：先看目标对象是否已经存在，再区分两种复制入口

**实际问题**：程序已经有一个读数对象 `original`。下面两处代码都想让另一个对象得到数值 `7`，但它们发生时目标对象的状态不同。

```cpp
class Reading {
public:
    explicit Reading(int value) : value_{value} {}

private:
    int value_;
};

Reading original{7};
Reading created{original};  // created 在这一行才开始创建

Reading existing{0};        // existing 已经创建完成
existing = original;        // 修改已有 existing 的状态
```

先不背函数名，只回答“目标在操作前是否存在”：

| 代码 | 操作前目标是否已经存在 | 操作完成后发生什么 |
|---|---|---|
| `Reading created{original};` | 否 | 新对象 `created` 被创建，初始值来自 `original` |
| `existing = original;` | 是 | `existing` 仍是同一个对象，只是状态被替换 |
| `Reading copy = original;` | 否 | `copy` 在这条声明中被创建；这里的 `=` 仍属于初始化语法 |

普通话结论：**新建目标时复制**和**修改已有目标时复制**是两件不同的事。判断依据不是“有没有等号”，而是目标对象在这条语句之前是否已经完成初始化。

**概念落点**：

- [拷贝构造函数（copy constructor）](../docs/glossary/day10.md#拷贝构造函数copy-constructor)是用同类对象初始化一个新对象的构造函数，其首个参数通常写作 `const T&`，并负责建立新对象的独立有效状态。
- [拷贝赋值运算符（copy assignment operator）](../docs/glossary/day10.md#拷贝赋值运算符copy-assignment-operator)是把一个已有同类对象的状态复制到另一个已经完成初始化的对象中的非静态成员函数，常见签名为 `T& operator=(const T&)`。

二者的共同作用都是让目标获得源对象所表达的状态；区别是一个负责**建立新对象**，另一个负责**替换已有对象的状态**。

**代码与机制**：对类 `T`，两个常见声明形式是：

```cpp
T(const T& other);             // 拷贝构造函数
T& operator=(const T& other);  // 拷贝赋值运算符
```

`Reading` 没有手写这两个函数，编译器在条件允许时会隐式声明并定义相应操作，因此上面的代码仍可使用。它们默认怎样处理每个成员，是机制二要回答的问题。

拷贝构造还可能出现在按值传参或返回等初始化场景，但 C++17 存在移动和拷贝消除规则。今天先掌握最稳定的判断：`T target{source};` 中目标正在创建，`target = source;` 中目标早已存在。

> [!NOTE] 拷贝构造
> [[notes for day10]]

**错误做法与修复**：把所有带 `=` 的写法都叫“拷贝赋值”会误判 `T copy = source;`。这段语法本身有明确定义，但术语判断错误。修复时只问一句：**目标对象在这一行之前是否已经存在？**

**小检查**：`Buffer a{3}; Buffer b{a}; Buffer c{1}; c = a;` 中，哪一行调用普通构造，哪一行属于拷贝构造，哪一行属于拷贝赋值？

### 机制二：默认逐成员拷贝对整数和拥有型指针产生不同后果

**实际问题**：现在把简单的 `Reading` 换成 `BrokenBuffer`。每个对象析构时都会 `delete[] data_`，表示它承诺负责释放 `data_` 指向的动态数组。

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

`second` 是通过机制一所说的拷贝构造创建的。类没有自定义拷贝构造函数，因此编译器生成的操作会依次复制各个成员：

| 被复制的成员 | `first` 中保存什么 | `second` 得到什么 | 复制后是否独立 |
|---|---|---|---|
| `size_` | 整数 `3` | 另一个值为 `3` 的整数成员 | 是 |
| `data_` | 数组 A 的地址 | 同一个地址值 | 两个指针成员独立，但都指向数组 A |

这里的“默认逐成员拷贝”可以先准确理解为：编译器生成的拷贝操作按成员各自的类型复制对应成员。它不是把对象全部字节盲目照搬；对于 `int*`，按指针类型复制的结果就是复制地址值，语言不会沿着地址自动创建新数组。

**概念落点**：

- [资源所有权（resource ownership）](../docs/glossary/day10.md#资源所有权resource-ownership)是对象或程序组件承担在约定时机结束所管理资源的有效使用并执行恰好一次相应释放操作的责任关系。
- [浅拷贝与深拷贝（shallow copy and deep copy）](../docs/glossary/day10.md#浅拷贝与深拷贝shallow-copy-and-deep-copy)：在本课程资源类语境中，浅拷贝只复制资源句柄或指针值，使多个对象指向同一底层资源；深拷贝则为目标建立独立资源并复制资源内容。


本例两个对象都执行 `delete[]`，等于都声称自己拥有数组 A；但同一份独占资源只能由一个所有者负责释放。`data_` 的默认复制因此形成了不符合本类承诺的浅拷贝。


**代码与机制**：离开作用域时，两个析构函数都会对数组 A 执行 `delete[]`。第一次释放后，另一个对象的 `data_` 成为悬空指针；第二次释放同一数组属于重复释放导致的**未定义行为**。程序不保证必然崩溃、报错或稳定输出。

浅拷贝并非对所有类型都错误。若类型明确采用共享所有权并正确协调最后一次释放，多个对象指向同一资源可能正是设计目标；第 18 天学习相应工具。本日 `BrokenBuffer` 承诺独占数组，所以只能选择深拷贝或禁止拷贝。

> [!NOTE] 为什么 `data_` 不能直接写成固定数组？
> 构造参数 `count` 在程序运行时才得到，而 `int data_[N]` 中的普通原生数组边界 `N` 必须满足编译期要求。工程代码通常会直接使用 `std::vector<int>` 管理运行期长度的数组；本日使用裸指针是为了观察资源类的拷贝问题。

**错误做法与修复**：故意危险文件 [`exercises/day10/broken/shallow_copy.cpp`](../exercises/day10/broken/shallow_copy.cpp) 可能通过编译，但禁止运行。分类：**双重释放/未定义行为**。修复可以是实现深拷贝、用 `= delete` 禁止复制，或改用能正确表达所有权的标准库成员。

**小检查**：默认复制 `int* data_` 时，究竟复制了哪个对象保存的什么值？为什么这不等于复制动态数组？

### 机制三：为独占资源类编写拷贝构造，让新对象拥有独立数组

**实际问题**：我们希望 `SampleBuffer copied{original};` 满足三点：初始元素相同、修改互不影响、两个对象都能独立析构。

```cpp
SampleBuffer original{3};       // 拥有数组 A
original.set(0, 10);

SampleBuffer copied{original};  // 应拥有数组 B
copied.set(0, 99);
```

正确拷贝后的状态是：

| 比较项 | `original` | `copied` |
|---|---|---|
| 第 0 个元素 | `10` | 初始也是 `10`，随后改为 `99` |
| `data_` 指向 | 数组 A | 独立的数组 B |
| 析构时释放 | A | B |

**概念落点**：这里仍使用机制一已经定义的[拷贝构造函数](../docs/glossary/day10.md#拷贝构造函数copy-constructor)，但根据本类“副本互不影响”的要求，选择机制二定义的[深拷贝](../docs/glossary/day10.md#浅拷贝与深拷贝shallow-copy-and-deep-copy)。

**代码与机制**：实现时按“先确定长度 → 为新对象申请数组 → 复制元素”阅读：

```cpp
SampleBuffer(const SampleBuffer& other)
    : size_{other.size_},
      data_{size_ > 0 ? new int[size_]{} : nullptr} {
    if (size_ > 0) {
        std::copy(other.data_, other.data_ + size_, data_);
    }
}
```

1. `other` 是源对象的 `const` 引用；函数不应修改源，也不能为了传参再复制一个 `SampleBuffer`。
2. `size_` 保存源长度。
3. `data_` 为新对象申请自己的数组 B。
4. `std::copy` 复制元素内容，而不是复制 `other.data_` 保存的地址。

若 `new` 失败，构造不会正常完成，`original` 仍未改变。本日先记住：新对象只有在自己的资源准备成功后才成为独立所有者；完整的构造失败与异常安全在第 19 天展开。

**错误做法与修复**：

```cpp
SampleBuffer(const SampleBuffer& other)
    : size_{other.size_}, data_{other.data_} {}
```

分类：代码可能编译，但它仍让两个独占对象指向同一数组，最终有重复释放的**未定义行为风险**。修复是独立分配并复制内容，或明确删除拷贝构造函数。

**小检查**：深拷贝完成后，源对象与新对象的元素值最初应有什么关系？两个 `data_` 保存的地址又应有什么关系？

### 机制四：拷贝赋值要替换旧状态，并把三条资源路径一起审视

**实际问题**：`target = source;` 执行前，`target` 已经拥有数组 B。赋值既要复制 `source` 的内容，又不能泄漏 B；调用代码还可能通过别名形成 `target = target;`。

```cpp
SampleBuffer source{3};  // 拥有数组 A
SampleBuffer target{2};  // 已经拥有数组 B
target = source;
```

按状态变化理解正确顺序：

| 时刻 | `source` 管理 | `target` 管理 |
|---|---|---|
| 赋值前 | 源数组 A | 旧数组 B |
| 准备新状态 | A 保持不变 | 先建立替代数组 C |
| 提交新状态 | 仍管理 A | 释放 B，改为管理 C |

普通话结论：拷贝构造面对“新对象还没有旧资源”；拷贝赋值面对“目标已经存在，必须安全结束原来的所有权”。

**概念落点**：[Rule of Three（三法则）](../docs/glossary/day10.md#rule-of-three三法则)是 C++11 之前形成、在现代 C++ 中仍用于理解资源类的设计规则：若类需要自定义析构函数、拷贝构造函数或拷贝赋值运算符中的一个，通常应一起审视并正确设计三者。

它不是编译器强制的语法，而是一条设计检查：对象结束、用源创建新对象、把源赋给已有对象这三条路径都必须遵守同一份所有权承诺。

**代码与机制**：

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

1. `this == &other` 检测目标和源是否其实是同一对象。
2. 先准备 `replacement`；若分配失败，目标的旧数组 B 尚未改变。
3. 本例元素是 `int`，复制完成后才 `delete[] data_`，结束目标对 B 的所有权。
4. 目标接管数组 C、更新大小，并返回 `*this`。

本实现清楚展示“先准备、后提交”。对复制时可能抛出异常的任意元素类型，还需要自动清理临时资源；第 19 天再系统学习。工程代码通常让 `std::vector` 等成员管理资源，从而避免手写三条路径。

**错误做法与修复**：

```cpp
SampleBuffer& operator=(const SampleBuffer& other) {
    delete[] data_;
    data_ = new int[other.size_];
    // 若 this == &other，other.data_ 也已经指向被释放的数组。
    return *this;
}
```

自赋值时再从 `other` 读取会形成释放后使用，属于**未定义行为**；若 `new` 失败，目标旧状态也已经丢失。修复是证明实现天然支持自赋值，或像本例一样显式检查，并采用先准备替代资源、成功后再提交的顺序。

**小检查**：为什么 `target = target;` 可能经由引用或指针别名出现？本例正确实现会不会改变目标状态？

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

