# 第 11 天：转移资源而不是复制

预计用时：120 分钟。标准主线：C++17；移动语义、右值引用与 `std::move` 来自 C++11。

## 🎯 今日攻坚目标

能够用对象所有权变化解释移动构造和移动赋值，准确说明 `std::move` 只表达“允许尝试移动”而不亲自搬运资源，并在 Rule of Five 与 Rule of Zero 之间做出合理选择。

## 🔁 前置知识检查

1. `Buffer copied{source};` 与 `copied = source;` 分别面对新对象还是已有对象？
2. 一个类用裸指针独占动态数组时，为什么默认逐成员拷贝会有重复释放风险？
3. 对独占资源类，析构函数、拷贝构造和拷贝赋值共同组成哪条设计提醒？

参考方向：第一种是新对象初始化，第二种是已有对象赋值；复制拥有型指针不会复制数组；三条路径对应 Day 10 的 Rule of Three。今天在这三条路径上加入“转移”。

## 📖 核心知识重构

Day 10 的深拷贝会新建数组并复制所有元素。可有些资源本来就不应复制，例如独占设备会话；有些大数组虽然能复制，但调用方已经不再需要源对象内容。这时可以把同一份资源的管理责任交给目标，而不是再造一份。

### 机制一：移动构造让新对象接管源资源

**实际问题**：下面的 `source` 独占数组 A。我们要创建 `target`，并且之后不再需要 `source` 保存原数据。若深拷贝，会申请数组 B 并复制元素；移动可以让 `target` 直接接管数组 A。

```cpp
MoveOnlyBuffer source{3};
source.set(1, 20);
MoveOnlyBuffer target{std::move(source)};
```

先只看这次操作前后的所有权，不急着背名词：

| 时刻 | `source` | `target` |
|---|---|---|
| 操作前 | 拥有数组 A，大小为 3 | 尚未创建 |
| 操作后 | 本类约定为空：`nullptr`、大小 0 | 拥有原数组 A，大小为 3 |

普通话结论：没有复制三个元素，也没有出现数组 B；只是把“以后由谁 `delete[]` 数组 A”的责任交给了新对象。

**概念落点**：这段代码需要三个相邻概念。

- [右值引用（rvalue reference）](../docs/glossary/day11.md#右值引用rvalue-reference)是以 `T&&` 表示、能够绑定到相应右值并让重载选择区分可被转移来源的引用类型。
- [移动构造函数（move constructor）](../docs/glossary/day11.md#移动构造函数move-constructor)是以同类右值引用初始化新对象的构造函数，通常通过接管源对象可转移资源并把源对象留在满足其契约的有效状态来建立目标。
- [`std::move`](../docs/glossary/day11.md#stdmove) 是把实参显式转换为相应右值引用表达式的标准库函数；它本身不搬运资源，后续被选择的操作决定是否以及怎样移动。

这里先把 `T&&` 当成“这个重载愿意接收可转移来源”的函数参数形式。表达式值类别的完整体系留到 Day 20；今天无需用尚未掌握的术语链解释它。

**代码与机制**：移动构造可以这样实现：

```cpp
MoveOnlyBuffer(MoveOnlyBuffer&& other)
    : size_{other.size_}, data_{other.data_} {
    other.size_ = 0;
    other.data_ = nullptr;
}
```

逐行映射状态表：

1. `size_{other.size_}` 把元素数量交给新对象。
2. `data_{other.data_}` 复制的是数组 A 的地址；这一步还没有安全地完成所有权转移。
3. `other.data_ = nullptr` 让源对象不再释放数组 A；`delete[] nullptr` 是安全的。
4. `other.size_ = 0` 让本类的“空对象”状态前后一致。

`std::move(source)` 本身不执行上述四步。它只改变该表达式参与重载选择的形式，使 `MoveOnlyBuffer(MoveOnlyBuffer&&)` 可以被选中；真正修改两个对象的是移动构造函数体。如果类型没有可用的移动操作，表达式也可能走拷贝或直接编译失败，取决于候选函数和参数限定。

标准规定移动构造的识别与隐式声明规则，但不规定裸指针资源必须用“置空”实现；置空是本例类型选择的清晰契约。编译器还可能省略某些构造，不能把“源码写了 `std::move`”等同于“一定观察到一次移动构造调用”。

**错误做法与修复**：

```cpp
MoveOnlyBuffer(MoveOnlyBuffer&& other)
    : size_{other.size_}, data_{other.data_} {
    // 忘记复位 other
}
```

分类：代码可以编译，但两个对象都会认为自己拥有数组 A。第一次析构后另一个指针悬空，随后重复 `delete[]` 导致**未定义行为**；标准不保证一定崩溃。修复是让源对象不再拥有该资源，或改用能自动表达独占所有权的成员类型。

**小检查**：在 `MoveOnlyBuffer target{std::move(source)};` 中，哪一部分只影响重载选择，哪一部分真正修改 `source.data_`？

### 机制二：移动赋值要先结束目标的旧所有权

**实际问题**：移动构造的目标尚未存在；移动赋值的目标已经拥有自己的资源。下面 `target` 先拥有数组 B，`source` 拥有数组 A：

```cpp
MoveOnlyBuffer source{3};  // 数组 A
MoveOnlyBuffer target{1};  // 数组 B
target = std::move(source);
```

按顺序画出三步：

| 步骤 | `source` | `target` |
|---|---|---|
| 赋值前 | 拥有 A | 拥有 B |
| 处理目标旧状态 | 仍拥有 A | 释放 B |
| 接管完成 | 本类约定为空 | 拥有 A |

如果直接把 A 的地址覆盖到 `target.data_`，数组 B 的地址就丢失了，形成内存泄漏。因此“赋值”比“构造”多了一项任务：先正确结束目标旧状态。

**概念落点**：

- [移动赋值运算符（move assignment operator）](../docs/glossary/day11.md#移动赋值运算符move-assignment-operator)是把可移动来源的状态转移到一个已经完成初始化的同类目标对象中的非静态成员函数，常见签名为 `T& operator=(T&&)`。
- [移后状态（moved-from state）](../docs/glossary/day11.md#移后状态moved-from-state)是对象作为移动操作的非 `const` 来源后仍处于生命周期内、满足该类型所承诺约束但具体可观察值由其契约决定的状态。

**代码与机制**：

```cpp
MoveOnlyBuffer& operator=(MoveOnlyBuffer&& other) {
    if (this == &other) {
        return *this;
    }

    delete[] data_;       // 1. 释放目标旧数组 B
    size_ = other.size_;  // 2. 取得源大小
    data_ = other.data_;  // 3. 接管数组 A
    other.size_ = 0;      // 4. 源变为本类约定的空状态
    other.data_ = nullptr;
    return *this;
}
```

`this == &other` 处理 `object = std::move(object)`。本例选择让自移动保持原状；这是该类型的实现选择，不应背成所有类型的统一规则。

移动之后，`other` 并没有析构，它的生命周期仍在继续。本类明确承诺它为空，所以可以调用 `empty()`、析构，也可以给它重新赋入一个新对象。对于标准库类型，除非该类型另有更强说明，一般保证移后对象“有效但值未指定”：可以安全析构或赋值；读取具体内容前仍要满足相应成员函数的前置条件，不能一律断言容器一定为空。

本日示例暂未给移动操作添加 `noexcept`。它会影响某些标准库容器在重新分配时选择移动还是拷贝，但异常规格和异常安全要到 Day 19 系统学习；现在只记住“移动不等于绝不会抛异常”。

**错误做法与修复**：

```cpp
MoveOnlyBuffer& operator=(MoveOnlyBuffer&& other) {
    data_ = other.data_;  // 原来数组 B 的地址丢失
    size_ = other.size_;
    other.data_ = nullptr;
    other.size_ = 0;
    return *this;
}
```

分类：目标旧数组 B 再也无法释放，属于**内存泄漏**。修复是在覆盖拥有型指针前释放旧资源，并考虑自移动；更通用的异常安全写法在 Day 19 展开。

**小检查**：为什么移动构造通常不需要释放目标旧资源，而移动赋值必须考虑这一步？

### 机制三：Rule of Five 用于审计，Rule of Zero 用于简化

**实际问题**：裸资源类现在至少出现五条路径。漏掉任何一条，都可能让相同的数组在复制、移动或销毁时落入不同规则。

```cpp
~Buffer();
Buffer(const Buffer&);
Buffer& operator=(const Buffer&);
Buffer(Buffer&&);
Buffer& operator=(Buffer&&);
```

把它们按问题排列，比机械背五个名字更容易理解：

| 问题 | 对应函数 |
|---|---|
| 对象结束时谁释放？ | 析构函数 |
| 新对象怎样复制？ | 拷贝构造 |
| 已有对象怎样复制？ | 拷贝赋值 |
| 新对象怎样接管？ | 移动构造 |
| 已有对象怎样接管？ | 移动赋值 |

**概念落点**：

- [Rule of Five（五法则）](../docs/glossary/day11.md#rule-of-five五法则)是现代 C++ 的资源类设计提醒：若类需要自定义析构、拷贝构造、拷贝赋值、移动构造或移动赋值中的一个，通常应共同审视这五条特殊成员路径。
- [Rule of Zero（零法则）](../docs/glossary/day11.md#rule-of-zero零法则)是让专门的资源管理类型承担释放、复制和移动，使业务类无需自行声明析构、拷贝或移动特殊成员函数的设计原则。

**代码与机制**：Rule of Five 用来提醒我们审计资源类，不是要求所有类都手写五个函数。对只需要一组样本和名字的业务类，更简单的写法是：

```cpp
#include <string>
#include <vector>

class ScanRecord {
public:
    ScanRecord(std::string frame, std::vector<int> samples)
        : frame_{std::move(frame)}, samples_{std::move(samples)} {}

private:
    std::string frame_;
    std::vector<int> samples_;
};
```

`ScanRecord` 没有手写析构、拷贝或移动操作，但并非“没有资源管理”。`std::string` 和 `std::vector` 各自负责自己的动态资源；编译器生成的 `ScanRecord` 特殊成员会按成员语义组合它们。这就是优先 Rule of Zero 的原因：让经过验证的资源管理类型承担容易出错的细节。

特殊成员的生成并不是“缺哪个就一定补哪个”。用户声明的析构、拷贝或移动操作会影响其他特殊成员是否被隐式声明、是否被定义为删除。尤其是资源类只写析构函数后，不应想当然地认为还会得到理想的隐式移动；必须依据成员和声明逐项检查。面试时先说规则有条件，再分析具体类。

**错误做法与修复**：只写一个释放裸指针的析构函数，然后假设编译器会自动生成安全拷贝和移动。结果可能是默认拷贝造成双重释放、期望的移动操作未生成，或调用因成员约束而编译失败。分类依具体表达式可能是**编译错误**，也可能是可编译但资源语义错误并最终触发**未定义行为**。修复是明确设计全部相关路径，或把裸资源换成 `std::vector`、`std::string`、智能指针等资源管理成员，采用 Rule of Zero。

**小检查**：Rule of Zero 为什么不等于“类没有析构行为”？

## 💻 最小可运行示例

完整代码位于 [`examples/day11/main.cpp`](../examples/day11/main.cpp)。它实现一个可移动、不可拷贝的独占数组类，并依次演示移动构造与移动赋值。

关键调用：

```cpp
MoveOnlyBuffer target{std::move(source)};
MoveOnlyBuffer assigned{1};
assigned = std::move(target);
```

预期输出：

```text
target[1]: 20
source empty: true
assigned[1]: 20
target empty: true
```

这只能证明 `MoveOnlyBuffer` 自己承诺移后为空，不能推广成“所有类型移动后都为空”。

## 🐛 错误代码诊断

### 反例 A：移动时没有解除源所有权

若移动构造只复制拥有型指针、没有把源指针复位，两个析构函数会释放同一数组。分类：**重复释放导致未定义行为**。程序可能被 Sanitizer 检出，但标准不保证固定表现。

### 反例 B：复制不可复制类型

[`exercises/day11/broken/copy_move_only.cpp`](../exercises/day11/broken/copy_move_only.cpp) 显式删除拷贝构造，却执行 `MoveOnlyBatch copy{source};`。分类：**编译错误**。修复不是去掉 `= delete`，而是在业务确实要转移时实现移动构造并显式写 `std::move(source)`。

### 反例 C：移动赋值覆盖目标拥有型指针

若移动赋值未先释放目标旧数组就覆盖 `data_`，旧数组不再可达。分类：**内存泄漏**。修复是先结束目标旧所有权，再接管源资源，并说明自移动契约。

## 🎤 高频面试实战

先独立口述，再到[第 11 天面试答案](../interview/answers/day11.md)逐题核对全部追问。

### 题 1：移动构造、移动赋值和 `std::move` 分别做什么？

**30 秒口头回答**：移动构造用可移动来源初始化新对象，移动赋值把来源状态转入已经存在的目标并先处理目标旧状态。`std::move` 本身只是把表达式显式转换为可匹配右值引用重载的形式，不搬运资源；被选中的移动操作才决定怎样接管资源和留下源对象。移后对象仍在生命周期内，具体状态由类型契约决定，不能一概说必为空。

**完整答题逻辑**：先用“新目标/已有目标”区分两种操作；再把 `std::move` 与被选函数体分开；用数组 A 的所有权变化解释接管和源复位；最后限定移动是否发生、是否更快以及移后状态都取决于具体类型与调用条件。

**可能连续追问**：

- `std::move` 后一定调用移动构造吗？
- 移动构造为什么通常接收 `T&&`？
- 移动后源对象是否一定为空？
- 移动构造和移动赋值怎样区分？
- 移动一定比拷贝快吗？

**容易失分的说法**：说 `std::move` 会立刻清空对象；把移后对象说成已经析构；忽略移动赋值的目标旧资源；断言移动永远发生、永远常数复杂度或永远不抛异常。

### 题 2：Rule of Five 与 Rule of Zero 有什么关系？为什么现代 C++ 更偏好 Rule of Zero？

**30 秒口头回答**：Rule of Five 提醒亲自管理资源的类共同审视析构、两种拷贝和两种移动路径。Rule of Zero 则把资源交给 `std::vector`、`std::string`、智能指针等专门类型，让业务类使用按成员组合的默认特殊成员。现代 C++ 更偏好 Rule of Zero，因为它减少手写所有权代码和不一致路径，但成员是否可拷贝、可移动仍会影响业务类的默认操作。

**完整答题逻辑**：先按销毁、复制、转移列出五条路径；说明五法则是设计提醒而非语法强制；再用含 `vector` 的业务类解释零法则并非没有资源；最后补充用户声明的特殊成员会影响隐式生成，不能只凭函数数量判断正确性。

**可能连续追问**：

- 写了析构函数后编译器还一定生成移动操作吗？
- 可移动但不可拷贝的类型怎样声明？
- Rule of Five 是标准强制语法吗？
- Rule of Zero 是否意味着类没有析构行为？
- 标准库容器为什么关心移动操作能否抛异常？

**容易失分的说法**：要求每个类机械写五个函数；认为零法则禁止资源；忽略成员类型会使默认操作被删除；在尚未分析类型时声称编译器一定生成或一定调用某个操作。

## ✍️ 当日练习

完成 [`exercises/day11/README.md`](../exercises/day11/README.md)：5 道判断、4 道代码与状态分析，以及一个可移动但不可复制的 `MoveOnlyBatch`。答案与解析在 [`solutions/day11/README.md`](../solutions/day11/README.md)，请先画完所有权表再查看。

## 🧠 深度思考题

一个机器人任务对象持有“大型轨迹数组”和“只读任务编号”。比较三种设计：深拷贝轨迹、移动转移轨迹、用 `std::vector` 采用 Rule of Zero。分别说明复制/转移后的两个对象能做什么、谁负责释放、调用者怎样表达意图，以及哪种设计最不容易留下双重释放或泄漏。

## ✅ 完成标准

- 能画出移动构造和移动赋值前后的资源所有权表。
- 能准确说明 `std::move` 只做转换，被选操作才执行资源转移。
- 能解释移后对象仍在生命周期内，具体值由类型契约决定。
- 能为独占裸数组实现基本移动构造与移动赋值，并识别双重释放和泄漏。
- 能说明 Rule of Five 是审计提醒，Rule of Zero 是现代工程的优先设计方向。

规则依据：C++ 公开工作草案的 [copy/move constructors](https://eel.is/c++draft/class.copy.ctor)、[copy/move assignment](https://eel.is/c++draft/class.copy.assign)、[`std::move`](https://eel.is/c++draft/forward) 与 [moved-from library types](https://eel.is/c++draft/lib.types.movedfrom) 条款，以及 C++ Core Guidelines 的 [C.20/C.21](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rc-zero)；公开草案不是已发布 ISO 标准本身。
