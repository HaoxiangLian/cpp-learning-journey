# 第 16 天：顺序容器怎样选

预计用时：120 分钟。标准主线：C++17；`std::vector`、`std::deque` 与 `std::list` 的基础接口来自更早标准，本日只使用 C++17 保证；`shrink_to_fit` 来自 C++11，仅作边界说明。

## 🎯 今日攻坚目标

完成本日后，你应能按正文顺序：

1. 用 `std::vector` 保存运行时可变数量的同类型元素，并沿 `push_back` 前后状态说出元素、`size()` 和合法下标怎样变化；
2. 区分 `size()` 与 `capacity()`、`reserve()` 与 `resize()`，预测哪一个会创建元素，并解释 `vector::push_back` 为什么是摊销常量时间；
3. 根据“是否随机访问、主要在首尾还是已知中间位置修改”在 `vector`、`deque`、`list` 中选择容器，并用复杂度说明理由；
4. 根据容器、修改操作、位置与是否重新分配，判断旧迭代器或引用能否继续使用，并修复失效迭代器代码。

## 🔁 前置知识检查

1. Day 7 中，内建数组的元素数量何时确定？运行时数量若用裸 `new[]/delete[]` 管理，调用者要额外承担什么责任？
2. Day 11 中，为什么让 `std::vector` 成为业务类成员有助于采用 Rule of Zero？
3. Day 15 中，`[first, last)` 的 `last` 是否代表一个可解引用元素？随机访问迭代器比双向迭代器多提供哪些能力？
4. Day 15 中，为什么 `std::sort` 能接收数组指针，却不能接收只有输入能力的迭代器？

参考方向：内建数组界限通常在编译期确定，裸动态数组需要人工配对释放并维护长度；`vector` 自己管理元素和存储；`last` 是排除端；算法能否调用取决于迭代器能力。今天会从一个 `vector<int>` 的实际状态开始，不要求你预先知道容量、复杂度或失效规则。

## 📖 核心知识重构

今天的顺序不能倒过来：先只认识一种容器及其元素状态；再解释它为什么会预留空间；有了操作成本，才比较另外两种顺序容器；最后统一处理修改后的旧位置。选择容器不是背“谁最快”，而是把代码真正需要的操作与标准保证对应起来。

### 机制一：`vector` 先解决“元素数量在运行时变化”

**实际问题**：传感器读数最初有 3 个，运行中又收到一个。内建数组 `int readings[3]` 不能把自身界限改成 4；先观察 `vector` 对象发生什么：

```cpp
#include <vector>

std::vector<int> readings{4, 9, 2};
readings.push_back(7);
readings[1] = 10;
```

| 时点 | 元素序列 | `size()` | 合法下标 |
|---|---|---:|---|
| 创建后 | `4, 9, 2` | 3 | 0、1、2 |
| `push_back(7)` 后 | `4, 9, 2, 7` | 4 | 0、1、2、3 |
| `readings[1] = 10` 后 | `4, 10, 2, 7` | 4 | 0、1、2、3 |

普通话结论：`readings` 是一个拥有 4 个元素的对象；`push_back` 在末尾创建新元素并让数量增加，合法下标范围随之变成 `[0, size())`。修改下标 1 只改变第二个元素，不改变元素数量。

**概念落点**：[顺序容器与 `std::vector`（sequence container and vector）](../docs/glossary/day16.md#顺序容器与-stdvectorsequence-container-and-vector)：顺序容器是元素次序由插入顺序及插入位置决定的一类标准容器；`std::vector<T>`（除 `bool` 特化）是其中自动管理可变长度连续元素序列、支持常量时间随机访问和末尾摊销常量时间插入的类模板。

本例 `T` 是 `int`，所以每个元素都是 `int`；“顺序”由创建时和随后插入的位置决定；“自动管理”表示 `vector` 在自身生命周期结束时负责销毁元素并释放其管理的存储，不要求我们手写 `delete[]`。

**代码与机制**：先掌握最小接口：

```cpp
std::vector<int> values{4, 9, 2};

values.push_back(7);       // 末尾增加一个元素
int first = values[0];     // 前提：0 < values.size()
bool has_values = !values.empty();

for (const int value : values) {
    std::cout << value << ' ';
}
```

`size()` 返回当前元素数量，`empty()` 回答数量是否为零，`begin()`/`end()` 给出 Day 15 学过的半开区间。对普通 `vector<int>`，元素连续存储并提供随机访问，因此合法的 `values[index]` 是常量时间操作。

初始化写法仍要结合 Day 2 判断：

```cpp
std::vector<int> a(3);  // 3 个 int 元素，值均为 0
std::vector<int> b{3};  // 1 个 int 元素，值为 3
```

这两个对象都有效，但状态不同。圆括号这里选择“元素数量”构造入口，花括号提供一个元素列表；不能只看数字相同就预测相同结果。

`operator[]` 要求调用者保证下标小于 `size()`，不自动做范围诊断。`at(index)` 会检查范围，并通过异常报告越界；异常机制在 Day 19 系统学习，本日完成代码时优先用范围 `for`、算法或先检查下标。

连续存储与随机访问是 C++17 可依赖的接口保证，不需要猜 `vector` 对象内部保存几个指针。`vector<bool>` 是特殊特化，不提供普通 `vector<T>` 的全部连续元素引用性质；本日示例统一使用 `vector<int>`。

**错误做法与修复**：把 `size()` 本身当作最后一个合法下标：

```cpp
std::vector<int> values{4, 9, 2};
int bad = values[values.size()];  // 下标 3 越过 [0, 3)
```

分类：越界访问是**未定义行为**，不保证一定崩溃。修复是使用 `values.size() - 1` 前先确认非空，或直接使用 `back()` 取得最后一个元素；遍历优先使用范围 `for` 或 `[begin(), end())`。

**小检查**：`std::vector<int> values{5};` 与 `std::vector<int> values(5);` 各有几个元素？执行一次 `push_back(8)` 后，`size()` 与最后一个合法下标分别是什么？

### 机制二：先分清已有元素和预留空间，再讨论扩容成本

**实际问题**：我们预计还会收到读数，想提前准备空间，但“准备空间”不等于“已经有这些元素”。观察同一个对象：

```cpp
std::vector<int> values{4, 9, 2};
const auto old_capacity = values.capacity();

values.reserve(old_capacity + 5);  // 参数必然大于原 capacity
values.resize(5, 0);
values.resize(2);
```

| 时点 | 元素序列 | `size()` | `capacity()` 可确定的结论 |
|---|---|---:|---|
| 创建后 | `4, 9, 2` | 3 | 至少为 3 |
| `reserve(old_capacity + 5)` 后 | `4, 9, 2` | 3 | 至少为 `old_capacity + 5` |
| `resize(5, 0)` 后 | `4, 9, 2, 0, 0` | 5 | 至少为 5 |
| `resize(2)` 后 | `4, 9` | 2 | 不因这句被要求缩小 |

普通话结论：`reserve` 后没有新增元素，合法下标仍只有 0、1、2；`resize(5, 0)` 才创建两个值为 0 的元素；随后缩到 2 会移除末尾三个元素。容量可以比数量大，具体数值不必等于请求值。

**概念落点**：本机制先定义“元素与空间”，再用它解释一串末尾插入的总成本。

首先，[大小、容量、`reserve`、`resize` 与重新分配（size, capacity, reserve, resize and reallocation）](../docs/glossary/day16.md#大小容量reserveresize-与重新分配size-capacity-reserve-resize-and-reallocation)：`std::vector` 的 `size` 是当前元素数量，`capacity` 是无需重新分配即可容纳的元素数量；`reserve(n)` 只提出至少容纳 `n` 个元素的容量要求而不改变 `size`，`resize(n)` 则把元素数量改为 `n`，重新分配是容器取得新存储并把现有元素转移到其中的过程。

接着，[复杂度与摊销复杂度（complexity and amortized complexity）](../docs/glossary/day16.md#复杂度与摊销复杂度complexity-and-amortized-complexity)：复杂度描述操作所需工作量如何随元素数量 `n` 增长；摊销复杂度把一串操作中少数高成本步骤分摊到整串操作，给出每次操作的长期上界，而不保证每一次都同样便宜。

**代码与机制**：先把一次 `push_back` 分成两种状态：

| 插入前状态 | 本次需要做什么 | 本次成本量级 |
|---|---|---|
| `size() < capacity()` | 在已预留位置创建末尾元素 | 常量 |
| `size() == capacity()` | 取得更大存储、转移现有 `n` 个元素、再创建新元素 | 线性 |

单看触发重新分配的那一次，工作量会随已有元素数量增长，不能说每次都常量。但标准保证 `vector` 末尾插入为**摊销常量时间**：连续执行很多次 `push_back` 时，少数重新分配的成本可以分摊到整串操作。这里不是随机输入的统计平均，也不是某一次调用的时间承诺。

`reserve(n)` 只有在 `n > capacity()` 时才重新分配；否则什么也不改变。若能够合理估计最终数量，提前 `reserve` 可避免在不超过该容量的后续插入中重复重新分配。但不要在每次 `push_back` 前都只增加一点 `reserve`，那可能破坏容器原本的增长策略和摊销优势。

标准没有规定容量按 1.5 倍还是 2 倍增长，也没有规定一次 `reserve(n)` 后 `capacity()` 必须恰好等于 `n`。`shrink_to_fit` 自 C++11 起是请求把容量向 `size()` 收缩，但请求不是强制的；不能把它当作必然释放到精确字节数的指令。

重新分配常见实现会申请一块更大的连续存储并移动或复制元素，随后释放旧存储；具体分配策略属于实现选择。初学者先依赖公开结果：元素值仍按容器规则存在，但旧位置是否可用要按机制四判断。

**错误做法与修复**：把预留容量当成已经创建的元素：

```cpp
std::vector<int> values;
values.reserve(3);
values[0] = 4;  // size() 仍为 0
```

分类：对不存在的元素执行越界写入是**未定义行为**。若要逐个加入读数，使用 `push_back`；若业务确实需要立即存在 3 个 `int` 元素，使用 `resize(3)` 或相应构造函数。

**小检查**：空 `vector` 执行 `reserve(10)` 后，`size()` 是多少、哪些下标合法？再执行 `resize(3, 7)` 后有哪些元素？为什么某一次 `push_back` 仍可能是线性成本？

### 机制三：先列出主要操作，再选择 `vector`、`deque` 或 `list`

**实际问题**：三个任务都保存有顺序的元素，但操作方式不同：

```cpp
std::vector<int> samples{4, 9, 2};
int second = samples[1];

std::deque<std::string> jobs{"normal"};
jobs.push_front("urgent");
jobs.push_back("later");

std::list<int> route{10, 30};
auto position = route.begin();
++position;
route.insert(position, 20);
```

| 真实需求 | 当前结果 | 最关键的操作 |
|---|---|---|
| 读数按下标读取，主要在末尾追加 | `4, 9, 2` | 随机访问、末尾插入 |
| 任务既可能加到最前，也可能加到最后 | `urgent, normal, later` | 首尾插入 |
| 已经拿到 `30` 前的位置，要插入途经点 | `10, 20, 30` | 已知位置插入 |

普通话结论：三个容器都能保存顺序，但它们保证便宜的操作不同。选择前先写“代码最频繁做什么”，不能从容器名字或某个单独的大 O 结论猜答案。

**概念落点**：[`std::deque` 与 `std::list`（deque and list）](../docs/glossary/day16.md#stddeque-与-stdlistdeque-and-list)：`std::deque` 是支持随机访问、并保证在首尾常量时间插入和删除的顺序容器；`std::list` 是只提供双向迭代、不提供随机访问、但在已知位置可常量时间插入或删除单个元素的顺序容器。

`vector` 已在机制一定义；现在三者可以用同一组问题比较：能否随机访问、哪里修改最频繁、修改位置是否已经取得。

**代码与机制**：C++17 主线下的常见保证可压成下表：

| 操作 | `vector` | `deque` | `list` |
|---|---|---|---|
| `container[index]` | 常量 | 常量 | 不提供 |
| 末尾插入/删除 | 插入摊销常量；删除常量 | 常量 | 常量 |
| 开头插入/删除 | 线性 | 常量 | 常量 |
| 已知迭代器处插入/删除一个元素 | 通常线性 | 中间通常线性 | 常量 |
| 迭代器能力 | Random-access | Random-access | Bidirectional |
| 连续元素存储 | `vector<T>` 保证，`bool` 特化除外 | 不保证 | 不保证 |

`list` 的“已知位置常量插入”不包括寻找位置。若需求是“先从头走到第 1000 个元素，再插入”，寻找本身是线性的；如果每次都从头寻找，不能只报告最后一小步的常量复杂度。

工程中的初始选择通常是 `vector`：它接口简单、随机访问、连续存储、从头到尾遍历常见表现好。只有需求明确要求频繁首尾操作时考虑 `deque`，或确实长期持有位置、频繁在已知中间位置修改且需要其稳定性时考虑 `list`。最终性能仍应以真实数据和测量验证，不能仅凭“链表插入 O(1)”断言更快。

C++17 标准保证的是上表中的操作、迭代器类别与复杂度。`deque` 常见实现使用多块存储，`list` 常见实现使用分别分配并互相链接的节点；块大小、节点字段和缓存表现不是可移植接口。即使常见实现帮助解释性能，也不能用其内部地址布局编写业务代码。

`std::sort` 需要随机访问迭代器，所以可用于 `vector` 和 `deque` 的区间，不能用于 `list` 的迭代器。若代码必须大量按下标访问，就不应先选 `list` 再寻找强制转换办法。

**错误做法与修复**：[`exercises/day16/broken/list_subscript.cpp`](../exercises/day16/broken/list_subscript.cpp) 写出 `values[1]`。分类：**编译错误**，因为 `std::list` 不提供随机访问下标。修复是根据需求改用 `vector`/`deque`，或通过迭代器顺序遍历；不要把线性遍历包装成看似常量时间的下标接口。

**小检查**：若任务需要频繁按下标读取并只在末尾追加，为什么先选 `vector`？若任务频繁从首尾进出且仍需下标访问，为什么 `deque` 更匹配？“已知中间位置”对 `list` 的常量插入结论为什么重要？

### 机制四：容器修改后，旧位置必须按失效规则重新审计

**实际问题**：修改前，迭代器指向值 9；随后明确请求超过当前容量的空间：

```cpp
std::vector<int> values{4, 9, 2};
const std::size_t remembered_index = 1;
auto old = values.begin() + remembered_index;

values.reserve(values.capacity() + 1);  // 必然重新分配

// 不再读取、比较或移动 old
auto current = values.begin() + remembered_index;
std::cout << *current;  // 9
```

| 时点 | `values` 中的元素 | `old` 能否使用 | 安全位置 |
|---|---|:---:|---|
| `reserve` 前 | `4, 9, 2` | 能，指向 9 | `old` |
| 重新分配完成后 | 仍为 `4, 9, 2` | 不能 | 尚未重新取得 |
| 再次调用 `begin()` 后 | 仍为 `4, 9, 2` | 仍不能 | `current` 指向 9 |

普通话结论：容器和元素值仍然存在，不代表修改前取得的位置仍可用。`old` 从重新分配完成时起就不能再用于访问，必须从修改后的容器重新取得位置。

**概念落点**：[迭代器失效（iterator invalidation）](../docs/glossary/day16.md#迭代器失效iterator-invalidation)：迭代器失效是容器修改使先前取得的迭代器不再可用于原来的遍历或访问；同一修改是否也使指针或引用失效，取决于具体容器、操作位置以及是否发生重新分配。

“失效”描述的是旧访问入口不再满足使用条件，不是说容器对象已经析构。旧迭代器即使变量仍在作用域内，也不能因此继续解引用。

**代码与机制**：先掌握高频修改，不一次背完整标准库：

| 容器与修改 | 旧迭代器/引用的关键规则 |
|---|---|
| `vector::reserve` 发生重新分配 | 全部迭代器、指针、引用及旧 `end()` 失效；未重新分配则保持有效 |
| `vector::push_back`/`insert` 发生重新分配 | 全部失效 |
| `vector::insert` 未重新分配 | 插入点之前保持有效；插入点及之后和旧 `end()` 失效 |
| `vector::erase` | 删除点及之后的迭代器和引用失效 |
| `deque` 在首尾插入 | 全部旧迭代器失效；对既有元素的引用保持有效 |
| `deque` 在中间插入 | 全部旧迭代器和元素引用失效 |
| `list::insert` | 既有元素的迭代器和引用保持有效 |
| `list::erase` | 只使被删除元素的迭代器和引用失效 |

修复策略按意图选择：

1. 修改后重新调用 `begin()`、`find()` 或使用修改函数返回的新迭代器；
2. 若只发生重新分配而元素顺序不变，可像本例暂存下标，修改后重新计算位置；若在前方插入或删除，下标也会改变，不能机械套用；
3. 已知合理上界时先 `reserve`，可在不超过容量的末尾插入期间避免重新分配，但旧 `end()` 等仍要按具体操作规则判断；
4. 不把容器迭代器长期缓存到无法控制容器修改的代码之外。

[`exercises/day16/broken/stale_vector_iterator.cpp`](../exercises/day16/broken/stale_vector_iterator.cpp) 在必然重新分配后解引用 `old`。分类：**未定义行为**，不保证一定崩溃，也不保证总能被测试发现。修复是丢弃旧迭代器并从修改后的容器重新取得，或重新设计代码让取得位置与使用位置之间不发生会使其失效的修改。

**错误做法与修复**：用“元素值看起来没变”证明旧迭代器有效。值是否相同与访问入口是否满足标准前提是两个问题；重新分配后即使打印结果偶然正确，仍不能使用旧迭代器。修复必须依据容器和操作的失效规则，而不是一次运行现象。

**小检查**：`vector` 因 `reserve(capacity() + 1)` 重新分配后，旧引用和旧迭代器怎样处理？`list` 删除中间元素后，指向第一个元素的迭代器为什么仍可用？`deque::push_back` 后，旧迭代器与旧元素引用的结论为什么不同？

## 💻 最小可运行示例

完整 C++17 代码位于 [`examples/day16/main.cpp`](../examples/day16/main.cpp)。它依次展示 `vector` 的元素状态、`reserve/resize`、按下标重新取得位置，以及 `deque` 首尾插入和 `list` 删除后的稳定位置。

稳定输出：

```text
vector: 4 9 2 7
reserve keeps size: true
capacity enough: true
resized size: 5
resized last: 0
refreshed value: 9
deque: urgent normal later
list after erase: 10 30
kept list value: 10
```

示例故意不输出具体 `capacity()`，因为标准只保证它达到最低要求，不规定增长倍率或精确值。

## 🐛 错误代码诊断

### 反例 A：`size()` 是最后一个合法下标

合法下标范围是 `[0, size())`，所以 `values[values.size()]` 越界。分类：**未定义行为**；修复为先检查非空后使用 `back()`，或使用安全遍历。

### 反例 B：`reserve(10)` 创建了 10 个元素

`reserve` 只处理容量，`size()` 不变；随后写 `values[0]` 仍可能越界。分类：**未定义行为**。需要元素时使用构造、`resize` 或 `push_back`。

### 反例 C：`list` 也能按下标访问

[`exercises/day16/broken/list_subscript.cpp`](../exercises/day16/broken/list_subscript.cpp) 是**编译错误**。`list` 只提供双向迭代；需要随机访问时选择 `vector` 或 `deque`。

### 反例 D：重新分配后旧迭代器仍指向同一个值

[`exercises/day16/broken/stale_vector_iterator.cpp`](../exercises/day16/broken/stale_vector_iterator.cpp) 解引用失效迭代器，属于**未定义行为**。容器仍有相同值并不能恢复旧访问入口。

## 🎤 高频面试实战

先独立口述，再到[第 16 天面试答案](../interview/answers/day16.md)逐题核对全部追问。

### 题 1：`std::vector` 的 `size`、`capacity`、`reserve` 和 `resize` 有什么区别？

**30 秒口头回答**：`size()` 是当前存在的元素数量，决定合法元素范围；`capacity()` 是不重新分配时最多可容纳的元素数量。`reserve(n)` 只保证容量至少达到 `n`，不创建元素也不改变 `size()`；`resize(n)` 把元素数量改为 `n`，会创建或移除末尾元素。超过容量的增长会重新分配并使旧迭代器、指针和引用失效。`push_back` 偶尔因此是线性成本，但一串末尾插入具有摊销常量复杂度。

**完整答题逻辑**：先用 `reserve` 前后的 `size` 和合法下标证明“空间不等于元素”；再用 `resize` 增大、减小展示元素创建与移除；随后说明重新分配的触发条件和失效范围。最后区分一次调用与整串调用的成本，并明确容量增长倍率、精确容量和底层分配策略不是 C++17 固定值。

**可能连续追问**：

- `reserve(100)` 会创建 100 个元素吗？
- `resize(100)` 与 `reserve(100)` 对 `size()` 的影响分别是什么？
- `vector` 的容量增长倍率由 C++17 固定吗？
- 为什么 `push_back` 是摊销常量时间，而不是保证每次常量时间？
- `vector` 发生重新分配后，旧指针、引用和迭代器怎样处理？

**容易失分的说法**：把容量当元素数量；声称 `reserve` 可直接产生合法下标；把扩容倍率写死；把摊销常量说成每次常量；只说地址“可能变”却不说旧访问入口已经失效。

### 题 2：`vector`、`deque`、`list` 应该怎样选择？容器修改后迭代器何时失效？

**30 秒口头回答**：通常先选 `vector`，因为它支持连续存储、随机访问和末尾摊销常量插入。若频繁在首尾插入删除且仍需随机访问，可选 `deque`；若已经持有中间位置、需要常量时间插入删除和其他元素位置稳定性，可考虑 `list`，但它不支持随机访问，寻找位置仍可能线性。失效必须按容器和操作判断：`vector` 重新分配会全部失效，`deque` 首尾插入使迭代器失效但保留既有元素引用，`list` 插入不影响既有位置、删除只使被删位置失效。

**完整答题逻辑**：先把主要访问与修改模式列出，再映射随机访问、首尾操作和已知位置操作的复杂度；说明复杂度不等于实际纳秒数，`vector` 仍是常见默认。随后把失效规则与修改动作绑定，而不是给整个容器贴“稳定/不稳定”标签；最后说明取得第 `n` 个位置的成本、标准保证与常见节点/分块实现边界。

**可能连续追问**：

- 为什么通常把 `vector` 作为默认顺序容器？
- 什么场景更适合 `deque`？
- `list` 在已知位置常量时间插入，是否表示“在第 1000 个位置插入”整体也是常量时间？
- `deque` 在首尾插入后，旧迭代器和旧引用都失效吗？
- `list` 删除一个元素后，哪些迭代器和引用失效？

**容易失分的说法**：只凭“链表插入 O(1)”选择 `list`；忘记寻找位置成本；断言 `deque` 连续存储；把迭代器与引用的失效结论混为一谈；用某次运行结果替代标准规则。

## ✍️ 当日练习

完成 [`exercises/day16/README.md`](../exercises/day16/README.md)：6 道判断、5 道状态/复杂度/失效分析，以及一个按操作模式使用三种顺序容器的小程序。答案与参考实现位于 [`solutions/day16/README.md`](../solutions/day16/README.md)，请先写出每一步的元素序列、`size/capacity` 关系和旧位置状态，再查看答案。

## 🧠 深度思考题

机器人系统中有三组数据：连续追加并频繁计算的传感器帧、允许紧急任务插到最前的调度队列、长期持有若干路径节点位置并在其附近插删的编辑器。分别选择顺序容器，列出最频繁操作及复杂度；再说明一次新增或删除后，已有迭代器、引用和下标身份是否还能代表原对象。若真实性能与复杂度直觉不同，你会怎样设计基准测量而不是直接换容器？

## ✅ 完成标准

- 能从 `vector` 代码状态说出元素、`size()` 和合法下标，不再用裸动态数组承担普通可变序列所有权。
- 能准确区分 `reserve` 与 `resize`，说明重新分配触发条件、容量增长边界和摊销常量含义。
- 能按随机访问、首尾操作和已知位置操作在 `vector`、`deque`、`list` 中作出有条件的选择。
- 能判断常见 `vector/deque/list` 修改后的迭代器与引用状态，并重新取得安全位置。
- 能把未定义行为与保证崩溃分开，把标准复杂度与常见实现性能分开。

规则依据：C++ 公开工作草案的 [sequence containers](https://eel.is/c++draft/sequences)、[`vector` overview](https://eel.is/c++draft/vector.overview)、[`vector` capacity](https://eel.is/c++draft/vector.capacity)、[`vector` modifiers](https://eel.is/c++draft/vector.modifiers)、[`deque` overview/modifiers](https://eel.is/c++draft/deque.overview) 与 [`list` overview/modifiers](https://eel.is/c++draft/list.overview) 条款，以及 C++ Core Guidelines 的 [SL.con.2：默认优先 `vector`](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rsl-vector)；公开工作草案不是已发布 ISO 标准本身。
