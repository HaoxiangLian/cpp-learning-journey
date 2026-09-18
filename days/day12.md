# 第 12 天：继承表达什么关系

预计用时：120 分钟。标准主线：C++17；`override` 来自 C++11。

## 🎯 今日攻坚目标

完成本日后，你应能按正文顺序：

1. 从 `class Lidar : public Sensor` 读出基类、派生类，并指出 `Lidar` 完整对象中的 `Sensor` 基类子对象；
2. 用“是一种”与“拥有一个”分别选择公有继承或组合，并说明派生对象绑定到基类引用时是否创建新对象；
3. 识别把派生对象按值复制成基类对象时发生的对象切片；
4. 为允许通过基类指针删除派生对象的类型层次设计虚析构函数。

## 🔁 前置知识检查

1. 一个类的成员对象在外层对象构造和析构时会发生什么？
2. `Sensor copied{source};` 与 `const Sensor& view{source};` 哪一个创建新对象？
3. 析构函数解决对象结束时的什么问题？通过指针执行 `delete` 时，指针必须指向怎样取得的对象？

参考方向：成员子对象会随外层对象按规则构造和析构；按值声明创建新对象，引用只是绑定；析构负责结束类级状态和资源，`delete` 必须匹配合法的动态分配及删除契约。

## 📖 核心知识重构

继承不是“高级代码复用语法”。今天不直接从“该不该继承”这种设计结论起步，而是依次回答四个问题：继承语法创建了怎样的对象关系；这种关系与成员组合怎样选择；按值变成基类对象会丢失什么；经基类指针删除时怎样结束完整对象。Day 13 再学习普通虚函数的运行时多态。

### 机制一：先读懂基类、派生类与基类子对象

**实际问题**：第一次看到 `class Lidar : public Sensor` 时，先不讨论这种设计是否合理，只回答两个可观察问题：哪个类已经存在，哪个类在它的基础上定义；创建一个 `Lidar` 时，对象里有没有 `Sensor` 的状态？


> [!NOTE] 对象的状态
> 对象的**状态**，简单说就是：
> > **这个对象当前保存着哪些数据，以及这些数据现在是什么值。**
> > 你可以把它理解成：
> > - **数据成员**：对象有哪些状态变量
> > - **这些数据成员当前的值**：对象当前处于什么状态


这一机制只读取冒号后的 `Sensor` 所建立的派生关系；`public` 对接口和转换的影响留到机制二定义，本机制不会提前要求你用它作设计判断。

```cpp
class Sensor {
public:
    explicit Sensor(int id) : id_{id} {}
    int id() const { return id_; }

private:
    int id_;
};

class Lidar : public Sensor {      // Lidar 是一种 Sensor
public:
    Lidar(int id, int beams)
        : Sensor{id}, beams_{beams} {}

private:
    int beams_;
};
```

先按构造步骤和对象状态阅读，不猜具体字节地址：

| 代码位置 | 正在建立什么 | 完成后 `Lidar` 中有什么 |
|---|---|---|
| `class Sensor` | 定义已有类型 `Sensor` | `Sensor` 对象有自己的 `id_` |
| `class Lidar : public Sensor` | 定义从 `Sensor` 派生的新类型 `Lidar` | 每个 `Lidar` 完整对象含一个 `Sensor` 基类子对象 |
| `Sensor{id}` | 构造 `Lidar` 时先初始化基类子对象 | 基类子对象保存传入编号 |
| `beams_{beams}` | 再初始化 `Lidar` 新增成员 | 完整对象还保存束数 |

普通话结论：`Lidar` 不是只保存新增的 `beams_`；它是一个完整对象，其中同时存在 `Sensor` 基类子对象和 `Lidar` 自己新增的状态。

**概念落点**：[基类与派生类（base class and derived class）](../docs/glossary/day12.md#基类与派生类base-class-and-derived-class)是这样的关系：基类是在类派生关系中被继承的类，派生类是在自身对象中包含相应基类子对象并可增加或调整成员的类。


> [!NOTE] 父类/基类？ 子类/派生类？
> [[父类_基类与子类_派生类]]


**代码与机制**：在本例中，`Sensor` 是基类，`Lidar` 是派生类。`Lidar` 构造函数中的 `Sensor{id}` 先初始化基类子对象，再初始化 `beams_`。基类私有成员 `id_` 并没有从 `Lidar` 对象中消失；只是 `Lidar` 的成员函数不能直接写 `id_`，要通过基类的公开或受保护接口使用它。`private` 是名字访问规则，不是“从对象内删除这块状态”。

这里的“包含一个基类子对象”是阅读源代码所需的语言规则。C++ 标准不要求这个子对象采用某个固定地址偏移，也不规定所有编译器使用统一对象布局；即使暂时不懂布局实现，也能判断构造顺序和成员是否存在。

**错误做法与修复**：

```cpp
class Lidar : public Sensor {
public:
    int broken_id() const {
        return id_;  // Sensor::id_ 是 private
    }
};
```

分类：**编译错误**。`id_` 仍存在于基类子对象中，但派生类成员函数不能直接命名基类的 `private` 成员。修复是使用 `Sensor` 提供的公开接口，例如 `return id();`；不要为了省一行访问函数就把数据无条件改为 `public`。

**小检查**：一个 `Lidar` 完整对象里有哪些逻辑部分？为什么 `Sensor::id_` 不能被 `Lidar` 直接命名，却不能据此说它不存在？
	包括基类sensor类的id_以及自己的成员beam_;id_在基类中是private，不能通过派生类的成员函数直接访问；


> [!NOTE] 成员函数“属于类”，但通常不说它们“存放在每一个对象里面”。
> [[C++对象中是否包含成员函数——区分类成员与对象内部存储]]

### 机制二：看懂对象结构后，再选择公有继承或组合

**实际问题**：现在才讨论设计关系。雷达可以作为传感器交给只读取编号的函数；机器人只是拥有一台雷达，并不应被当成传感器。下面两种代码关系不同：

```cpp
class Lidar : public Sensor {      // Lidar 是一种 Sensor
    // ...
};

class Robot {
public:
    Robot(int lidar_id, int beams)
        : front_lidar_{lidar_id, beams} {}

private:
    Lidar front_lidar_;             // Robot 拥有一个 Lidar
};

void print_id(const Sensor& sensor);
Lidar lidar{7, 64};
print_id(lidar);
```

| 要表达的事实 | 代码关系 | 调用方能否把完整对象当作另一类型 | 是否因此创建新对象 |
|---|---|---|---|
| `Lidar` 是一种 `Sensor` | `Lidar : public Sensor` | 可在转换可访问且无歧义时绑定 `Sensor` 指针或引用 | 绑定引用时不会 |
| `Robot` 拥有一个 `Lidar` | `Lidar front_lidar_;` | `Robot` 不会自动变成 `Lidar` | 成员随 `Robot` 构造 |

普通话结论：先用机制一确认==“派生对象确实含基类子对象”==，才能理解为什么 `Sensor&` 可以引用 `Lidar` 的基类部分；成员对象的存在则只说明“拥有”，不会建立类型替换关系。

**概念落点**：

- [公有继承（public inheritance）](../docs/glossary/day12.md#公有继承public-inheritance)是以 `public` 基类说明符建立的继承关系，它保留基类公开接口的可访问性并==允许可访问且无歧义的派生类指针或引用隐式转换为基类指针或引用==。
- [组合（composition）](../docs/glossary/day12.md#组合composition)是一个类把另一类型的对象作为成员子对象来表达“拥有或使用”关系的设计方式。

**代码与机制**：当 `print_id(lidar)` 执行时，没有创建新的 `Sensor`。形参是引用，它绑定到 `lidar` 内部的 `Sensor` 基类子对象。这个派生到基类的引用转换是语言规则；标准不要求它通过某种固定地址偏移实现。

对初学阶段的设计选择，先用一个可检验问题：所有只要求 `Sensor` 公开接口的代码，是否都应接受 `Lidar`？（如果有一个函数，它只要求传进来的东西是一个 `Sensor`：那么我们能不能自然地把 `Lidar` 传进去？）若答案是肯定的，公有继承可能合适；若只是“内部有一个、借它完成工作”，优先组合。语法只提供机制，不会自动证明业务关系正确。

今天以 `public` 继承为主。下面的表用于解释故意错误练习，不要求先背完三行：

| 写法 | 基类 `public` 在派生类中的可访问级别 | 基类 `protected` 在派生类中的可访问级别 | 类外派生到基类转换 |
|---|---|---|---|
| `: public Base` | `public` | `protected` | 通常可访问 |
| `: protected Base` | `protected` | `protected` | 通常不可访问 |
| `: private Base` | `private` | `private` | 通常不可访问 |

> [!NOTE] private 和 protected 的区别
> 在 C++ 里，`private` 和 `protected` 都是在控制“谁能访问类里的成员”，区别主要在于：
> 
> - `private`：**只有本类自己的成员函数（以及友元）能访问**
> - `protected`：**本类 + 派生类都能访问**


基类 `private` 成员在三种情况下都不能由派生类直接命名；这与机制一所说的“基类子对象仍然存在”并不矛盾。

**错误做法与修复**：为了复用 `Lidar` 的几个函数而写 `class Robot : public Lidar`，会公开宣称任意需要 `Lidar` 的位置都可把 `Robot` 当雷达使用，通常属于**有明确定义但设计关系不符合预期**。修复是把 `Lidar` 作为 `Robot` 成员，并只转发真正需要公开的操作。

[`exercises/day12/broken/private_inheritance_conversion.cpp`](../exercises/day12/broken/private_inheritance_conversion.cpp) 则用 `private` 继承后从类外尝试绑定 `Sensor&`，属于**编译错误**；若要公开表达 “Lidar is-a Sensor”，应改为 `public` 继承。

**小检查**：为什么 `Lidar` 适合公有继承 `Sensor`，而 `Robot` 通常更适合组合一个 `Lidar`？`print_id(lidar)` 会不会创建独立 `Sensor`？
	Lidar是“是一个”sensor，Robot中有一个sensor；不会，引用不会创建独立sensor；
### 机制三：按值复制基类会切掉派生新增状态（对象切片）

**实际问题**：`Lidar` 中有传感器编号和束数。如果函数按值接收 `Sensor`，调用时发生什么？

```cpp
Lidar lidar{7, 64};

void inspect_value(Sensor sensor);
void inspect_reference(const Sensor& sensor);

inspect_value(lidar);
inspect_reference(lidar);
```

两次调用的对象情况不同：

| 调用 | 是否新建参数对象 | 参数能保存什么 | 原 `lidar` 是否仍完整 |
|---|---|---|---|
| `inspect_value(lidar)` | 是，新建一个 `Sensor` | 只复制 `Sensor` 基类部分 | 是 |
| `inspect_reference(lidar)` | 否，只绑定引用 | 引用原对象中的基类部分 | 是 |

第一行不是把完整 `Lidar` 偷偷塞进小盒子，而是明确创建一个类型为 `Sensor` 的新参数对象。目标类型没有 `beams_` 成员，因此它只从 `lidar` 的基类子对象复制编号。原来的 `lidar` 没被删改，仍然保有 `beams_ == 64`。

**概念落点**：[对象切片（object slicing）](../docs/glossary/day12.md#对象切片object-slicing)是用派生类对象按值初始化或赋值基类对象时，只复制对应基类子对象而不保留派生类新增状态的现象。

**代码与机制**：也可以直接观察同样过程：

```cpp
Sensor sliced{lidar};
std::cout << sliced.id();       // 7
std::cout << lidar.beams();     // 64，原对象仍完整
```

`sliced` 从创建开始就是一个独立 `Sensor`，不是“内部还藏着 Lidar 但暂时看不到”。因此对象切片通常是**有明确定义但可能不符合设计预期**的结果，不是未定义行为。

==避免切片的常用办法是传 `const Sensor&` 或指针==，因为它们不创建基类值副本。但要精确说明边界：引用只保证完整派生对象仍在，它不会让所有同名函数自动动态选择派生版本。是否发生运行时派发还要求接口是虚函数等条件，留到 Day 13。

> [!NOTE]
> “完整对象还在” ≠ “一定调用派生类函数”。是否动态调用，要看函数是不是 `virtual`（虚函数）。

容器也会保留元素的静态类型（==静态类型 = 编译器在编译阶段就能确定的类型==）：`std::vector<Sensor>` 存放的是 `Sensor` 值，把 `Lidar` 放进去同样只得到基类值。需要保存多种派生对象时，通常使用拥有型智能指针等方案；智能指针在 Day 18 学习，不在今天提前展开。

**错误做法与修复**：

```cpp
void log_sensor(Sensor sensor) { /* ... */ }
```

如果设计目标是保留调用方完整派生对象，这段代码会发生**有明确定义但结果不符合预期的对象切片**。修复为 `const Sensor&`（只读观察）或合适的指针/所有权接口，并保证被引用对象在调用期间仍存活。不要把“修成引用”夸大成“已经自动获得运行时多态”。

**小检查**：`Sensor copied{lidar};` 会修改原来的 `lidar` 吗？`copied` 为什么没有 `beams_`？
	不会；派生类在进行按值初始化基类对象时，会发生对象切片；
### 机制四：允许通过基类指针删除时，析构接口必须匹配完整对象

**实际问题**：工厂或任务队列有时只保存 `Task*`，实际创建的是 `ScanTask`。删除时必须先结束扫描任务自己的部分，再结束共同的任务部分。

```cpp
class Task {
public:
    virtual ~Task() {
        std::cout << "destroy task\n";
    }
};

class ScanTask : public Task {
public:
    ~ScanTask() override {
        std::cout << "destroy scan task\n";
    }
};

Task* task{new ScanTask};
delete task;
```

按事件顺序读：

| 步骤 | 发生什么 |
|---:|---|
| 1 | `new ScanTask` 创建完整 `ScanTask` 对象，其中含 `Task` 基类子对象 |
| 2 | `Task*` 指向该对象的基类部分 |
| 3 | `delete task` 发现析构入口是虚函数，从完整 `ScanTask` 开始析构 |
| 4 | 先执行 `ScanTask::~ScanTask()`，再执行 `Task::~Task()`，最后完成匹配释放 |

普通话结论：指针变量只写着 `Task*`，不代表实际创建的对象只剩 `Task`。删除接口必须能沿着完整对象的类型关系执行正确析构序列。

**概念落点**：[虚析构函数（virtual destructor）](../docs/glossary/day12.md#虚析构函数virtual-destructor)是在基类中声明为 `virtual` 的析构函数，使通过基类指针删除派生类对象时按最派生对象开始执行正确析构序列。

**代码与机制**：基类析构写 `virtual` 后，派生析构也参与同一虚函数覆盖关系。`override` 不是让函数变虚所必需的，但能让编译器检查这个声明是否确实覆盖基类虚函数，推荐保留。

今天只依赖标准保证的可观察结果：合适的虚析构使常规 C++17 多态删除从完整派生对象开始。不同编译器通常用额外运行时信息实现虚调用，但 C++ 标准不要求名为 “vtable” 的固定数据结构、位置或大小；对象模型实现细节在 Day 13 严格区分。

并非每个基类都必须有公开虚析构。如果类型根本不允许外部通过基类指针删除，可以把非虚析构设为 `protected` 等方式阻止这种调用。面试中的稳妥设计表达是：用于多态删除的基类通常提供 `public virtual` 析构；不用于多态删除的基类应让这种删除在接口上不可行。

**错误做法与修复**：

```cpp
class Task {
public:
    ~Task() = default;  // 非虚
};

Task* task{new ScanTask};
delete task;
```

在这里通过基类指针删除实际为派生类型的对象，常规 C++17 规则下属于**未定义行为**。不能说它一定只漏掉派生资源、一定只打印基类日志或一定崩溃。修复是为允许多态删除的基类提供虚析构，或重新设计接口以禁止这种删除。

**小检查**：为什么 `Task*` 的拼写不能单独决定 `delete task` 应从哪个析构函数开始？
	通过基类指针~~或引用~~**（`delete` 的操作数必须是指针，不是引用）** 删除派生对象时，需要在基类中使用虚构虚函数，让析构从派生类开始，再虚构基类。

> [!NOTE]
> **`delete p`**：负责“销毁 `p` 指向的对象，并释放 `new` 申请的内存”。

## 💻 最小可运行示例

完整代码位于 [`examples/day12/main.cpp`](../examples/day12/main.cpp)，同时展示按值切片、引用不复制和虚析构顺序。

预期输出：

```text
by value id: 7
by reference id: 7
lidar beams: 64
destroy scan task
destroy task
```

前三行证明按值参数只是独立 `Sensor`，原 `Lidar` 仍保有束数；最后两行证明经 `Task*` 删除时先析构派生部分，再析构基类部分。

## 🐛 错误代码诊断

### 反例 A：私有继承后要求公开基类转换

[`exercises/day12/broken/private_inheritance_conversion.cpp`](../exercises/day12/broken/private_inheritance_conversion.cpp) 的 `Lidar` 私有继承 `Sensor`，类外调用 `print_sensor(lidar)` 时转换不可访问。分类：**编译错误**。若要公开表达 “Lidar is-a Sensor”，改用 `public` 继承；若只是实现复用，应重新评估组合。

### 反例 B：按值接收基类

`void inspect(Sensor sensor); inspect(lidar);` 可以编译并有明确定义，但参数对象只复制基类部分。分类：**有明确定义但可能不符合预期的对象切片**。修复为符合生命周期要求的引用或指针接口。

### 反例 C：通过非虚析构基类指针删除派生对象

`Task* task{new ScanTask}; delete task;` 若 `Task::~Task` 非虚，分类：**未定义行为**。后果没有固定保证。修复为允许多态删除的基类提供 `virtual` 析构，或从接口上禁止这种删除。

## 🎤 高频面试实战

先独立口述，再到[第 12 天面试答案](../interview/answers/day12.md)逐题核对全部追问。

### 题 1：公有继承与组合分别表达什么？怎样选择？

**30 秒口头回答**：公有继承表达公开的“派生类是一种基类”关系。派生对象包含基类子对象，并可在转换可访问且无歧义时把派生指针或引用当作基类指针或引用。组合是把另一个对象作为成员，表达“拥有或使用”。若调用方应把 `Lidar` 当 `Sensor`，可用公有继承；若 `Robot` 只是拥有雷达，优先组合。继承不能只按代码复用量选择。

**完整答题逻辑**：先说派生对象包含基类子对象；再解释公有派生到基类引用转换不创建新对象；用 is-a/has-a 对比业务契约；补充基类 `private` 状态存在但派生类不可直接访问；最后限定对象布局和可替换性不是语法自动保证。

**可能连续追问**：

- `class D : public B` 时，`D` 对象中是否存在 `B` 部分？
- `public`、`protected`、`private` 继承怎样影响基类成员的可访问性？
- 基类的 `private` 成员是否被派生类对象“删除”了？
- 什么情况下优先组合而不是继承？
- 派生类指针转成基类指针时是否创建新对象？

**容易失分的说法**：把继承只说成代码复用；说派生对象没有基类私有状态；认为任何继承都能在类外转成基类；把实现中的固定偏移或布局当标准保证。

### 题 2：什么是对象切片？为什么多态基类通常需要虚析构函数？

**30 秒口头回答**：对象切片发生在派生对象按值初始化或赋给基类对象时，目标只复制基类子对象，派生新增状态不进入目标；它通常有明确定义，但常不符合设计意图。引用或指针可以避免创建这个基类值副本。若接口允许通过基类指针删除实际创建的派生对象，基类需要虚析构，使析构从完整派生对象开始；否则这种删除可能是未定义行为。

**完整答题逻辑**：先比较基类值参数和 `const Base&` 是否创建新对象；说明切片不破坏原对象也不等于 UB；再画 `new Derived → Base* → delete` 的析构顺序；最后说明引用避免切片不等于自动启用普通虚函数派发，并给出 public virtual/protected nonvirtual 两类析构接口。

**可能连续追问**：

- 按值传递基类为什么会切片？
- 改成 `const Base&` 后就一定会调用派生类函数吗？
- 对象切片是未定义行为吗？
- 为什么基类析构函数通常要么 public virtual，要么 protected nonvirtual？
- 虚析构函数是否意味着每个成员函数都必须是 virtual？

**容易失分的说法**：把所有派生到基类转换都叫切片；声称切片一定崩溃；说引用本身就能让非虚函数动态派发；把非虚析构删除的后果固定描述成“只少调用一次派生析构”。

## ✍️ 当日练习

完成 [`exercises/day12/README.md`](../exercises/day12/README.md)：5 道判断、4 道对象关系与错误分析，以及一个同时使用公有继承、组合和虚析构的传感器任务小程序。答案见 [`solutions/day12/README.md`](../solutions/day12/README.md)。

## 🧠 深度思考题

为机器人系统设计 `Sensor`、`Lidar`、`Camera`、`Robot` 与 `CalibrationData` 的关系。逐一说明哪些是公有继承、哪些是组合；再选择一个只读日志函数和一个拥有任务对象的队列接口，解释怎样避免切片、怎样保证对象生命周期，以及何时需要虚析构。不要使用尚未学习的智能指针代码，只描述未来应解决的所有权问题。

## ✅ 完成标准

- 能画出派生对象中的基类子对象与新增成员，并避免把布局细节当标准保证。
- 能用 is-a/has-a 为公有继承与组合做选择。
- 能识别基类按值参数造成的对象切片，并说明引用只解决复制问题。
- 能准确分类通过非虚析构基类指针删除派生对象的风险为未定义行为。
- 能解释 public virtual 与 protected nonvirtual 析构接口各自允许或禁止什么。

规则依据：C++ 公开工作草案的 [derived classes](https://eel.is/c++draft/class.derived)、[base/derived conversions](https://eel.is/c++draft/conv.ptr) 与 [delete expressions](https://eel.is/c++draft/expr.delete) 条款，以及 C++ Core Guidelines 的 [C.35](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rc-dtor-virtual)；公开草案不是已发布 ISO 标准本身。
