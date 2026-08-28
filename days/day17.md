# 第 17 天：有序与哈希容器怎样选

预计用时：120 分钟。标准主线：C++17；`std::map`、`std::set`、`std::unordered_map` 与 `std::unordered_set` 均来自更早标准，本日只使用 C++17 可依赖的接口与保证，不使用 C++20 的 `contains`。

## 🎯 今日攻坚目标

完成本日后，你应能按正文顺序：

1. 根据“只保存唯一键”还是“由键取得对应值”选择 `std::set` 或 `std::map`，用 `find` 判断键是否存在，并预测 `map::operator[]` 对缺失键造成的状态变化；
2. 从比较器的两个方向判断有序容器是否把两个键视为等价，写出满足严格弱序的比较器，并解释有序关联容器查找的对数复杂度；
3. 沿“哈希值 → 桶 → 键相等判断”的步骤解释 `unordered_*` 查找，说明哈希冲突为何不丢数据，以及平均常量、最坏线性复杂度分别意味着什么；
4. 根据是否需要键序、范围处理、最坏复杂度保证或平均查找性能选择有序/无序容器，并判断重新散列后旧迭代器、指针与引用能否继续使用。

## 🔁 前置知识检查

1. Day 15 中，`find` 类查找返回结束迭代器时表示什么？为什么解引用结束迭代器不合法？
2. Day 15 中，谓词是什么？一个可调用对象怎样为算法提供“是否满足条件”的判断？
3. Day 16 中，复杂度描述的是什么？“摊销常量”为什么不等于每一次调用都同样便宜？
4. Day 16 中，容器元素仍然存在，为什么旧迭代器仍可能已经失效？

参考方向：查找失败要先与 `end()` 比较；谓词承担判断角色；复杂度描述工作量随 `n` 的增长关系；访问入口的有效性必须按容器和修改操作判断。今天会先从 `map`/`set` 的元素状态开始，不要求你提前知道树、哈希表或桶的实现。

## 📖 核心知识重构

今天先回答最贴近代码的问题：容器里保存的是“键”还是“键到值的对应关系”。随后才解释有序容器怎样判断先后与等价。理解这条路径后，再把“比较后缩小范围”换成“哈希后缩小候选桶”，最后统一比较复杂度、顺序保证和失效规则。

### 机制一：先区分“保存唯一键”与“用键取得值”

**实际问题**：机器人系统要保存当前激活的设备编号，还要保存每类传感器的阈值。先观察两种对象：

```cpp
#include <map>
#include <set>
#include <string>
#include <cstddef>

std::set<int> active_ids{9, 2, 9};
std::map<std::string, int> limits{{"force", 8}, {"vision", 5}};

auto found = limits.find("force");
const std::size_t before = limits.size();
int value = limits["temperature"];
```

| 时点 | `active_ids` | `limits` 的键 | 可观察结论 |
|---|---|---|---|
| 创建后 | `2, 9` | `force, vision` | 重复的 9 只保留一份；键按序迭代 |
| `find("force")` 后 | 不变 | 不变 | 找到已有元素，不插入 |
| `limits["temperature"]` 后 | 不变 | `force, temperature, vision` | 缺失键被插入，`int` 对应值为 0 |

普通话结论：只需要记录“编号是否存在”时，元素本身就是键；需要“名称对应阈值”时，一个元素同时包含键和值。`find` 只查找，`operator[]` 在键缺失时还会改变容器。

**概念落点**：[关联容器、键与 `std::map`/`std::set`（associative container, key, map and set）](../docs/glossary/day17.md#关联容器键与-stdmapstdsetassociative-container-key-map-and-set)：关联容器是根据键而不是插入位置组织并检索元素的一类标准容器；`std::set<Key>` 保存唯一的键，`std::map<Key, T>` 保存唯一键到映射值的对应关系，二者都按比较器定义的键序迭代。

本例中，`active_ids` 的键类型是 `int`；`limits` 的键类型是 `std::string`，映射值类型是 `int`。`map` 和 `set` 都是唯一键容器：再次插入容器认为等价的键，不会同时保存第二份；若业务确实允许重复键，标准库另有 `multimap`/`multiset`，本日练习先固定在唯一键模型。

**代码与机制**：先掌握查找入口：

```cpp
auto found = limits.find("force");
if (found != limits.end()) {
    std::cout << found->first << ' ' << found->second;
}
```

`find(key)` 成功时返回指向对应元素的迭代器，失败时返回 `end()`。`map` 的一个元素是 `std::pair<const Key, T>`：`pair` 是保存两个成员的标准库类型，`first` 是不能直接改写的键，`second` 是对应值。因此 `found->first` 是字符串 `force`，`found->second` 是整数 8。键不能通过迭代器随意改写，因为那会破坏容器已经建立的键序和查找关系。

只想查询时优先使用 `find`，因为它不会在失败时改变状态：

```cpp
const auto missing = limits.find("temperature");
const bool exists = missing != limits.end();  // false，limits 不变
```

`operator[]` 表达的是“取得这个键对应的值；如果没有，就先建立一个”：

```cpp
limits["force"] += 1;       // 已存在：把 8 改为 9
limits["temperature"] += 1; // 不存在：先插入 0，再改为 1
```

这对计数很方便，却不适合无副作用的存在性检查。C++17 中不能使用 C++20 才加入的 `contains`，所以判断存在仍写 `find(key) != end()`。

在默认比较器下，整数按数值升序、字符串按其比较规则迭代。`find`、单元素插入和按键删除具有对数复杂度；这里复用 Day 16 的[复杂度](../docs/glossary/day16.md#复杂度与摊销复杂度complexity-and-amortized-complexity)含义：元素数量从 `n` 增长时，查找工作量按 `log n` 的量级增长。C++17 保证行为与复杂度，但没有规定 `map`/`set` 必须由红黑树实现。

**错误做法与修复**：用下标做“只查询，不修改”的检查：

```cpp
if (limits["temperature"] == 0) {
    // 即使原来没有 temperature，这里也已经插入了它
}
```

分类：代码行为有定义，但**结果不符合“查询不改状态”的意图**。修复为先调用 `find` 并与 `end()` 比较；若业务意图就是“没有则创建默认值并累加”，再明确使用 `operator[]`。

**小检查**：`std::set<int>{7, 2, 7}` 的 `size()` 是多少？对空 `std::map<std::string, int>` 执行 `counts["force"]` 后，容器是否仍为空、返回的整数是多少？若只想判断键是否存在，为什么 `find` 更合适？

### 机制二：比较器同时决定键序与“容器眼中的相同键”

**实际问题**：业务把同一个十位区间的编号视为同组，只允许每组保留一个编号。观察自定义判断：

```cpp
struct TensGroupLess {
    bool operator()(int lhs, int rhs) const {
        return lhs / 10 < rhs / 10;
    }
};

std::set<int, TensGroupLess> ids;
ids.insert(12);
ids.insert(19);
ids.insert(25);
```

| 比较 | 结果 | 含义 |
|---|:---:|---|
| `comp(12, 19)` | `false` | 12 所在组不排在 19 所在组之前 |
| `comp(19, 12)` | `false` | 19 所在组也不排在 12 所在组之前 |
| 两个方向都为 `false` | — | 容器把 12 与 19 视为等价键 |
| `comp(12, 25)` | `true` | 十位组 1 排在十位组 2 之前 |

普通话结论：尽管 `12 == 19` 为假，这个容器仍把它们当成同一个键组；先插入的 12 已经占据该组，所以随后插入 19 不改变容器，最终只保存 12 和 25，`size()` 为 2。容器判断“是否重复”使用的就是建立顺序的那套规则。

**概念落点**：[比较器与严格弱序（comparison object and strict weak ordering）](../docs/glossary/day17.md#比较器与严格弱序comparison-object-and-strict-weak-ordering)：比较器是有序关联容器用来回答“一个键是否排在另一个键之前”的可调用对象；它必须给键建立严格弱序，而两个方向比较结果都为 `false` 的键会被容器视为等价键。

`TensGroupLess` 中的 `operator()` 是函数调用运算符成员函数；它让 `TensGroupLess` 对象可以写成 `comp(lhs, rhs)`。本日只需把它看作向容器提供一个二元判断的方式，与 Day 15 中谓词承担的可调用角色相同。

**代码与机制**：严格弱序先用四条可检查规则理解：

1. 任意键都不能排在自己之前：`comp(x, x)` 必须为 `false`；
2. 若 `comp(a, b)` 为 `true`，反向 `comp(b, a)` 必须为 `false`；
3. 若 `a` 在 `b` 前、`b` 在 `c` 前，则 `a` 必须在 `c` 前；
4. “两个方向都不在前”的等价分组也必须前后一致，不能随调用次数或可变外部状态改变。

默认的 `std::less<int>` 可直观理解为使用 `<` 建立升序。自定义多字段顺序时，要在主要字段相同后继续比较稳定的次要字段：

```cpp
struct Task {
    int priority;
    int id;
};

struct ByPriorityThenId {
    bool operator()(const Task& lhs, const Task& rhs) const {
        if (lhs.priority != rhs.priority) {
            return lhs.priority > rhs.priority; // 高优先级排前
        }
        return lhs.id < rhs.id;                 // 同优先级再按 id
    }
};
```

若省略 `id` 的第二层比较，同优先级的不同任务会成为等价键，唯一键容器只能保留其中一个。这不一定错误，关键是它是否符合业务对“同一个键”的定义。

标准保证迭代器按比较器定义的非降序访问元素，并保证普通按键查找为对数复杂度。主流实现常使用平衡搜索树来满足这些保证；“红黑树”“节点里有哪些指针”属于实现选择，不能当作 `std::map` 类型定义。

**错误做法与修复**：自定义键没有默认可用的先后关系，也没有向 `map` 提供比较器：

```cpp
struct SensorKey { int id; };
std::map<SensorKey, int> readings;
readings[{7}] = 42;
```

[`exercises/day17/broken/missing_key_order.cpp`](../exercises/day17/broken/missing_key_order.cpp) 会产生**编译错误**：默认比较器无法比较两个 `SensorKey`。修复是为键提供符合业务含义的 `<`，或像上例一样把合法的比较器类型作为 `map`/`set` 的模板参数；比较器必须满足严格弱序，不能简单把 `<=` 当作“更严格的 `<`”。

**小检查**：若 `comp(12, 19)` 与 `comp(19, 12)` 都为 `false`，唯一键 `set` 能同时保存二者吗？为什么合法比较器必须让 `comp(x, x)` 为 `false`？多字段比较省略最后的稳定区分字段可能造成什么状态？

### 机制三：哈希先缩小候选桶，键相等判断再确认目标

**实际问题**：现在不需要按编号升序遍历，只想频繁按编号查标签。为了直接观察冲突，使用一个故意简单的哈希函数：

```cpp
struct LastDigitHash {
    std::size_t operator()(int key) const {
        return static_cast<std::size_t>(key % 10);
    }
};

std::unordered_map<int, std::string, LastDigitHash> labels{
    {12, "force"},
    {22, "vision"},
};
```

| 键 | 哈希值 | 键是否相等 | 容器结果 |
|---:|---:|:---:|---|
| 12 | 2 | — | 保存 `12 -> force` |
| 22 | 2 | `12 == 22` 为 `false` | 与 12 同桶，但另存 `22 -> vision` |

普通话结论：哈希值相同只表示先到同一候选分组，不表示两个键相同。查找 22 时，容器进入相应桶后仍用键相等判断找到 22；两个对应关系都存在。

**概念落点**：本机制按依赖顺序定义两组概念。

首先，[无序关联容器（unordered associative container）](../docs/glossary/day17.md#无序关联容器unordered-associative-container)：无序关联容器是使用哈希函数和键相等判定按键组织元素、但不保证整体迭代顺序的一类标准容器；`std::unordered_set<Key>` 保存唯一键，`std::unordered_map<Key, T>` 保存唯一键到映射值的对应关系。

接着，[哈希函数、桶与哈希冲突（hash function, bucket and hash collision）](../docs/glossary/day17.md#哈希函数桶与哈希冲突hash-function-bucket-and-hash-collision)：哈希函数把键转换为 `std::size_t` 哈希值，桶是无序容器组织候选元素的分组；不同键得到同一哈希值并进入同一桶称为哈希冲突，冲突不表示两个键相等或元素被覆盖。

先由哈希函数决定去哪个候选桶，再由键相等判定确认是哪一个键，这就是两组术语的因果关系。若键相等判定认为两个键等价，哈希函数必须对它们返回相同值；反方向不成立，不同键允许同哈希值。

把示例的模板实参逐个读出来：`int` 是键类型，`std::string` 是映射值类型，`LastDigitHash` 是替代默认哈希函数的类型。若省略第三项，标准库会对 `int` 使用相应的 `std::hash<int>`；默认键相等判断按整数相等处理。本例显式提供第三项，只是为了稳定制造冲突，不要求你猜标准库默认哈希的具体结果。

**代码与机制**：`unordered_map` 与 `map` 都提供 `find`：

```cpp
const auto found = labels.find(22);
if (found != labels.end()) {
    std::cout << found->second; // vision
}
```

`labels.bucket(key)` 返回该键会进入的桶编号，范围为 `[0, bucket_count())`；它适合像本例一样观察组织状态，不替代 `find`。业务代码通常只按键查找，不需要知道桶编号。

区别在内部缩小候选范围的规则以及外部保证：

| 问题 | `map` | `unordered_map` |
|---|---|---|
| 组织依据 | 比较器建立键序 | 哈希函数、桶与键相等判断 |
| `find` 复杂度 | 对数 | 平均常量，最坏线性 |
| 全体迭代顺序 | 按比较器键序 | 不保证整体顺序 |
| 自定义键需要 | 合法比较器 | 相容的哈希函数与键相等判断 |

“平均常量”表示在哈希分布与桶负载正常时，查找工作量的平均量级不随 `n` 成比例增长；“最坏线性”表示若大量键集中到同一候选范围，某次查找可能需要检查与元素数量同量级的候选。它不是“永远 O(1)”，也不是随机抽样意义上的概率承诺。

标准规定无序关联容器有桶、同哈希值的键位于同桶，并规定接口、平均/最坏复杂度和等价关系；它不规定冲突一定用链表、开放寻址或某种固定节点布局。自定义 `LastDigitHash` 只是为了让冲突可见，真实工程应让常见键尽量均匀分布，并确保相等判定与哈希函数相容。

无序容器的遍历顺序不是升序，也不是插入顺序。即使同一程序本次恰好输出 `12, 22`，也不能把这个偶然顺序写进业务判断或稳定输出测试；需要键序时应选择有序容器或在明确的后续步骤中排序。

**错误做法与修复**：把同桶误判为同键：

```cpp
if (labels.bucket(12) == labels.bucket(22)) {
    std::cout << "same key"; // 结论错误：二者只是发生冲突
}
```

分类：程序本身有定义，但**业务判断结果错误**。修复是使用键相等规则判断键，或直接调用 `find(key)`；桶接口用于观察哈希组织，不能替代键身份。

**小检查**：哈希值相同的两个键必须相等吗？键相等判定认为等价的两个键，其哈希值可以不同吗？为什么 `unordered_map::find` 只能说平均常量，而不能承诺最坏常量？

### 机制四：重新散列只让旧迭代器失效，选择容器还要看顺序与最坏边界

**实际问题**：程序已经找到键 12，又要求容器增加桶数：

```cpp
auto old = labels.find(12);
const std::string* kept = &old->second;

labels.rehash(labels.bucket_count() + 1); // 必然提高最低桶数

// old 不再使用
auto current = labels.find(12);
std::cout << *kept << ' ' << current->second;
```

| 访问入口 | 重新散列前 | 重新散列后 | 后续安全动作 |
|---|---|---|---|
| `old` 迭代器 | 指向键 12 | 失效 | 丢弃，重新 `find` |
| `kept` 指针 | 指向现存字符串 `force` | 仍有效 | 元素未删除时可用 |
| `current` 迭代器 | 尚不存在 | 新取得且有效 | 可访问当前容器 |

普通话结论：元素没有被删除，只是重新分到新的桶；旧迭代器依赖旧的遍历/桶组织，所以失效，而指向现存元素本身的指针和引用仍保持有效。这与 `vector` 重新分配后指针、引用也全部失效的规则不同。

**概念落点**：[负载因子、重新散列与无序容器失效（load factor, rehash and unordered-container invalidation）](../docs/glossary/day17.md#负载因子重新散列与无序容器失效load-factor-rehash-and-unordered-container-invalidation)：负载因子是 `size()/bucket_count()` 表示的平均每桶元素数；重新散列（rehash）是无序容器改变桶数量并重新安排元素所属桶的过程，它会使所有旧迭代器失效，但不会使指向现存元素的指针和引用失效。

`size()` 仍是元素数量，`bucket_count()` 是桶数，`load_factor()` 返回平均每桶元素数。容器尝试让负载因子不超过 `max_load_factor()`；插入使桶内平均元素过多时，容器可能自动重新散列。

**代码与机制**：`reserve` 在这里与 Day 16 有相似名字，但准备的对象不同：

```cpp
labels.reserve(100); // 为至少容纳 100 个元素准备合适桶数，不创建元素
labels.rehash(200);  // 要求桶数至少达到 200，并满足负载约束
```

二者都可能触发重新散列。`unordered_map::reserve(100)` 不会让 `size()` 变成 100，也不能通过不存在的键迭代 100 次；它按当前最大负载因子换算所需桶数。标准不规定实际桶数必须恰好等于请求值。

高频失效规则可压成下表：

| 修改 | 旧迭代器 | 指向既有元素的指针/引用 |
|---|---|---|
| `map/set` 插入 | 保持有效 | 保持有效 |
| `map/set` 删除一个元素 | 只有指向被删元素的失效 | 只有指向被删元素的失效 |
| `unordered_*` 插入且未重新散列 | 保持有效 | 保持有效 |
| `unordered_*` 发生重新散列 | 全部失效 | 现存元素的保持有效 |
| `unordered_*` 删除一个元素 | 只有指向被删元素的失效 | 只有指向被删元素的失效 |

如果 `kept` 所指元素随后被 `erase(12)` 删除，它也会失效；“重新散列不使引用失效”不能扩大成“引用永远有效”。修改后若还要继续遍历，最简单可靠的做法是丢弃旧迭代器，从当前容器重新 `find` 或取得 `begin()`。

最终选择先写需求而不是先写容器名：

| 需求 | 通常选择 | 关键保证与代价 |
|---|---|---|
| 按键有序遍历、按键范围处理、需要对数最坏边界 | `map/set` | 常用按键操作为对数；没有平均常量承诺 |
| 不需要整体键序，重视普通按键查找的平均性能 | `unordered_map/unordered_set` | 平均常量、最坏线性；有桶开销与重新散列 |
| 只保存唯一键 | `set` 或 `unordered_set` | 不保存额外映射值 |
| 键需要对应业务值 | `map` 或 `unordered_map` | 元素含 `const Key` 与映射值 |

性能选择仍需用真实键分布、元素数量、内存约束和操作比例测量。不能仅凭“平均 O(1)”断言无序容器永远更快，也不能仅凭常见红黑树实现断言有序容器的具体内存布局。

**错误做法与修复**：重新散列后继续解引用旧迭代器：

```cpp
auto old = labels.find(12);
labels.rehash(labels.bucket_count() + 1);
std::cout << old->second; // old 已失效
```

[`exercises/day17/broken/stale_unordered_iterator.cpp`](../exercises/day17/broken/stale_unordered_iterator.cpp) 属于**未定义行为**，不保证一定崩溃，也不能因一次运行输出正确就继续使用。修复是重新散列后再次 `find`；若确实需要保持现存元素的访问，可保存指针或引用，但仍必须控制元素后续删除的生命周期。

**小检查**：`unordered_map` 重新散列后，旧迭代器、旧指针和旧引用分别怎样处理？`reserve(100)` 会创建 100 个元素吗？要求升序遍历和对数最坏查找时，为什么不应只看无序容器的平均常量复杂度？

## 💻 最小可运行示例

完整 C++17 代码位于 [`examples/day17/main.cpp`](../examples/day17/main.cpp)。它依次展示唯一键、`map::operator[]` 的插入副作用、比较器等价、可观察哈希冲突，以及重新散列后的迭代器/指针差别。

稳定输出：

```text
map lookup: 8
set size: 2
ordered ids: 2 9
subscript inserted: true
inserted value: 0
comparator-equivalent size: 2
same bucket: true
collision keeps both: force vision
pointer after rehash: force
bucket count enough: true
```

示例不遍历输出 `unordered_map` 的全部元素，因为整体迭代顺序没有稳定保证；也不输出具体桶数，因为标准只规定最低约束，不规定实现选择的精确数值。

## 🐛 错误代码诊断

### 反例 A：用 `map::operator[]` 做无副作用查询

缺失键会被插入并建立默认映射值。分类：**有定义但结果不符合预期**；修复为 `find(key) != end()`。

### 反例 B：自定义键没有可用比较器

[`exercises/day17/broken/missing_key_order.cpp`](../exercises/day17/broken/missing_key_order.cpp) 是**编译错误**。修复是提供满足严格弱序的 `<` 或比较器对象。

### 反例 C：哈希冲突表示键相等或数据覆盖

12 与 22 可得到相同哈希值并位于同桶，却仍是两个不同键。把同桶当同键属于**有定义但业务判断错误**；修复为使用键相等判断或 `find`。

### 反例 D：重新散列后旧迭代器仍可解引用

[`exercises/day17/broken/stale_unordered_iterator.cpp`](../exercises/day17/broken/stale_unordered_iterator.cpp) 使用失效迭代器，属于**未定义行为**。重新 `find`；不要照搬“指针/引用仍有效”的结论给迭代器。

## 🎤 高频面试实战

先独立口述，再到[第 17 天面试答案](../interview/answers/day17.md)逐题核对全部追问。

### 题 1：`std::map` 和 `std::set` 有什么区别？比较器为什么必须满足严格弱序？

**30 秒口头回答**：`set<Key>` 只保存唯一键，`map<Key, T>` 保存唯一键到值的对应关系；二者都由比较器决定键的迭代顺序和等价关系，普通按键查找、无提示插入和按键删除为对数复杂度。比较器必须建立严格弱序：不能让键排在自己前面，顺序和等价分组要传递且稳定。两个方向比较都为假时，容器把键视为等价，即使 `operator==` 为假。只查询 `map` 时用 `find`，因为 `operator[]` 会为缺失键插入默认值。

**完整答题逻辑**：先从元素状态区分“键集合”和“键值对应”；再说明 `map` 元素的 `first` 是不可直接修改的键、`second` 是映射值。随后用 `!comp(a,b) && !comp(b,a)` 解释等价键，并说明严格弱序是查找、唯一性和迭代顺序共同依赖的接口契约。最后给出 `find`/`operator[]` 的状态差别、对数复杂度和“标准不要求红黑树”的实现边界。

**可能连续追问**：

- `map` 的元素为什么可以看作 `pair<const Key, T>`？
- `map::operator[]` 查询不存在的键时会发生什么？
- 只想判断键是否存在时，`find` 与 `operator[]` 应怎样选择？
- `set` 再次插入等价键后会发生什么？
- 比较器判定的等价与 `operator==` 一定相同吗？
- `std::map` 是否由标准规定必须使用红黑树？

**容易失分的说法**：把 `map` 说成两个顺序数组；认为 `operator[]` 永远只读；用 `<=` 代替严格的“先于”；把等价键等同于 `==`；把红黑树实现说成类型定义。

### 题 2：`map` 与 `unordered_map` 怎样选择？哈希冲突和重新散列会带来什么影响？

**30 秒口头回答**：需要按键有序遍历、范围处理或对数最坏复杂度时选 `map`；不需要整体顺序、重视普通按键操作的平均性能时可选 `unordered_map`。无序容器先用哈希定位桶，再用键相等判断确认元素，所以冲突不会覆盖不同键。查找平均常量、最坏线性。负载升高可能触发重新散列；它使全部旧迭代器失效，但不使指向现存元素的指针和引用失效，删除元素时相应入口仍会失效。

**完整答题逻辑**：先列出顺序、范围和最坏边界是否是业务要求，再比较对数与平均常量/最坏线性。随后用“哈希值 → 桶 → 相等判断”解释冲突，强调等价键必须同哈希而同哈希不必等价。最后说明负载因子、`reserve`/`rehash`、整体遍历顺序和三类访问入口的失效差异，并把桶内具体布局留在实现边界。

**可能连续追问**：

- 哈希冲突是否意味着一个元素会覆盖另一个元素？
- 键相等判定认为等价的两个键，对哈希函数有什么要求？
- `unordered_map` 的遍历顺序能否当作插入顺序或稳定顺序？
- 负载因子、`reserve` 与 `rehash` 分别表示什么？
- 重新散列后，旧迭代器、指针和引用分别是否有效？
- 哪些需求会让你明确选择 `map` 而不是 `unordered_map`？

**容易失分的说法**：把平均 O(1) 说成最坏 O(1)；认为哈希绝不能冲突；依赖一次运行的遍历顺序；说重新散列会像 `vector` 一样使所有指针引用失效；只比较复杂度而不说明键序需求。

## ✍️ 当日练习

完成 [`exercises/day17/README.md`](../exercises/day17/README.md)：6 道判断、5 道容器状态/比较器/哈希/失效分析，以及一个同时使用有序与无序关联容器的小程序。答案与参考实现位于 [`solutions/day17/README.md`](../solutions/day17/README.md)，请先画出键、映射值、哈希值、桶和访问入口状态，再查看答案。

## 🧠 深度思考题

机器人系统有三份数据：需要按时间戳顺序扫描的状态快照、只需按设备 ID 高频查找的在线状态、需要去重并按报警等级顺序输出的报警键。分别选择 `map/set/unordered_map/unordered_set` 中的容器，说明键和值是什么、为何需要或不需要键序、查找的平均/最坏边界，以及一次批量插入触发重新散列后哪些长期保存的访问入口必须更新。如果自定义设备 ID 的“相等”忽略某个字段，哈希函数必须怎样同步设计？

## ✅ 完成标准

- 能从元素状态区分 `set` 与 `map`，用 `find` 做无副作用查询，并预测 `operator[]` 对缺失键的插入。
- 能用两个方向的比较结果判断等价键，写出稳定、传递且自比较为假的比较器。
- 能沿哈希值、桶和键相等判断解释一次查找，说明冲突不丢数据以及平均常量、最坏线性的边界。
- 能根据是否需要键序、范围与最坏保证选择有序或无序容器，不依赖无序遍历顺序或固定底层实现。
- 能判断重新散列及删除后的迭代器、指针和引用状态，并从修改后的容器重新取得安全迭代器。

规则依据：C++ 公开工作草案的 [associative container requirements](https://eel.is/c++draft/associative.reqmts)、[`map` overview](https://eel.is/c++draft/map.overview)、[`set` overview](https://eel.is/c++draft/set.overview)、[unordered associative container requirements](https://eel.is/c++draft/unord.req) 与 [`unordered_map` overview](https://eel.is/c++draft/unord.map) 条款；本日只采用 C++17 已具备的接口与保证，公开工作草案不是已发布的 ISO C++17 标准文本。
