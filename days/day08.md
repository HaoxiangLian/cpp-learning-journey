# 第 8 天：类如何维护不变量

预计用时：110—120 分钟。标准主线：C++17。

## 🎯 今日攻坚目标

完成本日后，你应能按正文顺序：

1. 区分类、对象和成员，并说明不同对象为什么各自保存非静态成员状态；
2. 用访问控制和公开成员函数维护一个明确的类不变量；
3. 说明非静态成员函数中的 `this` 指向哪个对象，以及尾置 `const` 限制什么。

## 🔁 前置知识检查

先独立回答，再回看第 2、5—7 天：

1. 类型决定对象的哪些基本规则？对象与变量有什么区别？
2. `static` 数据成员为什么不属于任何一个对象的普通成员子对象？
3. 如果类直接保存一个裸动态数组，谁必须负责释放？为什么今天先从不拥有动态资源的小类开始？

参考方向：类型规定可表示状态和允许操作；变量是具名对象或具名引用；静态成员属于类而非每个对象；手动资源所有权需要拷贝、析构等配套规则，将在第 9—11 天展开。

## 📖 核心知识重构

前 7 天主要使用语言内建类型和标准库类型。从今天开始，我们设计自己的类型。重点不是背 `class` 语法，而是让“合法状态只能通过合法操作产生”成为类型本身的性质。

### 机制一：类规定共同规则，对象各自保存成员状态

**实际问题**：机器人关节角必须位于 `[-180, 180]`。如果程序到处传递一个裸 `double`，调用者看不出单位和范围，也能随手写入 `500.0`。我们需要一种新类型，把状态的含义和允许的操作放在一起。

```cpp
class JointAngle {
public:
    double degrees() const {
        return degrees_;
    }

private:
    double degrees_{0.0};
};

JointAngle left_joint;
JointAngle right_joint;
```

先只看数量：`JointAngle` 的规则写一遍；程序随后创建了 `left_joint`、`right_joint` 两个对象，所以每个对象各有一个自己的 `degrees_`。改变左关节的角度，不会自动改变右关节。

| 代码中的名字 | 先怎样理解 |
|---|---|
| `JointAngle` | 一种自己设计的类型和共同规则 |
| `left_joint`、`right_joint` | 按这套规则创建的两个具体对象 |
| `degrees_` | 每个对象各自保存的角度状态 |
| `degrees()` | 对象提供的查询操作 |

**概念落点**：

- [类（class）](../docs/glossary/day08.md#类class)是用户定义的类型，它在一个类定义中规定该类型对象所包含的非静态数据成员、可执行的成员函数以及相关访问规则。
- [成员（member）](../docs/glossary/day08.md#成员member)是在类定义中声明并属于该类的实体；本日重点是保存每个对象状态的非静态数据成员，以及通过对象操作状态的非静态成员函数。

直观地说，类像一份统一规则，对象是按这份规则创建的具体实体。这个比喻只帮助入门：专业上，`JointAngle` 是类型，`left_joint` 和 `right_joint` 是两个独立对象；每个对象各有自己的非静态数据成员。

**代码与机制**：`degrees_` 是每个 `JointAngle` 对象内部的成员子对象，两个关节对象的角度状态彼此独立。`degrees()` 是成员函数，通过某个对象调用：

```cpp
std::cout << left_joint.degrees();
```

类定义规定完整成员集合；不能在另一个文件中临时给这个类“追加一个数据成员”。成员函数可以在类内定义，也可以先在类内声明、再在类外定义，但后者仍是原成员的定义，不是追加成员。

C++17 保证每个类对象都有与其直接非静态数据成员对应的成员子对象，但对象的总大小还可能受对齐、填充等影响。今天不能把源代码中的成员列表直接画成无间隙的字节布局，更不能臆测隐藏字段；对象模型细节在第 13 天集中处理。

**错误做法与修复**：

```cpp
double left_joint_angle{0.0};
double right_joint_angle{0.0};
// 任意位置都可能直接写入 900.0，类型没有表达范围约束。
```

分类：这是**有明确定义但设计不能阻止无效状态**的问题，并非编译错误或未定义行为。修复方向是引入表达领域含义的小类，把修改收口到成员函数，而不是只依赖调用者记住注释。

**小检查**：`JointAngle first; JointAngle second;` 中，类定义有几份？`degrees_` 成员子对象有几份？
	一份；两份；
### 机制二：封装让公开操作维护类不变量，private 只是实现手段

**实际问题**：仅仅把角度和函数放进同一个 `struct`，如果类外仍能直接写数据，就可以绕过检查。真正目标不是“隐藏”，而是让每次公开操作结束后，对象仍处于可用状态。

先看一次修改怎样被拦住：

```cpp
bool set(double candidate) {
    if (candidate < -180.0 || candidate > 180.0) {
        return false;                 // 拒绝，原角度不变
    }
    degrees_ = candidate;             // 合法才提交
    return true;
}
```

输入 `45.0` 时对象更新并返回 `true`；输入 `500.0` 时直接返回 `false`，成员保持原值。这里真正要维护的规则是“公开操作完成后，角度仍在范围内”。

**概念落点**：

- [封装与类不变量（encapsulation and class invariant）](../docs/glossary/day08.md#封装与类不变量encapsulation-and-class-invariant)：封装是把状态及维护该状态的操作组织在类型边界内并限制不受控访问的设计；类不变量是每个对外可观察的有效对象在其公开操作完成前后都应满足的条件。
- [成员访问控制（member access control）](../docs/glossary/day08.md#成员访问控制member-access-control)是通过 `public`、`protected` 和 `private` 规定某个成员声明能够在哪些程序上下文中被命名的语言规则。

本例不变量是：`degrees_` 始终在 `[-180, 180]`。公开修改函数先检查候选值，只有合法时才提交状态。

**代码与机制**：`public` 成员可由普通类外代码命名；`private` 成员只能在该类成员及友元等允许上下文中命名。`class` 默认访问级别是 `private`，`struct` 默认是 `public`；除此以外二者都能拥有数据成员、成员函数和访问说明符，因此不能把 `struct` 简化为“只能放数据”。

访问控制在程序合法性检查中生效：

```cpp
JointAngle joint;
joint.set(45.0);       // 合法：公开接口
// joint.degrees_ = 500.0;  // 编译错误：类外不能命名 private 成员
```

`private` 不会自动验证不变量。若公开函数不检查便写入 `degrees_`，设计仍然失败。反过来，简单的无不变量数据记录可以使用公开成员；是否封装取决于类型是否需要维护关系和约束，而不是机械追求 getter/setter 数量。

标准只规定访问规则和类语义；它不把 `private` 变成运行时沙箱、加密或物理隔离。调试器可能显示私有成员，错误的越界指针仍可能破坏任意存储——那属于别的问题。

**错误做法与修复**：

```cpp
class JointAngle {
public:
    void set(double candidate) { degrees_ = candidate; }  // 没有维护范围
private:
    double degrees_{0.0};
};
```

分类：代码**有明确定义但结果可能不符合类型承诺**。修复是明确不变量，在每个能改变状态的公开操作中先验证，再修改；查询函数则只暴露调用者真正需要的信息。

**小检查**：把 `degrees_` 设为 `private` 后，为什么仍不能自动证明 `JointAngle` 永远有效？

### 机制三：非静态成员函数通过 this 作用于具体对象，尾置 const 限制普通修改

**实际问题**：同一个 `set` 函数能分别修改左关节和右关节。函数体中的 `degrees_` 到底属于谁？查询函数为什么应该能被 `const JointAngle` 调用？

```cpp
bool set(double candidate) {
    this->degrees_ = candidate;
    return true;
}

double degrees() const {
    return this->degrees_;
}
```

把调用翻译成普通话：`left_joint.set(30.0)` 是“让这个函数操作 `left_joint`”，`right_joint.set(-20.0)` 是“让同一段函数代码操作 `right_joint`”。函数内部用 `this` 表示本次究竟是哪个对象；通常可以省略 `this->`。

**概念落点**：[this 指针与 const 成员函数（this pointer and const member function）](../docs/glossary/day08.md#this-指针与-const-成员函数this-pointer-and-const-member-function)：在非静态成员函数中，`this` 是指向该次调用所作用对象的指针；函数末尾的 `const` 表示通过这次调用不能修改该对象的非 `mutable` 数据成员。


**代码与机制**：调用 `left_joint.set(30.0)` 时，函数作用于 `left_joint`；调用 `right_joint.set(-20.0)` 时，作用于 `right_joint`。在普通成员访问中通常省略 `this->`，因此 `degrees_` 与 `this->degrees_` 在这里表示同一成员。

对 `const` 对象只能调用不会通过该调用普通修改对象的成员函数：

```cpp
const JointAngle parked_joint;
std::cout << parked_joint.degrees();  // degrees() 是 const 成员函数
// parked_joint.set(10.0);            // 编译错误：set 不是 const
```

==尾置 `const` 是成员函数类型语义的一部分==，不是返回值的 `const`。它不会保证“函数无任何副作用”：函数仍可能输出日志、修改外部对象或调用其他允许的操作；线程安全也需要独立设计。


> [!NOTE] `const` 成员函数
> [[notes for day08_2]]


静态成员函数没有对应的调用对象，因此没有 `this` 指针。类的静态成员在第 5 天已讲过，这里要避免把“属于类”误说成“暗中选择了某个对象”。


> [!NOTE] 非静态成员函数与静态成员函数
>  [[notes for day08]]

**错误做法与修复**：

```cpp
double degrees() {  // 忘记尾置 const
    return degrees_;
}
```

如果只由非 `const` 对象调用，它有明确定义；但 `const JointAngle` 无法调用该查询接口，属于**接口设计导致的编译错误**。修复是在确实不需普通修改对象状态的查询函数末尾加 `const`，而不是随意去掉调用方的 `const`。

**小检查**：`const` 成员函数能否向 `std::cout` 输出？能否直接执行 `degrees_ = 10.0`？为什么两个答案不同？
	可以；不可以；有没有改变类的成员子对象；

## 💻 最小可运行示例

完整代码位于 [`examples/day08/main.cpp`](../examples/day08/main.cpp)。

```cpp
#include <iostream>

class JointAngle {
public:
    bool set(double candidate) {
        if (candidate < -180.0 || candidate > 180.0) {
            return false;
        }
        this->degrees_ = candidate;
        return true;
    }

    double degrees() const {
        return this->degrees_;
    }

    bool within_limit() const {
        return degrees_ >= -180.0 && degrees_ <= 180.0;
    }

private:
    double degrees_{0.0};
};

int main() {
    JointAngle joint;
    const bool accepted{joint.set(45.0)};
    const bool rejected{!joint.set(500.0)};

    std::cout << std::boolalpha;
    std::cout << "accepted: " << accepted << '\n';
    std::cout << "rejected invalid: " << rejected << '\n';
    std::cout << "angle: " << joint.degrees() << '\n';
    std::cout << "invariant holds: " << joint.within_limit() << '\n';
}
```

预期输出：

```text
accepted: true
rejected invalid: true
angle: 45
invariant holds: true
```

观察重点：第二次 `set` 拒绝非法值，没有把对象改成无效状态；查询函数均为 `const` 成员函数。

## 🐛 错误代码诊断

[`exercises/day08/broken/private_access.cpp`](../exercises/day08/broken/private_access.cpp) 被显著标记为故意错误，不进入正常构建目标。

```cpp
JointAngle joint;
joint.degrees_ = 500.0;
```

分类：**编译错误**。`degrees_` 是 `private` 成员，普通类外代码不能命名它。修复不是简单改为 `public`，而是调用能验证候选值的公开成员函数。

另一个常见反例是公开 `set` 不做校验：它可以编译并有明确定义，却允许对象违反类不变量，属于逻辑/设计错误，而不是 UB。

## 🎤 高频面试实战

本节保留当日主问题的答题框架。先独立口述，再到[第 8 天面试答案](../interview/answers/day08.md)逐题核对全部追问。

### 题 1：类与对象是什么关系？封装为什么不只是把成员设成 private？

**30 秒口头回答**：==类是用户定义类型，规定成员和访问规则；==对象是该类型在程序执行中的具体实体，每个对象各自拥有非静态数据成员。封装的目标是把状态与维护状态的操作放进类型边界，并通过公开接口维持类不变量。`private` 只是限制类外直接命名成员的语言工具；如果公开函数仍能写入非法状态，就没有真正维护好封装。

**完整答题逻辑**：先按“类型与实体”区分类和对象；再区分数据成员与成员函数；随后给出不变量和合法操作；最后说明访问控制是编译期合法性规则，不是加密、运行时沙箱或某种固定内存布局。

**可能连续追问**：

- 一个类的两个对象是否共享非静态数据成员？
- `class` 和 `struct` 在成员访问上的默认区别是什么？
- `private` 是否意味着数据在内存中不可被查看？
- getter 和 setter 越多，封装就一定越好吗？
- 什么是类不变量？应该在哪些时刻成立？

**容易失分的说法**：把类说成“对象模板”后不再给专业定义；认为 `struct` 不能有函数；把 `private` 当成数据安全机制；只列 getter/setter 而不说明不变量。

### 题 2：this 指针是什么？const 成员函数限制了什么？

**30 秒口头回答**：非静态成员函数通过 `this` 指针知道本次调用作用于哪个对象，未限定的数据成员访问可理解为经当前对象访问。成员函数末尾的 `const` 限制通过本次调用修改对象的非 `mutable` 数据成员，并使函数可由合适的 `const` 对象调用。它不代表函数没有外部副作用，也不自动保证线程安全；静态成员函数没有 `this`。

**完整答题逻辑**：先从两个对象调用同一成员函数解释当前对象；再说明 `this` 的作用和省略写法；随后区分尾置 `const`、返回类型 `const` 和对象本身的 `const`；最后给出静态成员函数及外部副作用边界。

**可能连续追问**：

- 成员函数代码是否在每个对象中各保存一份？
- 可以给 this 指针重新赋值，让成员函数改为操作另一个对象吗？
- 为什么 const 对象不能调用普通非 const 成员函数？
- const 成员函数能否修改外部全局对象或输出日志？
- 静态成员函数为什么没有 this？

**容易失分的说法**：把 `this` 当成对象内部隐藏数据成员；说 `const` 成员函数什么都不能修改；断言成员函数必须占据每个对象的大小；认为静态函数暗中接收某个对象。

## ✍️ 当日练习

完成 [`exercises/day08/README.md`](../exercises/day08/README.md)：

- 5 道概念判断；
- 3 组访问控制、对象独立状态和 `const` 调用分析；
- 一道维护 `[0, 100]` 电量不变量的小类编程题。

先用一句话写出类不变量，再决定哪些成员必须公开。完成后查看 [`solutions/day08/README.md`](../solutions/day08/README.md)。

## 🧠 深度思考题

一个机器人任务包含“任务编号、最大重试次数、当前重试次数和是否完成”。请先写出至少两个类不变量，再设计最小公开接口。哪些字段不应直接公开？哪个操作可能失败，应该怎样向调用方报告？不要写代码，先证明接口无法轻易制造矛盾状态。

## ✅ 完成标准

- 能准确区分类、对象、非静态数据成员和成员函数。
- 能用公开操作和私有状态维护一个明确的类不变量。
- 能说明 `public/private` 决定可命名性，不是运行时加密或内存隔离。
- 能解释非静态成员函数中的 `this` 指向本次调用对象。
- 能为只读查询添加尾置 `const`，并说明其限制与不限制的内容。

规则依据：C++ 公开工作草案的 [class members](https://eel.is/c++draft/class.mem.general)、[member access control](https://eel.is/c++draft/class.access) 与 [`this`](https://eel.is/c++draft/expr.prim.this) 条款；公开草案不是已发布 ISO 标准本身。
