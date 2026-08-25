# 第 15 天：STL 如何把数据与操作解耦

预计用时：120 分钟。标准主线：C++17；Lambda 表达式来自 C++11；C++20 新增的连续迭代器与 Ranges 只作为补充，不进入本日构建目标。

## 🎯 今日攻坚目标

完成本日后，你应能按正文顺序：

1. 沿一个普通数组移动起点位置，解释半开区间 `[first, last)` 包含哪些元素，以及为什么结束位置不能解引用；
2. 把手写查找循环改成 `std::find`，检查返回位置后再使用结果，并说明算法是否拥有或修改本例数据；
3. 分别把普通函数和 Lambda 传给 `std::count_if`，区分谓词的角色与 Lambda 的写法，并预测值捕获、引用捕获的结果；
4. 根据迭代器支持的操作判断 `std::find` 与 `std::sort` 能否使用，定位“输入迭代器不能排序”的编译错误。

## 🔁 前置知识检查

1. Day 7 中，数组 `int values[]{4, 9, 2};` 有几个元素？表达式 `values + 1` 指向哪个元素？
2. Day 3 中，手写循环查找目标值时，循环条件和 `break` 分别控制什么？
3. Day 4 中，把普通函数名作为另一个函数的实参之前，至少需要知道该函数接收什么参数、返回什么结果吗？
4. Day 14 中，函数模板只在具体类型支持模板体所需操作时才能成功实例化；这条规则会不会因为函数位于标准库中就消失？

参考方向：数组元素连续排列；循环条件决定是否继续，`break` 提前结束；作为参数传入的操作仍要有匹配的调用形式；标准库算法也是模板，也有明确的类型与操作要求。这里尚未要求你知道“迭代器、谓词、Lambda”的定义，它们会在对应机制中从代码状态开始学习。

## 📖 核心知识重构

今天只沿一条依赖链前进：先把“从哪里开始、到哪里停止”看清楚；再把这个区间交给通用算法；算法需要自定义判断时，先认识判断角色，再学习 Lambda 写法；最后才比较不同位置对象的能力，解释为什么有些算法不能接收它们。

### 机制一：先用两个位置划定要处理的元素

**实际问题**：我们想依次读取数组中的三个值，但不希望把数组长度 `3` 写进每一次操作。先观察 `first` 怎样移动：

```cpp
#include <iterator>

int values[]{4, 9, 2};
auto first = std::begin(values);
auto last = std::end(values);

int a = *first;  // 4
++first;
int b = *first;  // 9
```

`std::begin(values)` 给出数组起始位置，`std::end(values)` 给出越过最后一个元素的结束位置。在这个普通数组例子中，两者的类型都是 `int*`。先按状态看，不急着推广：

| 时点 | `first` 表示的位置 | `*first` | 是否到达 `last` |
|---|---|---:|:---:|
| 创建后 | 下标 0 | 4 | 否 |
| 一次 `++first` 后 | 下标 1 | 9 | 否 |
| 三次 `++first` 后 | 结束位置，不代表元素 | 不可使用 | 是 |

普通话结论：`first` 当前指到哪个元素，就能用 `*first` 读取哪个元素；每次 `++first` 向后移动一个位置。当 `first == last` 时，已经没有待处理元素，应停止，而不是读取 `*last`。

**概念落点**：[迭代器与半开区间（iterator and half-open range）](../docs/glossary/day15.md#迭代器与半开区间iterator-and-half-open-range)：迭代器是表示序列中某个位置、并按其能力支持访问或移动的对象；半开区间 `[first, last)` 从 `first` 开始，包含到 `last` 之前的元素，但不包含 `last`。

定义中的“位置”对应当前 `first`；“访问”对应 `*first`；“移动”对应 `++first`。本例区间包含下标 0、1、2，不包含 `last`。`last` 不是“最后一个元素”的别名，而是“处理到这里就停”的边界。

**代码与机制**：用最小循环完整走过区间：

```cpp
for (auto current = std::begin(values);
     current != std::end(values);
     ++current) {
    std::cout << *current << ' ';
}
```

每轮都先检查 `current != last`，确认当前位置仍代表元素，才执行 `*current`。半开区间有两个直接好处：空区间可以写成 `first == last`；相邻两段可以写成 `[first, middle)` 和 `[middle, last)`，既不遗漏也不重复 `middle` 所在的元素。

本例的数组迭代器恰好是裸指针，但“迭代器”描述的是对象能提供的位置访问与移动能力，并不要求所有迭代器都是指针，也不保证都支持 `current + 5`。机制四会在已有最小模型上比较这些能力。

C++17 标准规则关心的是位置、有效区间以及允许的操作，不规定迭代器必须用某种内存布局实现。对于数组，结束指针可以用于比较和区间边界，却不能解引用。数组仍然拥有元素；`first`、`last` 只是访问位置，不会延长数组的生命周期。

**错误做法与修复**：下面循环在 `current == last` 时仍执行一次循环体：

```cpp
for (auto current = std::begin(values); ; ++current) {
    std::cout << *current << ' ';  // 最终会尝试解引用 last
    if (current == std::end(values)) {
        break;
    }
}
```

分类：解引用结束位置是**未定义行为**。未定义行为表示 C++ 标准不约束结果，不等于保证崩溃或保证出现段错误。修复是把 `current != last` 放在解引用之前检查，保持“先验证仍在区间内，再读取”的顺序。

**小检查**：若 `first` 初始指向下标 0，`last` 是结束位置，循环为什么正好读取三次？当 `first == last` 时，对 `first` 还能做什么，不能做什么？

### 机制二：算法接收区间，把查找步骤从数据表示中抽离

**实际问题**：有了 `[first, last)` 后，手写查找循环仍然会重复“比较、移动、停止”的控制代码：

```cpp
int values[]{4, 9, 2};
auto first = std::begin(values);
auto last = std::end(values);

auto found = first;
while (found != last && *found != 9) {
    ++found;
}
```

标准库可以替我们执行同样的遍历：

```cpp
auto found = std::find(first, last, 9);
if (found != last) {
    std::cout << *found << '\n';
}
```

先看调用结束后的两种状态：

| 输入目标 | `found` 的位置 | `found != last` | 能否解引用结果 |
|---:|---|:---:|:---:|
| 9 | 下标 1 | 是 | 能，得到 9 |
| 7 | `last` | 否 | 不能 |

普通话结论：查找过程不再由我们手写，但查找失败的可能性没有消失。`std::find` 用返回位置表达结果；找到时返回相应元素的位置，没找到时返回传入的 `last`。

**概念落点**：[标准算法（standard algorithm）](../docs/glossary/day15.md#标准算法standard-algorithm)：标准算法是标准库中通过迭代器区间等参数执行查找、统计、排序或复制等通用操作的函数模板；算法本身不拥有所处理的数据。

本例中的 `std::find` 是 `<algorithm>` 提供的标准算法。它接收 `first`、`last` 和目标值 `9`，对 `[first, last)` 逐个比较，并返回位置。这里“解耦”不是说数据与操作毫无关系，而是说算法不必把参数写成“只能处理这个具体数组”；只要传入的类型满足它所要求的读取、移动和比较操作，就能实例化相应版本。

**代码与机制**：把手写循环与算法逐项对应：

| 手写动作 | `std::find` 中的对应规则 |
|---|---|
| 从 `first` 开始 | 第一个参数 |
| 到 `last` 前停止 | 第二个参数，区间仍是 `[first, last)` |
| 比较 `*current == 9` | 第三个参数提供目标值 |
| 找到后保存当前位置 | 返回匹配元素的迭代器 |
| 全部不匹配 | 返回 `last` |

`std::find` 不修改本例数组；`std::sort` 则会重排区间中的元素，所以不能把“标准算法”一概说成只读。应分别查清每个算法的输入、修改效果和返回值。两者都不拥有数组：数组先失效，指向它的迭代器也不能继续用于访问。

算法接收的是一个有效区间。调用者必须保证 `first` 与 `last` 能按该算法允许的移动方式描述同一段可达序列；把互不相关的数据位置硬拼成区间，不会得到自动边界检查。违反算法前置条件时，不能依赖某次运行“看起来正常”。

**错误做法与修复**：直接解引用查找结果：

```cpp
auto found = std::find(first, last, 7);
std::cout << *found << '\n';  // 目标不存在时 found == last
```

分类：若没找到，解引用 `last` 是**未定义行为**。修复是先检查 `found != last`，并为未找到分支决定清楚的业务行为。不要把“算法调用成功”误解成“目标一定存在”。

**小检查**：把目标从 `9` 改成 `7` 后，`std::find` 返回什么？它是否创建了数组副本、是否修改了数组、是否让数组活得更久？

### 机制三：先确定判断角色，再用函数或 Lambda 提供判断

**实际问题**：`std::find` 比较一个固定目标；现在要统计偶数，算法需要对每个元素询问“它是否满足条件”。先用已经学过的普通函数提供答案：

```cpp
bool is_even(int value) {
    return value % 2 == 0;
}

int values[]{4, 9, 2};
auto count = std::count_if(
    std::begin(values), std::end(values), is_even);
// count 为 2
```

`std::count_if` 依次拿出 4、9、2，像调用普通函数一样执行 `is_even(value)`：

| 当前元素 | 调用 | 结果按 `bool` 判断 | 是否计数 |
|---:|---|:---:|:---:|
| 4 | `is_even(4)` | `true` | 是 |
| 9 | `is_even(9)` | `false` | 否 |
| 2 | `is_even(2)` | `true` | 是 |

普通话结论：算法负责遍历和计数，调用方提供“什么叫满足条件”。这里还没有 Lambda，已经能完成任务，因此“条件判断的角色”必须先与“某种便捷语法”分开。

**概念落点**：本机制有两个术语，依赖顺序是“算法需要怎样的判断”在前，“怎样就地写出这个判断”在后。

首先，[谓词（predicate）](../docs/glossary/day15.md#谓词predicate)：谓词是能被调用，并针对给定实参返回可按 `bool` 判断结果的可调用对象；算法用它决定某个元素是否满足条件或两个元素的相对关系。

`is_even` 接收一个 `int`，返回 `bool`，所以在这次 `std::count_if` 调用中充当一元谓词。“谓词”是角色和要求，不是只能用某个关键字声明的独立类型。

接着，[Lambda 表达式与捕获（lambda expression and capture）](../docs/glossary/day15.md#lambda-表达式与捕获lambda-expression-and-capture)：Lambda 表达式是在使用处创建简单函数对象的表达式；捕获列表规定 Lambda 函数体能以值副本或引用使用哪些外层局部对象。

若判断还依赖当前阈值，可以在使用处创建一个简单函数对象：

```cpp
int threshold = 5;
auto by_value = [threshold] (int value) {
    return value >= threshold;
};

threshold = 8;
bool first_result = by_value(7);  // true，副本仍是 5

auto by_reference = [&threshold] (int value) {
    return value >= threshold;
};
bool second_result = by_reference(7);  // false，引用看到 8
```

**代码与机制**：按创建与调用时点拆开看：

| 对象 | 创建时保存什么 | 外层 `threshold` 改成 8 后，检查 7 | 原因 |
|---|---|:---:|---|
| `by_value` | 值 5 的副本 | `true` | 使用自己的副本 5 |
| `by_reference` | 外层对象的引用 | `false` | 调用时读取外层对象当前值 8 |

捕获列表 `[]` 写在 Lambda 最前面；参数列表 `(int value)` 表示调用时从算法接收一个元素；函数体返回判断结果。`[threshold]` 明确按值捕获这个名字，`[&threshold]` 明确按引用捕获。初学阶段优先写出具体名字，比宽泛的 `[=]` 或 `[&]` 更容易审计依赖和生命周期。

值捕获与引用捕获都来自 C++11。值捕获保存副本，因此外层对象之后改变不会同步改变副本；引用捕获能观察外层对象的变化，但不会延长外层对象的生命周期。若 Lambda 在 `threshold` 生命周期结束后仍被保存和调用，引用捕获会悬空；修复方向通常是按值捕获所需数据，或保证被引用对象覆盖 Lambda 的全部使用期。

Lambda 创建的函数对象可以充当谓词，但不是每个 Lambda 都是谓词：参数或返回结果不满足算法要求时就不行；普通函数 `is_even` 也能充当谓词，所以谓词也不等于 Lambda。

**错误做法与修复**：[`exercises/day15/broken/missing_lambda_capture.cpp`](../exercises/day15/broken/missing_lambda_capture.cpp) 在捕获列表 `[]` 为空时直接使用外层局部变量 `threshold`。分类：**编译错误**，因为该名字需要捕获却没有被捕获。修复是根据所需状态选择 `[threshold]` 或 `[&threshold]`，并同时检查引用捕获的生命周期，而不是机械地把所有变量都写成 `[&]`。

**小检查**：为什么 `is_even` 与 `[threshold] (int value) { return value >= threshold; }` 都能交给 `std::count_if`？若创建 Lambda 后把外层 `threshold` 从 5 改成 8，值捕获和引用捕获检查 7 各返回什么？

### 机制四：算法只接受能力达到要求的迭代器

**实际问题**：机制一的数组迭代器既能 `++first`，也能写 `first + 2`。现在从输入流中逐个读取整数，先只观察这个位置对象能做什么：

```cpp
#include <iterator>
#include <sstream>

std::istringstream input{"8 3 5"};
std::istream_iterator<int> input_first{input};
std::istream_iterator<int> input_last{};

auto found = std::find(input_first, input_last, 3);  // 可以
// std::sort(input_first, input_last);               // 编译错误

int values[]{8, 3, 5};
std::sort(std::begin(values), std::end(values));     // 可以
```

`std::istream_iterator<int>` 在这里可以先理解为“每次递增就继续读取格式化整数的位置对象”，不要求你记住它的全部接口。对比两个例子实际提供的能力：

| 操作能力 | 输入流迭代器 | 数组中的 `int*` |
|---|:---:|:---:|
| 读取当前位置 `*it` | 能 | 能 |
| 向前一步 `++it` | 能 | 能 |
| 后退 `--it` | 不能 | 能 |
| 跳到 `it + n`、求位置差 | 不能 | 能 |
| 多次独立遍历同一输入 | 不保证 | 能 |

普通话结论：`std::find` 只需从前向后读到匹配项，因此两者都够用；`std::sort` 需要在区间内快速跳转、比较不同位置并交换元素，输入流位置缺少这些能力，所以编译阶段就不能形成有效调用。

**概念落点**：[迭代器类别（iterator category）](../docs/glossary/day15.md#迭代器类别iterator-category)：迭代器类别是按迭代器支持的遍历、读写和跳转能力及相应保证划分的能力等级；算法通过类别要求规定可接受的最低迭代器能力。

类别不是给运行速度贴一个模糊标签，而是描述代码可以合法使用哪些操作以及必须获得哪些保证。算法声明最低能力要求，使同一算法可以接收多种具体迭代器，又能拒绝能力不足的调用。

**代码与机制**：C++17 主线可先用下面的能力梯度阅读：

| 类别 | 初学者先记住的能力 | 最近增加的能力或角色 |
|---|---|---|
| Input iterator | 单向读取，至少能 `*it`、`++it` | 可用于一次向前读取 |
| Output iterator | 单向写入 | 主要承担输出角色，不是 Input 的简单“更强一级” |
| Forward iterator | 可多遍地单向访问 | 同一位置可用于可靠的多次遍历 |
| Bidirectional iterator | 在 Forward 基础上可 `--it` | 能向后一步 |
| Random-access iterator | 可常量时间跳转、求距离和按位置比较 | 支持 `it + n`、`it[n]` 等 |

能力关系中，Forward、Bidirectional、Random-access 可以看成逐步增加遍历能力；Output 主要描述写入目的，不要强行把五个名称背成一条单线等级。

`std::find` 的 C++17 版本要求 Input iterator，因为它只需读取、比较和向前移动。`std::sort` 要求 Random-access iterator，并且区间元素还要满足交换、移动和比较等相应要求；“类别够了”不代表其他类型要求自动成立。数组的 `int*` 支持随机访问，因此可以排序；`std::istream_iterator<int>` 只提供输入能力，不能用于 `std::sort`。

下面是 **C++20 补充**：C++20 在传统类别体系上增加了 Contiguous iterator，用于表达逻辑相邻元素在存储中也连续，并引入 Ranges 算法与 iterator/sentinel 的更完整模型。本课程的 C++17 主线不要求把它塞进本日五类表，也不会用 C++20 规则反推今天的代码。

C++17 标准规定算法与迭代器必须满足的可观察操作和复杂度要求；某个编译器如何展开模板、是否把循环向量化，是实现选择。删除这些实现细节，也不影响本日判断：输入迭代器可供 `find` 使用，却不满足 `sort` 的随机访问要求。

**错误做法与修复**：[`exercises/day15/broken/sort_input_iterator.cpp`](../exercises/day15/broken/sort_input_iterator.cpp) 把两个 `std::istream_iterator<int>` 传给 `std::sort`。分类：**编译错误**，因为算法实例化时需要随机访问操作，输入迭代器没有提供。修复不是强制转换迭代器，而是先把输入读入支持所需操作的数据结构后再排序；相应顺序容器将在 Day 16 正式学习。

**小检查**：为什么 `std::find(input_first, input_last, 3)` 能成立，而 `std::sort(input_first, input_last)` 不成立？把输入流迭代器换成数组的 `int*` 后，哪项新增能力使 `sort` 的迭代器要求得到满足？

## 💻 最小可运行示例

完整 C++17 代码位于 [`examples/day15/main.cpp`](../examples/day15/main.cpp)。它按今天的依赖顺序展示数组迭代器和半开区间、`std::find`、普通函数/Lambda 谓词、输入迭代器与排序要求。

关键调用：

```cpp
const auto found = std::find(first, last, 7);
const auto at_least_threshold = [threshold] (int value) {
    return value >= threshold;
};
std::count_if(first, last, at_least_threshold);
std::sort(first, last);
```

稳定输出：

```text
first: 4
found 7: true
at least 5: 2
even: 2
stream found 3: true
sorted: 1 2 4 7 9
```

## 🐛 错误代码诊断

### 反例 A：把结束位置当作最后一个元素

`last` 标记 `[first, last)` 的结束，不属于区间。对它解引用是**未定义行为**；标准不保证一定崩溃。修复是始终在解引用前检查当前位置不等于 `last`。

### 反例 B：`std::find` 调用返回就代表一定找到

未找到时返回的正是 `last`。不检查就执行 `*found` 可能产生**未定义行为**。修复是先分支判断 `found != last`，再读取结果。

### 反例 C：Lambda 函数体可以直接读取任意外层局部变量

[`exercises/day15/broken/missing_lambda_capture.cpp`](../exercises/day15/broken/missing_lambda_capture.cpp) 的空捕获列表没有提供 `threshold`。分类：**编译错误**。按是否需要独立副本或共享当前对象选择显式值捕获或引用捕获。

### 反例 D：只要有 `first` 和 `last`，任何算法都能调用

[`exercises/day15/broken/sort_input_iterator.cpp`](../exercises/day15/broken/sort_input_iterator.cpp) 使用的输入迭代器不支持随机访问。分类：**编译错误**。先查算法要求，再选择能力足够的迭代器和数据结构。

## 🎤 高频面试实战

先独立口述，再到[第 15 天面试答案](../interview/answers/day15.md)逐题核对全部追问。

### 题 1：迭代器和半开区间怎样让标准算法与数据结构解耦？

**30 秒口头回答**：迭代器表示序列中的位置，并按其类别提供读取、移动等操作；`[first, last)` 包含 `first` 到 `last` 之前的元素，`last` 只标记结束，不能作为元素解引用。标准算法通过这对位置遍历数据，不拥有数据，也不必把接口绑定到某个具体数据结构。以 `std::find` 为例，找到时返回匹配位置，没找到时返回 `last`，调用方必须检查后再解引用。

**完整答题逻辑**：先用数组中的 `int*` 演示 `*first`、`++first` 和 `first == last`，说明区间边界；再把手写查找的开始、停止、比较和返回位置映射到 `std::find`。随后说明解耦仍有条件：区间必须有效，迭代器和元素要满足算法要求，数据生命周期仍由原对象管理。最后区分只读 `find` 与会重排元素的 `sort`，避免断言所有算法都不修改数据。

**可能连续追问**：

- 迭代器一定是裸指针吗？
- 为什么标准算法常用 `[first, last)` 而不是把 `last` 作为最后一个元素？
- `std::find` 没找到目标时返回什么？
- 标准算法会拥有或自动延长输入数据的生命周期吗？
- 解引用 past-the-end 迭代器一定会崩溃吗？

**容易失分的说法**：把所有迭代器说成指针；把 `last` 当最后一个元素；忘记检查 `find` 结果；声称算法会保存数据或自动检查任意边界；把未定义行为说成保证崩溃。

### 题 2：谓词和 Lambda 有什么关系？为什么 `std::sort` 不是任何迭代器都能用？

**30 秒口头回答**：谓词是能接收算法给定实参并返回可按 `bool` 判断结果的可调用对象，是算法所需的角色；普通函数、函数对象或合适的 Lambda 都可以充当谓词。Lambda 是在使用处创建函数对象的表达式，捕获决定怎样使用外层局部对象。算法还规定迭代器最低能力：`find` 只需输入迭代器的单向读取，`sort` 需要随机访问以及元素交换、移动和比较等要求，所以输入流迭代器不能直接排序。

**完整答题逻辑**：先让普通函数 `is_even` 充当 `count_if` 谓词，证明谓词不等于 Lambda；再用 `[threshold]` 和 `[&threshold]` 比较副本与引用在外层值改变后的结果，并说明引用生命周期。最后列出输入与随机访问迭代器的操作差异，把 `find`、`sort` 各自需要的动作映射过去，同时保留元素类型的额外要求和 C++17/C++20 类别边界。

**可能连续追问**：

- 谓词必须由 Lambda 编写吗？
- `[threshold]` 与 `[&threshold]` 的可观察区别是什么？
- Lambda 使用外层局部变量却没有捕获会怎样？
- 为什么 `std::find` 可以接收输入迭代器，而 `std::sort` 要求随机访问迭代器？
- C++20 的连续迭代器是 C++17 五类主线中的一类吗？

**容易失分的说法**：把谓词说成 Lambda 的同义词；混淆值捕获与引用捕获；认为引用捕获会延长生命周期；认为所有迭代器支持随机跳转；只检查类别而忘记元素操作要求。

## ✍️ 当日练习

完成 [`exercises/day15/README.md`](../exercises/day15/README.md)：6 道判断、5 道区间/算法/捕获/类别分析，以及一个把手写循环改写为标准算法的小程序。答案与参考实现位于 [`solutions/day15/README.md`](../solutions/day15/README.md)，请先画出 `first`、返回位置和捕获状态，再查看答案。

## 🧠 深度思考题

一组传感器读数既要查找错误码，也要统计超过动态阈值的值，最后可能需要排序。请为三个操作分别选择算法、区间和谓词写法，并说明：哪个操作不修改数据、哪个会重排数据；阈值应按值还是按引用捕获；若数据来自只能读取一次的输入流，哪些操作能直接完成，哪些必须先把数据保存到支持相应迭代器能力的数据结构中。

## ✅ 完成标准

- 能沿 `[first, last)` 逐步指出当前元素与结束条件，并明确 `last` 不可解引用。
- 能用 `std::find` 替代手写查找循环，区分找到与未找到状态后安全读取结果。
- 能说明谓词是角色、Lambda 是一种创建函数对象的写法，并预测值捕获与引用捕获。
- 能从所需操作判断 `find` 与 `sort` 的迭代器要求，识别能力不足导致的编译错误。
- 能说明算法不拥有数据、不同算法是否修改数据要分别判断，并保留 C++17 与 C++20 边界。

规则依据：C++ 公开工作草案的 [iterator requirements](https://eel.is/c++draft/iterator.requirements)、[algorithm requirements](https://eel.is/c++draft/algorithms.requirements)、[`find`](https://eel.is/c++draft/alg.find)、[`sort`](https://eel.is/c++draft/alg.sort) 与 [Lambda expressions](https://eel.is/c++draft/expr.prim.lambda) 条款，以及 C++ Core Guidelines 的 [Standard library](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#S-stdlib) 部分；公开工作草案不是已发布 ISO 标准本身。
