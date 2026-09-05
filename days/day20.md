# Day 20：表达式的类型和值从哪里来——推导、值类别与转发

> C++17 主线；`auto`、`decltype`、右值引用和完美转发均源自 C++11。建议用时：140—180 分钟。

## 🎯 今日攻坚目标

1. 根据声明形式推导常见 `auto` 变量，判断它是独立对象还是对原对象的引用。
2. 区分 `decltype(name)` 与 `decltype((name))`，用查询结果预测修改会落到哪个对象。
3. 分开判断表达式的类型和值类别，并据此预测引用绑定和重载选择。
4. 用引用折叠解释转发引用，再用 `std::forward` 保留调用者原来的左值或右值性质。

## 🔁 前置知识检查

1. `const int source{3}; int copy{source};` 中有几个对象？修改 `copy` 会影响 `source` 吗？
2. `T&` 与 `T&&` 分别可以绑定哪些表达式？`const T&` 为什么也能观察临时对象？
3. `std::move(object)` 自己是否搬运资源？真正的移动在什么时候发生？

## 📖 核心知识重构

今天把两个问题分开：**编译器为表达式和变量确定什么类型**，以及**当前表达式怎样表示一个对象或计算结果**。这里的“语言规则/标准语义”是所有符合 C++17 的实现都要保持的代码含义；“实现方式/实现模型”才是编译器如何记录类型、选择指令等具体做法。初学者先按声明、对象和重载结果推理，不需要猜寄存器或物理布局。

### 机制一：`auto` 根据目标声明形式决定复制还是引用

**实际问题**：同一个 `const int` 被 `auto` 接收时，有时得到能修改的副本，有时却仍在观察原对象。先看 6 行：

```cpp
const int source{7};
auto copy = source;
const auto& view = source;

copy = 8;
std::cout << source << ' ' << copy << ' ' << view << '\n';
```

这里有两个整数对象：`source` 和 `copy`；`view` 是 `source` 的另一个名字，不创建第三个整数对象。只有 `copy` 从 7 变成 8，`source` 与 `view` 仍读到 7。离开所在作用域后，两个对象和引用都不能再用；在这之前，`view` 的可用时间还不能超过它所绑定的 `source`。

| 声明 | 得到什么 | 修改落点 |
|---|---|---|
| `auto copy = source` | 独立的可修改 `int` 对象 | `copy` |
| `const auto& view = source` | 对 `source` 的只读引用 | 不能通过 `view` 修改 |

**概念落点**：这就是 [`auto` 类型推导（auto type deduction）](../docs/glossary/day20.md#auto-类型推导auto-type-deduction)：`auto` 类型推导是编译器根据初始化表达式和声明中 `auto` 周围的 `const`、`&`、`&&` 等形式确定变量类型的编译期规则。

**代码与机制**：`auto copy = source` 是按值声明，推导时不保留 `source` 类型最外层的 `const`，所以 `copy` 可改；`const auto& view` 把“只读引用”写在目标声明里，因此绑定原对象。`auto` 不是运行时才变化的类型：编译完成前，`copy` 和 `view` 的类型已经唯一确定。C++17 保证这些推导与绑定结果，但不要求编译器把引用实现为某种固定大小的指针。

**错误做法与修复**：

- **结果不符合预期**：想修改原对象却写 `auto value = object`，实际只改副本。修复为确认允许修改后使用 `auto& value = object`。
- **编译错误**：写 `auto& reference = 42`，可修改左值引用不能绑定临时值。只读观察可用 `const auto&`，需要独立值则用 `auto value = 42`。
- **类型误判**：把 `auto` 说成动态类型。修复为在纸上先替换出完整声明，再分析运行时对象。

**小检查**：`const int limit{5}; auto& alias = limit;` 中 `alias` 推导为什么带 `const`？能否通过它赋值？

### 机制二：`decltype` 查询表达式，括号可能改变查询规则

**实际问题**：模板或包装函数需要精确保留一个表达式的类型，按值的 `auto` 会丢掉引用信息。观察 6 行：

```cpp
int value{10};
decltype(value) copy{20};
decltype((value)) alias = value;

alias = 11;
std::cout << value << ' ' << copy << '\n';
```

有两个整数对象：`value` 和 `copy`；`alias` 引用 `value`。给 `alias` 赋 11 会改变 `value`，`copy` 仍是 20。作用域结束后它们都不能再使用；`alias` 在此之前也依赖 `value` 仍然存在。

| 查询 | 结果 | 原因 |
|---|---|---|
| `decltype(value)` | `int` | 未加括号的名字使用声明类型 |
| `decltype((value))` | `int&` | `(value)` 是表示既有对象的左值表达式 |

**概念落点**：这就是 [`decltype` 类型查询（decltype type query）](../docs/glossary/day20.md#decltype-类型查询decltype-type-query)：`decltype` 类型查询是在不按通常方式求值其操作数的情况下，依照操作数的语法形式和值类别得到一个类型的编译期规则。

**代码与机制**：对未加括号的变量名，`decltype(value)` 直接给出其声明类型。进入一般表达式规则后，左值结果为 `T&`，将亡值结果为 `T&&`，纯右值结果为 `T`；所以额外括号会让 `value` 按表达式类别判断。`decltype(f())` 通常不会真的调用 `f`。C++14 增加的 `decltype(auto)` 可把同一规则用于变量或函数返回类型，因此返回 `(member)` 时可能保留引用，必须审查被引用对象的生命周期。

**错误做法与修复**：

- **悬空风险**：函数用 `decltype(auto)` 返回局部变量的带括号表达式，可能返回对即将结束对象的引用。修复为按值返回，或只引用寿命由调用者保证的对象。
- **结果不符合预期**：认为多一层括号只影响排版，误把 `decltype((value))` 当成 `int`。修复为先识别是否走“未加括号名字”的特殊规则。
- **概念错误**：认为 `decltype(function())` 必定执行函数。修复为区分编译期类型查询和普通运行时求值。

**小检查**：`const int level{3};` 的 `decltype(level)` 与 `decltype((level))` 分别是什么？

### 机制三：值类别决定表达式怎样参与绑定和重载

**实际问题**：Day 11 看到 `std::move` 能选择移动入口，但“有名字”“临时结果”和“即将被复用资源”究竟怎样影响重载？先看 8 行：

```cpp
struct Packet {};

void route(const Packet&) { std::cout << "read\n"; }
void route(Packet&&) { std::cout << "move\n"; }

Packet packet;
route(packet);
route(Packet{});
```

`packet` 是一个具名对象，第一调用输出 `read`；`Packet{}` 创建用于本次调用的临时对象，第二调用输出 `move`，完整表达式结束后该临时对象销毁，不能再用。两个表达式的类型都与 `Packet` 有关，但它们表示对象的方式不同。

**概念落点**：这就是 [值类别（value category）](../docs/glossary/day20.md#值类别value-category)：值类别是表达式的一项编译期分类：左值表示可定位的对象或函数，将亡值表示资源可被复用的临近结束对象，纯右值用于初始化对象或计算一个值；将亡值和纯右值合称右值。

**代码与机制**：名字表达式 `packet` 是左值，适合绑定普通左值引用或 `const` 左值引用；临时构造表达式 `Packet{}` 是纯右值，可匹配 `Packet&&`。`std::move(packet)` 按规则产生将亡值，让右值引用重载成为候选，但它本身不移动；被选中的构造函数或重载才决定是否复用资源。一个声明为 `Packet&& reference` 的变量，使用名字 `reference` 时仍是左值，因为这个名字能稳定定位该对象。值类别是标准规定的表达式分类，不说明对象一定放在栈、堆或寄存器。

**错误做法与修复**：

- **概念错误**：把右值定义为“写在等号右边的东西”。赋值右侧也完全可以是左值。修复为问：表达式是否稳定表示某个对象，还是临时计算/可复用来源？
- **结果不符合预期**：右值引用形参有名字后直接向下传，意外选择左值重载。修复方向是机制四的条件转发。
- **有效但逻辑错误**：对后面仍需其原值的对象调用 `std::move`。移动是否发生取决于后续操作，若发生，源对象只保证满足类型契约。修复为仅在确实放弃原值时转换。

**小检查**：`Packet&& ref = Packet{}; route(ref);` 为什么选择读取入口，而 `route(std::move(ref));` 可选择移动入口？

### 机制四：转发引用在模板边界保留调用者的值类别

**实际问题**：一个包装函数既要接收具名对象，也要接收临时对象；进入函数后形参都有名字，若直接传下去，两次都会被当作左值。先看 10 行：

```cpp
template <class T>
void relay(T&& packet) {
    route(std::forward<T>(packet));
}

Packet packet;
relay(packet);    // read
relay(Packet{});  // move
```

第一调用中 `packet` 引用外部具名对象，不创建副本；第二调用中它引用本次调用的临时对象。`relay` 不改变对象，`std::forward` 只让下一次重载看到调用者原来的类别。临时对象在本次完整表达式结束后不能再用，外部具名对象则仍由原作用域控制。

| 调用 | 推导出的 `T` | `T&&` 折叠结果 | 转发结果 |
|---|---|---|---|
| `relay(packet)` | `Packet&` | `Packet&` | 左值 |
| `relay(Packet{})` | `Packet` | `Packet&&` | 右值 |

**概念落点**：先有模板推导，才有引用组合，最后才能转发。这就是 [转发引用、引用折叠与 `std::forward`（forwarding reference, reference collapsing and std::forward）](../docs/glossary/day20.md#转发引用引用折叠与-stdforwardforwarding-reference-reference-collapsing-and-stdforward)：转发引用是在类型模板参数由调用实参推导时形如 `T&&` 的函数参数；引用折叠把推导组合中的任意含左值引用情形折叠为左值引用、仅两个右值引用组合成右值引用，`std::forward<T>` 据此保留调用实参原来的值类别。

**代码与机制**：左值调用使 `T` 推导为 `Packet&`，组合出的 `Packet& &&` 按规则折叠成 `Packet&`；右值调用使 `T` 为 `Packet`，形参保持 `Packet&&`。虽然函数体里的名字 `packet` 本身是左值，`std::forward<T>` 会按 `T` 有条件地恢复入口类别。这组 C++11 机制常称“完美转发”，但它不承诺业务语义完美，也不执行复制或移动。普通类成员里的 `Packet&&`，或 `T` 已经确定而未从该调用推导的 `T&&`，都不是转发引用。

**错误做法与修复**：

- **结果不符合预期**：包装函数直接 `route(packet)`，所有调用都选择左值入口。修复为仅在转发引用模板中写 `std::forward<T>(packet)`。
- **逻辑错误**：在普通代码中无条件 `std::move(packet)`，会把调用者传入的左值也当成可移动来源。修复为转发时保留原类别，而不是一律转成右值。
- **概念错误**：看到任意 `T&&` 就称为转发引用。修复为同时检查“`T` 是否在这次函数调用中被推导”。

**小检查**：为何 `relay(packet)` 中会出现看似奇怪的 `Packet& &&`，它最终为什么仍是 `Packet&`？

## 💻 最小可运行示例

完整示例：[`examples/day20/main.cpp`](../examples/day20/main.cpp)

```text
copied: 8, limit: 7
view: 7
value through decltype ref: 11
lvalue route
rvalue route
```

前三行分别验证按值 `auto`、引用声明和 `decltype((name))`；最后两行验证转发保留了原调用的值类别。

## 🐛 错误代码诊断

| 现象 | 分类 | 后果边界 | 修复方向 |
|---|---|---|---|
| `auto& reference = 42` | 编译错误 | 非 `const` 左值引用不能绑定临时值 | 用独立值或 `const auto&` |
| 用 `auto` 按值接收却期望修改原对象 | 有定义但结果不符合预期 | 只修改副本 | 明确使用 `auto&` 并检查生命周期 |
| `decltype(auto)` 返回局部对象的引用 | 悬空/未定义行为风险 | 函数返回后引用所指对象已结束，若再访问则为未定义行为；不保证立刻崩溃 | 按值返回或延长被引用对象生命周期 |
| 把具名右值引用直接传给下层 | 有定义但结果不符合预期 | 选择左值入口 | 转发模板中使用 `std::forward` |
| 对仍需原值的对象无条件 `std::move` | 逻辑错误 | 若后续确实移动，源对象值由类型契约约束 | 只在放弃原值时移动 |

故意编译失败检查：[`exercises/day20/broken/bind_auto_reference.cpp`](../exercises/day20/broken/bind_auto_reference.cpp)。

## 🎤 高频面试实战

答案汇总：[`interview/answers/day20.md`](../interview/answers/day20.md)

### 题 1：`auto` 和 `decltype` 有什么区别？为什么 `decltype(name)` 与 `decltype((name))` 可能不同？

**30 秒回答**：`auto` 根据初始化表达式和目标声明形式推导变量类型，按值时通常去掉顶层 `const` 与引用；`decltype` 查询操作数的类型，不按通常方式求值。对未加括号的名字，`decltype(name)` 采用声明类型；`decltype((name))` 进入表达式规则，具名变量表达式是左值，因此得到 `T&`，并保留相应 `const`。两者都是编译期规则，不是动态类型。

**完整逻辑**：先看目标是要声明新变量还是精确查询表达式；再区分按值、引用以及 `decltype` 的名字特殊规则；最后通过“独立副本还是原对象别名”验证，并提醒 `decltype(auto)` 保留引用时要检查生命周期。

**可能连续追问**：

- `const int source{3}; auto value = source;` 中 `value` 能修改吗？
- `auto&` 和 `const auto&` 在绑定与修改能力上有什么区别？
- `decltype(function())` 会真的调用函数吗？
- `decltype(auto)` 返回局部变量时有什么风险？

**容易失分的说法**：说 `auto` 是运行时动态类型；说 `auto` 总保留 `const`；忽略 `decltype` 的未加括号名字规则；只背类型结论而不说明是否创建新对象。

### 题 2：什么是值类别和转发引用？`std::move` 与 `std::forward` 分别做什么？

**30 秒回答**：值类别描述表达式怎样表示对象或计算结果，左值、将亡值和纯右值会影响引用绑定与重载选择。`std::move` 无条件把实参转换成可匹配右值入口的表达式，本身不移动；转发引用是模板参数由调用推导时的 `T&&`，左值调用经引用折叠仍为左值引用，右值调用保持右值引用，`std::forward<T>` 据此有条件保留原类别。真正移动由随后选中的操作完成。

**完整逻辑**：先分开类型和值类别，再用具名对象、临时对象和具名右值引用说明重载；随后说明转发引用成立的两个条件、四条折叠规律与 `std::forward`；最后把资源变化归因于被选中的构造、赋值或函数，而不是转换工具。

**可能连续追问**：

- 一个类型为 `T&&` 的具名变量表达式为什么是左值？
- 四种引用组合怎样折叠？
- 任意函数参数 `T&&` 都是转发引用吗？
- 为什么包装函数直接传递具名形参会丢失调用者的右值性质？
- `std::forward` 会不会真正移动资源？

**容易失分的说法**：把右值定义成等号右边；说 `std::move` 立即搬资源；漏掉 `T` 必须由调用推导；把完美转发说成保证零开销或业务结果完美。

## ✍️ 当日练习

- **概念判断**：辨析 `auto` 限定、`decltype` 括号、类型与值类别。
- **执行结果预测**：判断副本与两个引用的修改分别落在哪个对象。
- **编程练习**：实现保留左值/右值性质的消息转发函数。

题目见 [`exercises/day20/README.md`](../exercises/day20/README.md)，独立答案与解析见 [`solutions/day20/README.md`](../solutions/day20/README.md)。

## 🧠 深度思考题

一个任务队列接口接收既有 `Task` 和临时 `Task`。请分别分析按值接收、`const Task&` 接收和转发引用接收时：会创建几个对象，调用者对象能否被修改或移动，临时对象何时结束；最后说明为什么接口不应只为了“看起来现代”就使用转发引用。

## ✅ 完成标准

- [ ] 能推导按值 `auto`、`auto&` 与 `const auto&`，并指出修改落点。
- [ ] 能准确说明 `decltype(name)` 与 `decltype((name))` 的差别。
- [ ] 能区分表达式类型和值类别，预测左值/右值重载选择。
- [ ] 能写出四条引用折叠结果并识别转发引用的成立条件。
- [ ] 能解释 `std::move`、`std::forward` 都不自行搬运资源。
