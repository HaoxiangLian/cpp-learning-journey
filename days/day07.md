# 第 7 天：连续数据、字符串与手动资源管理

预计用时：120 分钟。标准主线：C++17。

## 🎯 今日攻坚目标

能够安全遍历固定大小数组，区分数组与指针、C 风格字符串与 `std::string`，并准确配对 `new/delete`、`new[]/delete[]` 与 `malloc/free`，识别越界、泄漏、释放后使用和错误释放方式。

## 🔁 前置知识检查

先独立回答，再回看第 5—6 天：

1. 指针对象与所指对象可以具有不同存储期吗？举一个例子。
2. 为什么 `pointer != nullptr` 仍不足以证明解引用安全？
3. 对象生命周期结束后，通过悬空指针访问它属于哪类错误？是否保证发生段错误？

参考方向：局部指针可以指向静态或动态对象；还需证明类型、边界和生命周期；释放后访问属于未定义行为，不保证固定症状。

## 📖 核心知识重构

今天把单个对象扩展为连续元素序列。原生数组和手动分配是理解旧代码与面试追问的必修内容，但现代工程会优先使用能保存长度并自动管理资源的标准库类型。

### 机制一：数组拥有固定数量的连续元素，但大多数表达式会把数组转换为首元素指针

**实际问题**：三个传感器读数需要按下标访问。为什么 `sizeof(readings)` 能得到整个数组字节数，而把它传给函数后却无法自动知道元素数量？因为数组与指针不是同一种类型，只是在许多表达式中发生了转换。

**概念落点**：[数组与数组到指针转换（array and array-to-pointer conversion）](../docs/glossary/day07.md#数组与数组到指针转换array-and-array-to-pointer-conversion)：数组是由固定数量、同一元素类型的子对象连续组成的对象；在多数表达式中，数组表达式会转换为指向首元素的指针值，但数组类型并不因此等同于指针类型。

```cpp
int readings[3]{12, 15, 18};
```

`readings` 是“array of 3 int”，含有下标 `0`、`1`、`2` 三个连续元素。C++17 普通数组的显式界限必须是转换为 `std::size_t` 的常量表达式且大于零；某些编译器支持变长数组是扩展，不能写成可移植 C++17 规则。

**代码与机制**：

```cpp
constexpr std::size_t count{sizeof(readings) / sizeof(readings[0])};

for (std::size_t index{0}; index < count; ++index) {
    std::cout << readings[index] << '\n';
}
```

内建下标 `readings[index]` 与相应的指针加法再解引用相关；语言不会自动为原生数组的每次下标访问执行范围检查。合法元素是 `[0, count)`，读取 `readings[count]` 已越过尾部。

数组到指针转换解释了函数形参陷阱：

```cpp
void process(int values[3]);  // 函数类型中会调整为 int* values
```

形参里的 `3` 不会让函数自动携带或运行时检查长度。应把长度作为独立参数，或在现代接口中使用保存范围信息的类型；`std::span` 是高频 C++20 补充，C++17 项目可用容器、迭代器区间或项目约定的 view 类型。

`sizeof(array)` 在数组仍保持数组类型的上下文中计算整个数组对象大小；`sizeof(pointer)` 只计算指针对象大小。C++17 的 `std::size(array)` 也能在实际数组仍可见时取得元素数量。

**错误做法与修复**：

```cpp
int readings[3]{12, 15, 18};
int invalid{readings[3]};  // 越界读取
```

分类：**未定义行为**。内存相邻位置看似可读不代表那里是数组元素，也不能预测固定“垃圾值”或崩溃。修复是让边界条件严格使用 `index < count`，并优先使用能够携带长度、提供更安全接口的标准库类型。

**小检查**：为什么 `int values[10]` 与 `int* values` 在普通对象声明中是不同类型，却在函数形参列表中可能表示相同的调整后参数类型？

### 机制二：C 风格字符串依赖终止空字符，`std::string` 自己管理长度与存储

**实际问题**：`char label[3]{'i', 'm', 'u'};` 看起来包含三个字符，为什么不能安全交给要求 C 字符串的函数？因为字符数组不一定是以空字符终止的字符串。

**概念落点**：

- [C 风格字符串（C-style string）](../docs/glossary/day07.md#c-风格字符串c-style-string)是以值为零的字符 `\0` 作为终止标记的字符序列；使用它的接口依赖在可访问范围内找到该终止字符。
- [`std::string`](../docs/glossary/day07.md#stdstring)是标准库提供的字符串类，它作为对象管理字符序列、当前长度和所需存储，并提供拼接、查询和比较等成员操作。

字符串字面量 `"imu"` 的类型是含终止字符的 `const char` 数组；用它初始化可修改字符数组时，数组大小为 4：

```cpp
char label[]{"imu"};  // {'i', 'm', 'u', '\0'}
std::string name{"imu"};
```

**代码与机制**：C 字符串接口没有从一个 `const char*` 值中自动恢复合法范围的通用办法，它依赖约定的 `\0`。`std::strlen(label)` 返回终止字符之前的字符数量，不包含 `\0`，前提是 `label` 确实指向一个有效 C 字符串。

`std::string` 保存自己的 `size()`，可以自然拼接：

```cpp
std::string task{"inspect"};
task += "-done";
std::cout << task.size() << '\n';
```

内部容量、small string optimization 或具体指针布局都不是 C++17 对 `std::string` 的统一保证。今天只依赖公开接口，不根据某个编译器对象布局写代码。

**错误做法与修复**：

```cpp
char label[3]{'i', 'm', 'u'};  // 没有 '\0'
std::cout << std::strlen(label); // 继续越界寻找终止符
```

分类：若调用要求 C 字符串却在数组范围内没有终止字符，继续读取越界会产生**未定义行为**。修复为给数组留出终止字符，或直接使用 `std::string label{"imu"};`。不要说“编译器会自动补 `\0`”——只有相应初始化形式和足够数组大小才会包含它。

**小检查**：`sizeof("imu")` 与 `std::string{"imu"}.size()` 在常见 C++17 字符编码下分别是多少？多出来的一个元素是什么？

### 机制三：`new` 创建动态对象，`delete` 必须匹配创建形式并只执行一次

**实际问题**：元素数量直到运行时才知道时，旧式代码会手动申请数组。如果某条返回路径忘记释放、用错 `delete` 形式或释放后继续访问，资源与对象生命周期就失去控制。

**概念落点**：[`new` 与 `delete` 表达式（new-expression and delete-expression）](../docs/glossary/day07.md#new-与-delete-表达式new-expression-and-delete-expression)是创建动态存储期对象并结束相应对象生命周期、释放其存储的语言机制；单对象形式与数组形式必须按来源匹配。

```cpp
int* one{new int{7}};
delete one;

int* many{new int[3]{2, 4, 6}};
delete[] many;
```

`new int{7}` 尝试取得合适存储并初始化一个 `int`，结果是指向该对象的指针。普通抛出形式的 `new` 分配失败时抛出与 `std::bad_alloc` 匹配的异常，而不是返回空指针；异常与 `std::nothrow` 形式在第 19 天展开。

**代码与机制**：动态对象不因保存其地址的局部指针离开作用域而自动销毁。必须由明确的所有者在每条路径上完成恰好一次匹配释放：

```cpp
std::size_t count{3};
int* buffer{new int[count]{2, 4, 6}};

int sum{0};
for (std::size_t i{0}; i < count; ++i) {
    sum += buffer[i];
}

delete[] buffer;
buffer = nullptr;
```

`delete[]` 结束数组元素生命周期并释放相应数组存储。给当前变量赋 `nullptr` 有助于避免它再次访问，但不会更新其他指向同一对象的别名。对空指针执行匹配的 `delete` 是允许的；对同一非空动态对象释放两次或释放后使用则不是。

三类错误必须分开：

- 忘记释放且失去最后访问路径：**内存泄漏**，不必然立即触发未定义行为，但会丢失可复用资源并可能最终耗尽资源。
- `new[]` 配 `delete`、`new` 配 `delete[]`：**未定义行为**。
- 释放后经悬空指针访问：**释放后使用（use-after-free）**，属于未定义行为。

**错误做法与修复**：

```cpp
int* buffer{new int[3]{}};
delete buffer;  // 错误：应为 delete[] buffer
```

分类：**未定义行为**，不保证某次测试一定失败。修复语法是匹配 `delete[]`，工程修复则是减少裸 `new`：数组优先交给标准容器管理，单对象所有权优先交给智能指针管理。容器与智能指针分别在第 16、18 天展开。

**小检查**：`delete[] buffer; buffer = nullptr;` 中，哪一步结束对象生命周期并释放存储？哪一步只改变指针变量？

### 机制四：`malloc/free` 管理原始存储，不应与 `new/delete` 混用

**实际问题**：C 接口和旧代码中常见 `malloc/free`。它们和 `new/delete` 都能取得内存，为什么不能交叉释放？因为它们属于不同的分配/对象管理接口，契约与对象初始化、销毁行为不同。

**概念落点**：[`malloc` 与 `free`（malloc and free）](../docs/glossary/day07.md#malloc-与-freemalloc-and-free)是 C 标准库风格的原始存储分配与释放函数；`malloc` 按字节数请求未初始化存储并以 `void*` 返回，失败时返回空指针，`free` 只接受与该分配族契约相符的指针。

**代码与机制**：

```cpp
#include <cstdlib>

void* raw{std::malloc(128)};
if (raw != nullptr) {
    // 这里只展示分配族配对；对象生命周期与类型化访问需要额外规则。
    std::free(raw);
}
```

`malloc` 不执行 C++ 构造函数，`free` 不执行析构函数；类对象从第 8 天开始学习，这一差异会变得关键。`new` 表达式不仅请求存储，还按类型规则创建并初始化对象；`delete` 表达式会先执行相应销毁语义再释放存储。

以下配对必须保持家族一致：

| 创建/分配 | 对应释放 |
|---|---|
| `new T` | `delete` |
| `new T[n]` | `delete[]` |
| `std::malloc` / `std::calloc` / `std::realloc` 产生的相应分配 | `std::free` |

**错误做法与修复**：

```cpp
int* value{new int{7}};
std::free(value);  // 错误分配族
```

分类：**未定义行为**。不能因为某个平台的 `operator new` 可能在内部调用 `malloc` 就交叉使用公开接口；实现细节不改变调用者必须遵守的配对规则。修复为 `delete value;`，并在现代业务代码中优先让对象、字符串、容器或智能指针自动管理资源。

**小检查**：为什么“某实现的 `new` 内部可能调用 `malloc`”仍不能推出 `free(new T)` 合法？

## 💻 最小可运行示例

完整代码位于 [`examples/day07/main.cpp`](../examples/day07/main.cpp)。

```cpp
#include <cstddef>
#include <iostream>
#include <string>

int main() {
    int readings[]{12, 15, 18};
    constexpr std::size_t reading_count{sizeof(readings) / sizeof(readings[0])};

    int reading_sum{0};
    for (std::size_t index{0}; index < reading_count; ++index) {
        reading_sum += readings[index];
    }

    const char sensor_name[]{"imu"};
    std::string task{"inspect"};
    task += "-done";

    constexpr std::size_t buffer_count{3};
    int* buffer{new int[buffer_count]{2, 4, 6}};
    int dynamic_sum{0};
    for (std::size_t index{0}; index < buffer_count; ++index) {
        dynamic_sum += buffer[index];
    }

    delete[] buffer;
    buffer = nullptr;

    std::cout << "reading sum: " << reading_sum << '\n';
    std::cout << "sensor: " << sensor_name << '\n';
    std::cout << "task: " << task << '\n';
    std::cout << "dynamic sum: " << dynamic_sum << '\n';
    std::cout << std::boolalpha << "released pointer reset: " << (buffer == nullptr) << '\n';
}
```

预期输出：

```text
reading sum: 45
sensor: imu
task: inspect-done
dynamic sum: 12
released pointer reset: true
```

观察重点：所有数组循环都使用 `[0, count)`；C 字符串包含终止字符；`std::string` 通过接口管理文本；动态数组只释放一次并使用 `delete[]`。

## 🐛 错误代码诊断

### 反例 A：非法数组界限

[`exercises/day07/broken/negative_array_bound.cpp`](../exercises/day07/broken/negative_array_bound.cpp) 被显著标记为故意错误，不进入正常构建目标。

分类：**编译错误**。C++17 普通数组界限不能是负数。

### 反例 B：错误的数组释放形式

[`exercises/day07/broken/mismatched_delete.cpp`](../exercises/day07/broken/mismatched_delete.cpp) 会通过许多编译器的语法检查，但被标记为故意危险代码，绝不能作为正常目标运行。`new[]` 与 `delete` 不匹配，分类为**未定义行为**。

### 反例 C：非终止字符数组冒充 C 字符串

把没有 `\0` 的字符数组交给 `strlen` 或流式 C 字符串输出，会在数组外继续寻找终止符，最终越界访问属于未定义行为；不保证恰好读到某个“随机字符串”。

## 🎤 高频面试实战

本节保留当日主问题的答题框架。先独立口述，再到[第 7 天面试答案](../interview/answers/day07.md)逐题核对所有连续追问。

### 题 1：数组和指针是同一种类型吗？C 风格字符串与 `std::string` 怎么选？

**30 秒口头回答**：数组不是指针；数组对象包含固定数量的连续元素，只是在多数表达式中转换成首元素指针，函数数组形参还会调整为指针，所以长度信息常丢失。C 风格字符串是以 `\0` 终止的字符序列，接口必须依赖终止约定；`std::string` 自己管理长度和存储，现代 C++ 文本处理通常优先使用它，只有兼容 C 接口等边界才转换。

**完整答题逻辑**：先比较类型、对象内容和 `sizeof`；再说明转换发生的上下文与函数参数调整；随后把“字符数组”和“C 字符串”分开，以终止字符作为必要条件；最后说明标准库类型的工程优势而不虚构其内部布局。

**可能连续追问**：

- `sizeof(array)` 与 `sizeof(pointer)` 为什么不同？
- 函数参数写成 `int values[10]` 会自动检查至少有 10 个元素吗？
- 数组元素连续存储是否是标准保证？越界访问会怎样？
- `char text[3]{'a', 'b', 'c'}` 是合法 C 字符串吗？
- `std::string::size()` 是否包含 C 接口所需的终止空字符？

**容易失分的说法**：直接说数组就是常量指针；认为函数形参中的方括号保留长度；说越界必然抛异常；把任意字符数组都叫 C 字符串。

### 题 2：`new/delete` 与 `malloc/free` 有什么区别？为什么现代 C++ 避免裸 `new`？

**30 秒口头回答**：`new` 表达式按类型申请存储并初始化对象，`delete` 执行相应销毁语义并释放存储；`malloc/free` 按字节管理原始存储，不调用 C++ 构造和析构。分配族不能混用，单对象和数组形式也必须匹配。裸 `new` 把每条路径上的恰好一次释放责任交给人工，容易泄漏、重复释放或释放后使用，所以现代 C++ 优先值、字符串、容器和智能指针。

**完整答题逻辑**：按“是否创建对象、失败接口、释放配对、所有权表达”比较；列出三组正确配对；再区分泄漏与未定义行为；最后说明课程学习手动机制是为了读旧代码和理解底层，工程默认选择自动管理类型。

**可能连续追问**：

- 普通 `new` 分配失败时会返回 `nullptr` 吗？
- 为什么 `new[]` 必须配 `delete[]`，`new` 必须配 `delete`？
- 内存泄漏本身一定是未定义行为吗？
- `delete nullptr`、重复 `delete` 和释放后使用分别怎样判断？
- 既然学习了手动释放，工程中为什么仍优先容器和智能指针？

**容易失分的说法**：只说 `new` 比 `malloc` 高级；声称所有 `new` 失败都返回空指针；把泄漏、双重释放和释放后使用混成一种错误；认为置空一个指针会自动置空所有别名。

## ✍️ 当日练习

完成 [`exercises/day07/README.md`](../exercises/day07/README.md)：

- 5 道概念判断；
- 3 组数组、字符串和释放错误分析；
- 一道动态采样缓冲区小程序。

先标出每个范围的元素数量、合法下标和资源所有者，再编写代码。完成后再查看 [`solutions/day07/README.md`](../solutions/day07/README.md)。

## 🧠 深度思考题

一个旧式 C 接口要求 `const char*` 任务名和 `const int* + size` 读数；你的 C++17 业务层希望避免裸所有权。请设计两层之间的数据流：业务层保存什么类型、调用边界怎样取得指针与长度、谁保证生命周期、谁负责释放？指出哪些保证来自标准接口，哪些只是团队约定。

## ✅ 完成标准

- 能证明数组与指针不是同一类型，并解释数组到指针转换和函数参数调整。
- 能写出严格 `[0, size)` 的数组循环并判断越界属于未定义行为。
- 能用终止字符区分字符数组与 C 风格字符串，并优先使用 `std::string` 管理文本。
- 能正确配对 `new/delete`、`new[]/delete[]` 和 `malloc/free`。
- 能区分内存泄漏、错误释放、双重释放和释放后使用，并说明现代 C++ 的自动资源管理方向。
