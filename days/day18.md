# Day 18：智能指针——先说清谁负责，再谈自动释放

> C++17 主线；`unique_ptr`、`shared_ptr`、`weak_ptr` 来自 C++11，`make_unique` 来自 C++14。建议用时：150—190 分钟。

## 🎯 今日攻坚目标

1. 用 `unique_ptr` 表达唯一管理责任，并能预测移动前后的对象状态。
2. 用 `shared_ptr` 表达真正的共同管理，判断被管理对象何时结束。
3. 用 `weak_ptr::lock()` 安全观察共享对象，并识别、打断循环共享。

## 🔁 前置知识检查

1. Day 10 的“资源所有权”回答的是哪两个问题？
2. `std::move` 会不会亲自移动对象？移动后源对象还能否销毁和重新赋值？
3. 一个局部对象离开作用域时，析构函数为什么会被调用？

## 📖 核心知识重构

今天只围绕一句话推理：**指针能找到对象，不等于它负责让对象结束。** 智能指针把这种责任写进类型，并在负责者离开时自动执行相应的销毁过程。这里的“语言规则/标准语义”是所有符合 C++17 的实现都要保持的代码含义；“实现方式/实现模型”则是编译器和运行环境满足这些规则的具体做法，二者不能混为保证。

### 机制一：`unique_ptr` 把唯一责任写进类型

**实际问题**：函数接收一个任务并长期保存。裸指针只告诉我们地址，调用者和函数都可能以为对方负责删除；也可能两边都删除。先看最小代码：

```cpp
auto source = std::make_unique<Job>(7);
auto destination = std::move(source);

if (destination) {
    std::cout << destination->id() << '\n';
}
std::cout << (source == nullptr) << '\n';
```

这里只有一个 `Job` 对象，却有 `source`、`destination` 两个指针对象。第一行后 `source` 负责；第二行把责任交给 `destination`，`source` 变空。`source` 这个指针对象仍可检查、赋新值和销毁，只是为空时不能解引用；`Job` 在 `destination` 放弃责任或结束时不能再使用。

| 时刻 | `source` | `destination` | `Job{7}` 谁负责 |
|---|---|---|---|
| 创建后 | 指向对象 | 尚未创建 | `source` |
| 移动后 | 空 | 指向对象 | `destination` |
| `destination` 结束后 | 空 | 已结束 | 对象已销毁 |

**概念落点**：这就是 [`std::unique_ptr` 与 `std::make_unique`（unique pointer and make_unique）](../docs/glossary/day18.md#stdunique_ptr-与-stdmake_uniqueunique-pointer-and-make_unique)：`std::unique_ptr<T>` 是独占管理一个对象或数组的智能指针类型：任一时刻相应资源最多由一个 `unique_ptr` 负责结束，所有权可移动但不可复制；`std::make_unique`（C++14）通过构造参数创建对象并返回管理它的 `unique_ptr`。

**代码与机制**：`make_unique<Job>(7)` 创建 `Job` 并立刻把唯一责任交给返回的 `unique_ptr`。`std::move(source)` 只是允许移动构造被选择，真正转交责任的是 `unique_ptr` 的移动构造。复制被类型规则禁止，所以同一资源不会因一次普通复制突然出现两个独占负责者。标准保证移动后的 `source` 为空，也保证负责者销毁或 `reset()` 时销毁被管理对象；它不规定指针对象在内存中的物理布局。

**错误做法与修复**：

- **编译错误**：`auto copy = source;` 试图复制独占责任。修复为 `auto next = std::move(source);`，并明确之后只通过 `next` 访问。
- **未定义行为风险**：移动后执行 `source->id()`，空指针解引用没有可依赖的结果，不能写成“必然崩溃”。修复为检查 `source`，或使用新的负责者。
- **设计错误**：为了“方便传参”一律改成 `shared_ptr`。若责任本来唯一，继续传 `unique_ptr`；只借用时按接口约定传引用或非拥有指针。

**小检查**：`destination = std::move(source)` 之后，哪一个指针对象还能访问原来的 `Job`？`source` 本身还能做哪两件安全的事？

### 机制二：`shared_ptr` 只在确实共同负责时共享

**实际问题**：一个下载任务同时被界面和日志模块异步保留，无法事先确定谁最后结束。两边都需要保证任务活着：

```cpp
auto ui = std::make_shared<Job>(9);
auto log = ui;
std::cout << ui.use_count() << '\n';

ui.reset();
std::cout << log->id() << '\n';
log.reset();
```

这里有一个 `Job`，以及 `ui`、`log` 两个指针对象。复制后两者共同负责；`ui.reset()` 只让 `ui` 变空，`Job` 仍能通过 `log` 使用；最后 `log.reset()` 后没有负责者，`Job` 才结束。

| 时刻 | `ui` | `log` | 共享负责者数量 | `Job{9}` |
|---|---|---|---:|---|
| 创建后 | 负责 | 尚未创建 | 1 | 可用 |
| 复制后 | 负责 | 负责 | 2 | 可用 |
| `ui.reset()` 后 | 空 | 负责 | 1 | 可用 |
| `log.reset()` 后 | 空 | 空 | 0 | 已销毁 |

**概念落点**：这就是 [`std::shared_ptr` 与控制块（shared pointer and control block）](../docs/glossary/day18.md#stdshared_ptr-与控制块shared-pointer-and-control-block)：`std::shared_ptr<T>` 是让多个智能指针共同承担同一对象结束责任的类型；与该共享关系关联的控制块保存共享拥有者数量等管理信息，最后一个共享拥有者离开时相应对象被销毁。

**代码与机制**：`make_shared<Job>(9)` 建立对象及其共享关系；复制 `ui` 增加一个共享拥有者，`reset()` 或指针对象结束会减少一个。`use_count()` 能帮助观察这个示例，却不适合驱动业务或充当并发同步条件，因为读取之后数量仍可能变化。控制块是解释共享关系的实现模型：常见实现保存强计数、弱计数和删除方式；标准要求可观察的生命周期语义，却不要求某个 ABI、字段排列或固定物理布局。常见实现可让 `make_shared` 合并部分分配工作，但今天的选择依据仍是所有权，而非分配次数。

**错误做法与修复**：

- **资源管理错误**：从同一个裸指针分别构造两个互不相关的 `shared_ptr`，可能发生重复销毁，后果属于未定义行为。修复为只创建一次，再复制已有 `shared_ptr`；优先使用 `make_shared`。
- **并发逻辑错误**：写 `if (p.use_count() == 1)` 就认定之后始终独占。修复为使用同步机制或重新设计责任，不能把一次计数快照当锁。
- **性能/设计误区**：认为 `shared_ptr` 是“更安全的默认指针”。共享管理有计数和更复杂的生命周期；责任唯一时优先 `unique_ptr`。

**小检查**：`ui.reset()` 后为什么 `Job` 没有销毁？什么事件才决定它真正结束？

### 机制三：`weak_ptr` 安全观察但不延长生命

**实际问题**：缓存想查看任务，但不应仅因为缓存还记得它就让任务永远存在；父子对象还可能互相保存。先看观察过程：

```cpp
std::weak_ptr<Job> observer;
{
    auto owner = std::make_shared<Job>(12);
    observer = owner;
    if (auto alive = observer.lock()) {
        std::cout << alive->id() << '\n';
    }
}
std::cout << observer.expired() << '\n';
```

这里有一个 `Job`、一个负责它的 `owner`、一个不负责的 `observer`，以及 `lock()` 成功时短暂创建的 `alive`。内部代码块结束后，最后一个共享负责者离开，`Job` 结束；`observer` 仍是可检查的指针对象，但不能直接访问已经结束的 `Job`。

| 对象 | 内部代码块中 | 代码块结束后 |
|---|---|---|
| `owner` | 共享负责者 | 已结束 |
| `observer` | 只观察 | 仍存在，但已过期 |
| `Job{12}` | 可用 | 已销毁，不能再用 |

**概念落点**：这就是 [`std::weak_ptr`、`lock` 与循环共享（weak pointer, lock and cyclic sharing）](../docs/glossary/day18.md#stdweak_ptrlock-与循环共享weak-pointer-lock-and-cyclic-sharing)：`std::weak_ptr<T>` 是从 `shared_ptr` 共享关系建立、但不增加共享拥有者数量的非拥有观察者；`lock()` 在对象仍存在时返回一个共享拥有者，否则返回空 `shared_ptr`，它常用于打断会让对象彼此长期保留的循环共享。

**代码与机制**：`observer = owner` 只建立观察关系。`lock()` 把“检查仍存在”和“若存在则取得一次共享责任”作为一个操作完成；成功返回非空 `shared_ptr`，失败返回空值，因此应检查返回值后再访问。若父节点用 `shared_ptr` 管理子节点，子节点通常用 `weak_ptr` 指回父节点：反向边不增加共享拥有者数量，外部负责者离开后就不会只靠父子互相保留。控制块可能继续存在到最后一个 `weak_ptr` 结束，但被管理的 `Job` 已在强计数归零时销毁；不要把两者的结束时间混成一个对象生命周期。

**错误做法与修复**：

- **资源滞留/逻辑泄漏**：父、子都以 `shared_ptr` 保存对方，外部指针离开后两边计数仍不为零。它通常不会崩溃，而是对象和资源未按预期结束。修复为把不承担结束责任的反向边改成 `weak_ptr`。
- **未定义行为风险**：对 `observer.lock()` 的空结果直接解引用。修复为保存结果并判断：`if (auto p = observer.lock()) { ... }`。
- **概念误解**：认为 `weak_ptr` 是普通裸指针包装。它与共享控制块关联，能安全判断共享对象是否仍存在，但不能单独管理对象。

**小检查**：为什么不能先调用 `expired()`，过一会儿再放心解引用？`lock()` 把哪两步合成了一个安全操作？

## 💻 最小可运行示例

完整示例：[`examples/day18/main.cpp`](../examples/day18/main.cpp)

```text
unique id: 7
source empty: true
shared count: 2
after reset count: 1
weak alive: true
session 9 destroyed
weak expired: true
session 7 destroyed
```

输出顺序说明：`Session{9}` 的最后一个共享负责者先离开内部代码块，所以先析构；`destination` 持有的 `Session{7}` 到 `main` 末尾才结束。

## 🐛 错误代码诊断

| 现象 | 分类 | 后果边界 | 修复方向 |
|---|---|---|---|
| 复制 `unique_ptr` | 编译错误 | 程序无法生成 | 用 `std::move` 明确转交，或重新确认是否真需共享 |
| 解引用空的智能指针 | 未定义行为 | 不能依赖崩溃、输出或任何固定结果 | 解引用前检查，并沿正确所有权路径取得对象 |
| 两个独立 `shared_ptr` 接管同一裸指针 | 未定义行为风险 | 可能重复销毁 | 单一入口创建，共享时复制现有 `shared_ptr` |
| 对象互持 `shared_ptr` | 资源滞留/设计错误 | 对象可能一直不析构，但不是语言层面的未定义行为 | 非拥有反向关系改用 `weak_ptr` |
| 用 `use_count()` 做并发判定 | 并发逻辑错误 | 检查结果随即失效 | 使用同步机制或重构责任边界 |

故意编译失败示例：[`exercises/day18/broken/copy_unique_ptr.cpp`](../exercises/day18/broken/copy_unique_ptr.cpp)。

## 🎤 高频面试实战

答案汇总：[`interview/answers/day18.md`](../interview/answers/day18.md)

### 题 1：`unique_ptr` 与 `shared_ptr` 有什么区别？应该怎样选择？

**30 秒回答**：`unique_ptr` 表示唯一管理责任，不可复制但可移动，负责者结束时销毁对象；`shared_ptr` 表示多个指针共同负责，通过共享关系记录拥有者，最后一个拥有者离开时销毁对象。默认先选 `unique_ptr`，只有多个模块确实都要独立延长同一对象生命周期时才选 `shared_ptr`，只借用则不增加所有权。

**完整逻辑**：先说明两者解决的责任模型，再说复制/移动规则和销毁时机，最后给选择顺序。智能指针不是按“更高级”排序；接口应该让调用者一眼看出是转交、共享还是借用。`make_unique`/`make_shared` 让创建和接管处于一个清晰表达式中。

**可能连续追问**：

- `unique_ptr` 移动后，源对象还能做什么？
- 函数只在调用期间读取对象，参数一定要写成 `shared_ptr` 吗？
- 为什么不能复制 `unique_ptr`？
- 可以从同一个裸指针构造两个 `shared_ptr` 吗？

**容易失分的说法**：说“智能指针绝不会出错”；说移动后源指针对象已经被销毁；把 `shared_ptr` 当默认参数类型；用“栈快、堆慢”代替所有权判断。

### 题 2：`weak_ptr` 为什么能打破 `shared_ptr` 循环引用？`lock()` 做了什么？

**30 秒回答**：互相持有的 `shared_ptr` 会让共享拥有者数量无法归零。`weak_ptr` 只观察共享关系，不增加共享拥有者数量，所以把不负责生命周期的一条边改成 `weak_ptr` 后，外部拥有者离开时对象可以正常结束。`lock()` 会在对象仍存在时取得一个临时 `shared_ptr`，否则返回空值。

**完整逻辑**：先画出“谁拥有谁”的有向边；只有承担结束责任的边才用 `shared_ptr`。弱观察者自身可能比对象活得久，因此访问前必须 `lock()`。它不是先检查后裸用，而是在成功时取得一段受共享责任保护的使用期。强计数归零销毁对象；弱观察者仍存在时，管理共享关系所需的信息可以继续存在，这是实现模型，不代表业务对象仍活着。

**可能连续追问**：

- `expired()` 返回 `false` 后，能否保证下一行对象仍存在？
- `lock()` 返回的 `shared_ptr` 会影响对象寿命吗？
- 循环共享一定是未定义行为吗？
- 父子树结构中，哪一条边更适合使用 `weak_ptr`？

**容易失分的说法**：说 `weak_ptr` 会自动删除循环中的对象；说循环共享必然崩溃；忽略 `lock()` 的空结果；把控制块仍存在误说成被管理对象仍存在。

## ✍️ 当日练习

- **概念判断**：区分“源指针对象仍存在”和“已不再拥有资源”。
- **输出预测**：跟踪独占转交、共享计数与观察失效。
- **编程练习**：实现转交函数和安全观察函数，再设计父子关系。

题目见 [`exercises/day18/README.md`](../exercises/day18/README.md)，独立答案与解析见 [`solutions/day18/README.md`](../solutions/day18/README.md)。请先写出对象/责任变化表，再运行答案程序。

## 🧠 深度思考题

一个任务调度器把任务放入队列，工作线程取出并执行，监控页面只显示任务是否仍存在。分别为“队列到工作线程”“工作线程正在执行”“监控页面观察”选择 `unique_ptr`、`shared_ptr`、`weak_ptr`、引用或裸指针，并说明每条边是否承担让任务继续存在的责任。若你的设计用了 `shared_ptr`，能否进一步把责任收紧成唯一转交？

## ✅ 完成标准

- [ ] 不看答案，能画出 `unique_ptr` 移动前后的两个指针对象和一个被管理对象。
- [ ] 能用一句话解释 `shared_ptr` 的最后一个共享拥有者为何决定对象结束。
- [ ] 能识别一个双向 `shared_ptr` 循环，并指出哪条边应改成 `weak_ptr`。
- [ ] 能正确检查 `lock()` 结果，不把未定义行为描述成必然崩溃。
- [ ] 能完成三类练习，并用 GCC 与 Clang 以 C++17 构建当天正确示例。
