# 第 9 天：对象怎样出生和结束

预计用时：110—120 分钟。标准主线：C++17。

## 🎯 今日攻坚目标

能够用构造函数和成员初始化列表建立有效对象，准确预测成员的初始化/析构顺序，并使用 `explicit` 阻止不符合接口意图的隐式转换。

## 🔁 前置知识检查

1. 类不变量通常应在哪些对外可观察时刻成立？
2. `private` 数据成员能否由同一个类的成员函数访问？
3. 局部自动存储期对象离开其块时，对象生命周期通常发生什么变化？

参考方向：有效对象在公开操作完成前后应满足不变量；类成员可访问该类私有成员；局部对象离开对应范围时会按语言规则销毁，其析构过程与动态对象的 `delete` 路径要分开分析。

## 📖 核心知识重构

第 8 天的类依靠默认成员初始化器获得初始状态。真实工程还需要携带任务编号、配置值或资源句柄创建对象。构造函数负责把输入转成有效初始状态，析构函数负责对象结束时的类级清理；两者共同围住对象可用的生命周期区间。


> [!NOTE] 构造函数与析构函数的定义与作用
> [[notes for day09_3]]


### 机制一：构造函数在函数体运行前完成成员初始化

**实际问题**：`RobotTask` 创建后必须立即拥有任务编号，不能先产生“没有编号的半成品”再等待调用者补写。如何保证对象一完成初始化就满足不变量？

```cpp
class RobotTask {
public:
    explicit RobotTask(int id)
        : id_{id}, active_{true} {
        // 进入这里时，id_ 和 active_ 已经完成初始化。
    }

private:
    int id_;
    bool active_;
};
```

创建 `RobotTask task{7};` 时，先看三个时刻：


> [!NOTE] 为什么Robot robot{"机械臂"};就自动调用了构造函数
> [[notes for day09_2]]


| 时刻 | `id_` | `active_` |
|---|---:|---:|
| 进入构造函数体之前 | 已用 `7` 初始化 | 已用 `true` 初始化 |
| 执行构造函数体时 | 已经可用 | 已经可用 |
| 构造完成后 | 对象对外可用 | 对象对外可用 |

冒号后的 `id_{id}, active_{true}` 不是“先创建空成员，再在函数体里赋值”，而是在成员建立时直接给出初始状态。

**概念落点**：

- [构造函数（constructor）](../docs/glossary/day09.md#构造函数constructor)是类的特殊成员函数，在类类型对象初始化时被选择调用，以建立该对象及其成员的初始状态。
- [成员初始化列表（member initializer list）](../docs/glossary/day09.md#成员初始化列表member-initializer-list)是构造函数声明体之前、冒号之后的一组初始化器，用来直接指定基类和非静态数据成员的初始化参数。

**代码与机制**：构造函数名称写成类名，没有返回类型。创建 `RobotTask task{7};` 时，语言先为完整对象及其子对象提供存储，再按初始化规则建立成员，最后进入构造函数体。函数体适合执行需要成员已经有效的检查或日志，不是 `id_` 第一次存在的地方。

对比下面两种写法：

```cpp
RobotTask(int id) : id_{id} {}  // 直接初始化 id_

RobotTask(int id) {             // id_ 先按其他规则初始化
    id_ = id;                   // 随后才赋值
}
```

对 `int` 而言，两者在某些例子中最终值可能相同，但语义不同；对 `const` 成员、引用成员或没有默认构造/赋值路径的类成员，函数体赋值甚至不可行。工程上应在初始化列表中直接建立成员初始状态。

如果类内给成员写了默认成员初始化器，而某个构造函数又在列表中显式初始化该成员，则该次构造使用列表里的初始化器。不要把两者理解成“先默认初始化一次，再覆盖一次”。


> [!NOTE] 什么是”初始化器“
> [[notes for day09]]


**错误做法与修复**：

```cpp
class Task {
public:
    Task(int id) { id_ = id; }
private:
    const int id_;  // 编译错误：进入函数体前必须已经初始化
};
```

分类：**编译错误**。`const` 成员不能先处于待赋值状态。修复为 `Task(int id) : id_{id} {}`。即使成员不是 `const`，优先直接初始化也能更准确表达意图。

**小检查**：执行构造函数体第一条语句时，列表中列出的成员是否已经完成初始化？
	是的
### 机制二：成员按声明顺序初始化，不按初始化列表顺序

**实际问题**：一个成员的初始值依赖另一个成员。如果只看初始化列表的排列，代码审查者可能以为依赖已经满足，实际却读取了尚未初始化的成员。

危险示例：

```cpp
class ScaledReading {
public:
    explicit ScaledReading(int raw)
        : raw_{raw}, doubled_{raw_ * 2} {}

private:
    int doubled_; // 实际先初始化，却读取尚未初始化的 raw_
    int raw_;
};
```

不要先看冒号后的排列，先看类中成员从上到下的声明：`doubled_` 写在前，所以它先初始化；但它的表达式读取了此时尚未初始化的 `raw_`。普通话规则就是：**成员排队看类里的声明顺序，不看构造函数列表写的顺序。**

**概念落点**：[初始化与析构顺序（initialization and destruction order）](../docs/glossary/day09.md#初始化与析构顺序initialization-and-destruction-order)是类子对象按语言固定次序建立并按相反次序销毁的规则；非静态数据成员按它们在类定义中的声明顺序初始化，并按逆序析构。

**代码与机制**：虽然列表先写 `raw_`，真实次序仍是 `doubled_`、`raw_`，因为它们在类定义中如此声明。`doubled_` 的初始化表达式读取尚未初始化的 `raw_`，在 C++17 中会触发未定义行为；编译器可能警告，但诊断不是该语义规则的替代品。

正确做法让声明顺序体现依赖：

```cpp
private:
    int raw_;
    int doubled_;

public:
    explicit ScaledReading(int raw)
        : raw_{raw}, doubled_{raw_ * 2} {}
```

==构造一个完整类对象时，先处理基类子对象，再按声明顺序处理成员，最后执行构造函数体。==第 12 天学习继承时再展开基类细节；今天只需牢牢记住“成员看声明，不看列表排列”。

**错误做法与修复**：仅交换初始化列表项目、却不交换成员声明，代码仍然错误。分类是**未定义行为风险**，不保证产生固定数值或崩溃。修复是按依赖顺序声明成员，同时让初始化列表保持相同顺序以消除误导和编译器重排警告。

**小检查**：若 `first_` 在类中先声明、初始化列表却先写 `second_`，哪个成员先初始化？
	second_
### 机制三：析构函数执行类级结束操作，成员随后按构造逆序销毁

**实际问题**：一个任务对象离开作用域时，内部传感器会话和规划器会话应按可靠顺序关闭。如何理解析构函数体与成员析构之间的先后？

```cpp
class RobotTask {
public:
    ~RobotTask() {
        std::cout << "task done\n";
    }
private:
    Trace sensor_;
    Trace planner_;
};
```

假设 `sensor_` 先构造、`planner_` 后构造。对象结束时先执行 `RobotTask` 的析构函数体，然后成员按相反顺序结束：先 `planner_`，再 `sensor_`。可以先记成“后建立的成员先收尾”。

**概念落点**：[析构函数（destructor）](../docs/glossary/day09.md#析构函数destructor)是类的特殊成员函数，在类对象销毁过程中被调用，用于执行该类层级的结束操作，并==由语言继续销毁成员和基类子对象==。

**代码与机制**：对象销毁时先执行 `RobotTask` 析构函数体，然后按声明的逆序销毁成员：`planner_` 先，`sensor_` 后。逆序规则让后构造、可能依赖先构造成员的对象先结束。

析构函数与存储释放不是同一句话：局部对象的析构完成后，其自动存储按相应规则结束使用；`delete pointer` 对动态对象会触发析构并释放对应动态存储。只调用 `pointer->~T()` 属于显式析构语法，不等于普通代码已经完成匹配的 `delete`，错误使用还可能导致重复析构；本课程暂不把它作为常规资源管理手段。

析构函数为后续“对象自动清理所拥有资源”的思想提供基础，但今天的示例只打印可观察顺序。异常安全和 RAII 在第 19 天系统学习，不能提前把一句“写析构函数”当成完整异常安全方案。

**错误做法与修复**：

```cpp
{
    RobotTask task{7};
}
// 这里再使用 task：编译错误，名称已离开作用域；对象也已经销毁。
```

若保存指向该局部对象的指针并在块外解引用，则会形成悬空访问，属于**未定义行为**，不保证必然崩溃。修复是让观察者生命周期不超过对象，或采用清晰的所有权设计。

**小检查**：成员 `sensor_` 先声明、`planner_` 后声明；二者的构造和析构顺序分别是什么？

### 机制四：explicit 让单参数构造必须表达类型转换意图

**实际问题**：函数要求 `RetryLimit`，调用者却写 `schedule(3)`。编译器若自动把 `3` 构造成重试策略，单位或语义错误会被隐藏在一次隐式转换中。

```cpp
class RetryLimit {
public:
    explicit RetryLimit(int count) : count_{count} {}
private:
    int count_;
};

RetryLimit limit{3};       // 直接初始化：合法
// RetryLimit other = 3;   // 复制初始化：不能使用 explicit 构造函数
```

先看调用意图是否写出来：`RetryLimit{3}` 明确说“把整数 3 变成重试次数对象”，而 `schedule(3)` 只出现一个裸整数。`explicit` 要求调用者把前一种类型形成动作写出来，避免单位和含义被悄悄猜测。

**概念落点**：[explicit 构造函数（explicit constructor）](../docs/glossary/day09.md#explicit-构造函数explicit-constructor)是不能作为普通隐式用户定义转换被自动采用、但仍可通过直接初始化或显式转换调用的构造函数。

**代码与机制**：`explicit` 不改变构造函数体，也不验证 `count` 是否非负；范围不变量仍需构造函数自己维护。它解决的是调用表达式是否可以悄悄插入用户定义转换。

高频规则是：能够用一个实参调用、且不应承担隐式转换语义的构造函数通常声明为 `explicit`。C++11 起列表初始化也参与相关重载规则；C++20 还支持条件 `explicit(...)`，本课程只作识别，不要求今天掌握。

**错误做法与修复**：

```cpp
void schedule(RetryLimit limit);
schedule(3);  // explicit 后为编译错误
```

分类：**编译错误**。修复为 `schedule(RetryLimit{3});`，让类型形成动作在调用点可见；不要为了省几个字符去掉有意义的 `explicit`。

**小检查**：`explicit RetryLimit(int)` 是否允许 `RetryLimit{3}`？是否允许 `RetryLimit value = 3;`？
	允许；不允许；因为explicit不支持隐式类型转换；
## 💻 最小可运行示例

完整代码位于 [`examples/day09/main.cpp`](../examples/day09/main.cpp)。

```cpp
#include <iostream>
#include <string>

class Trace {
public:
    explicit Trace(const char* label) : label_{label} {
        std::cout << "construct " << label_ << '\n';
    }

    ~Trace() {
        std::cout << "destroy " << label_ << '\n';
    }

private:
    std::string label_;
};

class RobotTask {
public:
    explicit RobotTask(int id)
        : sensor_{"sensor"}, planner_{"planner"}, id_{id} {
        std::cout << "task " << id_ << " ready\n";
    }

    ~RobotTask() {
        std::cout << "task " << id_ << " done\n";
    }

    void run() const {
        std::cout << "task " << id_ << " running\n";
    }

private:
    Trace sensor_;
    Trace planner_;
    int id_;
};

int main() {
    RobotTask task{7};
    task.run();
}
```

预期输出：

```text
construct sensor
construct planner
task 7 ready
task 7 running
task 7 done
destroy planner
destroy sensor
```

## 🐛 错误代码诊断

### 反例 A：依赖顺序与声明顺序相反

前文 `ScaledReading` 在初始化 `doubled_` 时读取尚未初始化的 `raw_`。分类：**未定义行为**。仅交换列表顺序不能修复，必须调整成员声明与依赖设计。

### 反例 B：explicit 阻止隐式转换

[`exercises/day09/broken/implicit_conversion.cpp`](../exercises/day09/broken/implicit_conversion.cpp) 被显著标记为故意错误，不进入正常构建目标。`schedule(3)` 无法使用 `explicit RetryLimit(int)` 自动形成对象，分类为**编译错误**。

## 🎤 高频面试实战

先独立口述，再到[第 9 天面试答案](../interview/answers/day09.md)核对全部追问。

### 题 1：构造函数与成员初始化列表分别做什么？为什么不建议在构造函数体内给成员赋初值？

**30 秒口头回答**：构造函数在对象初始化时被选择调用，成员初始化列表直接为基类和非静态数据成员提供初始化器；成员在进入构造函数体前已经按规则完成初始化。在函数体里写 `member = value` 通常是先初始化成员再赋值，语义不同，对 `const`、引用或不可默认构造后赋值的成员还可能不合法。因此应优先在初始化列表中建立初始状态。

**完整答题逻辑**：先说明构造函数没有返回类型；再按“存储—子对象初始化—函数体”描述时序；随后比较直接初始化与赋值；最后补充默认成员初始化器和列表覆盖关系。

**可能连续追问**：

- 构造函数有返回类型吗？可以显式 return 一个对象吗？
- 进入构造函数体时，数据成员是否已经完成初始化？
- 默认成员初始化器与构造函数初始化列表同时出现时怎样选择？
- 为什么 const 成员和引用成员通常必须出现在初始化列表中？
- 构造函数是否能保证参数自动满足类不变量？

**容易失分的说法**：把初始化列表叫赋值列表；认为函数体开始时成员尚不存在；说构造函数返回当前对象；把参数校验能力归因于语法自动完成。

### 题 2：成员按什么顺序初始化和析构？explicit 解决什么问题？

**30 秒口头回答**：非静态数据成员按类定义中的声明顺序初始化，而不是初始化列表的书写顺序；销毁时按逆序进行。因此成员依赖必须与声明顺序一致。`explicit` 阻止构造函数被普通隐式用户定义转换自动采用，但仍允许直接初始化和显式转换，用于让类型转换意图在调用点可见。

**完整答题逻辑**：先给出声明顺序规则和逆序析构原因；再分析读取后声明成员的危险；随后用 `RetryLimit` 对比直接初始化与复制初始化；最后说明 `explicit` 不负责值域验证，也不表示固定运行时成本。

**可能连续追问**：

- 调整成员初始化列表的顺序能改变真实初始化顺序吗？
- 一个成员的初始化表达式读取后声明成员会怎样？
- 析构函数体与成员析构谁先执行？
- explicit 构造函数还能用 Type{argument} 调用吗？
- 是否所有单参数构造函数都必须 explicit？

**容易失分的说法**：说列表从左到右决定构造顺序；断言未初始化读取只会得到零；认为 `explicit` 禁止所有构造；把设计建议说成标准强制。

## ✍️ 当日练习

完成 [`exercises/day09/README.md`](../exercises/day09/README.md)：5 道判断、3 组顺序/初始化分析，以及一个能观察构造和析构过程的 `TaskSession` 小类。完成后再查看 [`solutions/day09/README.md`](../solutions/day09/README.md)。

## 🧠 深度思考题

设计 `TrajectorySegment`：对象必须同时拥有正数编号、起点和终点，并保证最大速度大于零。哪些条件应在构造阶段验证？如果某个输入非法，是拒绝构造、修正为默认值还是创建“无效对象”？比较三种策略对调用方和类不变量的影响，不必提前使用异常语法。

## ✅ 完成标准

- 能说明构造函数体执行前成员已经完成初始化。
- 能用成员初始化列表直接建立 `const`、引用或类类型成员状态。
- 能仅根据类内声明顺序预测成员构造与逆序析构。
- 能识别成员依赖顺序导致的未初始化读取风险。
- 能用 `explicit` 区分直接初始化与不希望发生的隐式转换。

规则依据：C++ 公开工作草案的 [initialization](https://eel.is/c++draft/class.base.init)、[destructor](https://eel.is/c++draft/class.dtor) 与 [explicit initialization](https://eel.is/c++draft/class.expl.init) 条款；公开草案不是已发布 ISO 标准本身。
