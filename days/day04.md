# 第 4 天：函数如何划分责任

预计用时：120 分钟。标准主线：C++17。

## 🎯 今日攻坚目标

能够设计输入、输出和职责清晰的小函数，准确解释值传递，并判断重载、默认实参和递归调用的匹配与终止问题。

## 🔁 前置知识检查

先独立回答，再回看第 1—3 天：

1. 函数声明与函数定义分别向编译和链接提供什么信息？
2. `double result{5 / 2};` 为什么得到 `2.0` 而不是 `2.5`？
3. `if` 怎样保证一个非 `void` 函数的每条正常路径都返回结果？

参考方向：声明说明调用所需属性，定义提供函数体；整数除法先发生；需要逐条检查所有可能离开函数的控制路径。

## 📖 核心知识重构

第 1 天已经认识了“函数”及其声明和定义。今天把函数当作责任边界：调用方只应知道怎样提供输入和使用结果，函数内部负责完成一件可说明、可测试的工作。

### 机制一：形参描述输入位置，实参提供本次输入，返回值把结果带回调用处

**实际问题**：轨迹评分、温度限制和任务计数都可能写成函数。如果说不清输入从哪里来、结果回到哪里，就容易把函数内外的同名变量当成一个对象，或漏掉某条返回路径。

先读一个完整调用：

```cpp
int next_count(int current_count) {
    return current_count + 1;
}

int before{3};
int after{next_count(before)};
```

**概念落点**：

- [形参与实参（parameter and argument）](../docs/glossary/day04.md#形参与实参parameter-and-argument)：形参是在函数声明或定义的参数列表中声明的输入位置及其类型约束；实参是在函数调用表达式中提供、用于初始化对应形参的表达式。`current_count` 是形参，`before` 是这次调用的实参。
- [返回值（return value）](../docs/glossary/day04.md#返回值return-value)是非 `void` 函数的一次调用完成时，按照函数返回类型交给该调用表达式的结果值。这里的结果值用于初始化 `after`。

形参名与实参名无需相同，实参甚至不需要有名称：`next_count(3)` 同样合法。匹配关注参数位置、类型和允许的转换，而不是变量名是否一样。

**代码与机制**：函数头可以按三个问题阅读：

```cpp
int next_count(int current_count)
// ^ 返回类型   ^ 函数名      ^ 输入位置及类型
```

调用开始时，对应实参用于初始化形参；执行 `return current_count + 1;` 时，表达式结果按返回类型规则成为本次调用结果，并结束这次函数执行。调用方可以使用或忽略该结果：

```cpp
int saved{next_count(3)};  // 使用返回值初始化对象
next_count(3);             // 调用仍发生，但结果被忽略
```

接口应表达单一责任。`int next_count(int)` 比“既更新全局状态、又打印、又返回不明状态码”的函数更容易独立测试。今天暂不引入全局状态规则，第 5 天再讲作用域和存储期。

**错误做法与修复**：

```cpp
int alarm_level(double temperature) {
    if (temperature >= 30.0) {
        return 1;
    }
    // 故意错误：temperature < 30.0 时到达函数末尾
}
```

对非 `main` 的值返回函数，如果本次调用沿正常路径到达末尾而没有返回值，会触发**未定义行为**；编译器经常警告，但不要把警告当作标准规定的固定运行结果。修复是让所有正常路径返回与接口一致的结果：

```cpp
int alarm_level(double temperature) {
    if (temperature >= 30.0) {
        return 1;
    }
    return 0;
}
```

**小检查**：在 `int after{next_count(before)};` 中，哪个是形参、哪个是实参、哪个表达式产生返回值？

### 机制二：值传递建立独立形参对象，修改形参不会倒流到调用方基本类型对象

**实际问题**：函数内部把计数加一，调用方的原计数为什么没变？如果把“把值交给函数”和“让函数直接操作原对象”混在一起，就会误判大量代码输出。

**概念落点**：[值传递（pass by value）](../docs/glossary/day04.md#值传递pass-by-value)是用实参的值初始化一个非引用形参对象，使函数体操作该独立形参对象的传参方式。

```cpp
int add_one(int count) {
    ++count;       // 修改形参对象
    return count;
}

int original{3};
int changed{add_one(original)};
```

执行后 `original` 仍为 `3`，`changed` 为 `4`。两个名称即使都写成 `count`，也不会因为拼写相同而指向同一个对象。

**代码与机制**：对今天的 `int`、`double` 和 `bool`，可以用“把实参当前值交给一个新的形参对象”作为准确模型。函数体修改的是形参；要把结果用于调用方，应通过返回值明确传回，再由调用方决定是否赋值：

```cpp
int reset_to_zero(int count) {
    count = 0;
    return count;
}

int samples{5};
samples = reset_to_zero(samples);  // 调用方明确接收并赋值
```

标准语义要求存在独立形参对象，但不要求生成的机器代码一定包含一条可见的内存复制指令。编译器可以在不改变可观察结果的前提下优化。类对象的值传递还会涉及拷贝、移动和省略，第 10—11 天再展开；不要把今天的标量成本结论扩大成所有类型都“零成本”或“很昂贵”。

**错误做法与修复**：

```cpp
int reset_to_zero(int count) {
    count = 0;
    return count;
}

int samples{5};
reset_to_zero(samples);  // 返回值被忽略
std::cout << samples << '\n';  // 输出 5
```

分类：**有明确定义但结果不符合预期**。函数正确返回了 `0`，但调用方忽略结果；值传递也没有修改 `samples`。当前知识范围内，修复为 `samples = reset_to_zero(samples);`。让函数通过引用直接修改调用方对象的方式将在第 6 天学习。

**小检查**：`int next{add_one(original)};` 之后，为什么 `original` 和 `next` 可以分别是 `3` 与 `4`？这是否要求机器上一定发生一次内存复制？

### 机制三：重载提供同名的多种参数接口，默认实参只补足同一个函数的调用

**实际问题**：打印整数和小数可以使用同一个业务名称；设置温度上限又希望常用值可以省略。两者表面上都让调用写得更短，但语言机制完全不同。

**概念落点**：

- [函数重载（function overloading）](../docs/glossary/day04.md#函数重载function-overloading)是在同一作用域中让多个函数使用同一名称和可区分的参数列表，并在调用处通过重载决议选择唯一最佳可行函数的机制。
- [默认实参（default argument）](../docs/glossary/day04.md#默认实参default-argument)是在函数声明中为形参指定的表达式，当调用省略对应实参时，该表达式用于补足这次调用。

先看重载：

```cpp
void report(int value) {
    std::cout << "count: " << value << '\n';
}

void report(double value) {
    std::cout << "reading: " << value << '\n';
}

report(3);    // int 版本是唯一最佳匹配
report(3.5);  // double 版本是唯一最佳匹配
```

再看默认实参：

```cpp
double cap_reading(double reading, double upper = 80.0) {
    if (reading > upper) {
        return upper;
    }
    return reading;
}

double first{cap_reading(92.0)};        // 相当于为 upper 使用 80.0
double second{cap_reading(92.0, 100.0)}; // 调用方显式提供 upper
```

后者始终只有一个 `cap_reading` 函数。默认实参是在调用点补足缺失输入，不是生成一个单参数重载。

**代码与机制**：重载决议会收集候选函数，检查哪些函数在实参数量、类型和允许转换下可行，再寻找唯一最佳者。若没有可行函数或没有唯一最佳者，调用为编译错误。返回类型不参与“仅靠返回值选择哪个同名函数”，因此下面不能构成重载：

```cpp
int read_status();
double read_status();  // 故意错误：只有返回类型不同
```

对今天的普通函数，某个形参有默认实参后，同一参数列表中它右侧的普通形参也需要可用默认实参；因此常把可选输入放在末尾：

```cpp
int score(int tasks, int points_per_task = 10);  // 清晰
```

默认实参通常只在一个可见声明处写一次（==默认写在头文件的声明中==），避免声明与定义之间重复或不一致。

**错误做法与修复**：

```cpp
void calibrate(int offset, double scale);
void calibrate(double offset, int scale);

calibrate(1, 1);  // 故意错误：没有唯一最佳重载
```

分类：**编译错误**。第一个候选在第一实参上更好，第二个候选在第二实参上更好，没有一个候选整体优于另一个。修复应消除接口竞争，例如使用能表达含义的不同函数名，或在需求明确时提供类型清楚的实参；不要随意强制转换来猜测业务。

**小检查**：`report(3)` 与 `cap_reading(92.0)` 分别涉及几个实际函数？为什么默认实参不会新增重载？

### 机制四：递归必须同时具备终止条件和向终止条件推进的步骤

**实际问题**：任务可以描述为“当前任务的分数，加上剩余更小任务的分数”。这种自相似结构适合递归，但只写“函数调用自己”会造成无法结束的调用链。

**概念落点**：[递归（recursion）](../docs/glossary/day04.md#递归recursion)是函数在执行过程中直接或间接再次调用自身的调用结构。

一个可终止的最小例子：

```cpp
int sum_to(int n) {
    if (n <= 0) {          // 终止条件
        return 0;
    }
    return n + sum_to(n - 1);  // n 变小，向终止条件推进
}
```

调用 `sum_to(3)` 可以按函数语义展开为 `3 + sum_to(2)`，再到 `2 + sum_to(1)`，最后由 `sum_to(0)` 返回 `0`。结果逐层组合为 `6`。

**代码与机制**：每次递归调用都是一次新的函数调用，拥有自己的形参对象和返回过程。主流实现通常==为尚未返回的调用保存调用记录，常称为调用帧并放在运行时调用栈中==；这是常见实现模型，不是 C++ 标准规定的唯一物理布局。标准也不保证尾递归一定被优化。

递归前要回答两个问题：

1. 哪个输入直接产生结果，不再递归？
2. 每次调用用什么可证明的方式靠近该输入？

简单计数问题通常用循环更直观；递归应服务于问题结构，而不是为了显得高级。

**错误做法与修复**：

```cpp
int sum_to(int n) {
    if (n <= 0) {
        return 0;
    }
    return n + sum_to(n);  // n 没有变小
}
```

分类：**逻辑错误（递归不终止）**：对正数输入，调用不会靠近停止条件，因此得不到预期返回结果；它不是编译错误。常见实现最终可能耗尽调用资源，但 C++ 标准不保证出现名为“stack overflow”的特定错误、异常或诊断。修复为递归调用 `sum_to(n - 1)`，并再次检查所有允许输入是否都能到达终止条件。

**小检查**：函数中存在 `if (n <= 0) return 0;` 为什么仍不足以证明递归会终止？还必须检查哪一行？

## 💻 最小可运行示例

完整代码位于 [`examples/day04/main.cpp`](../examples/day04/main.cpp)。

```cpp
#include <iostream>

int next_count(int count) {
    ++count;
    return count;
}

double cap_reading(double reading, double upper = 80.0) {
    if (reading > upper) {
        return upper;
    }
    return reading;
}

void report(int value) {
    std::cout << "count: " << value << '\n';
}

void report(double value) {
    std::cout << "reading: " << value << '\n';
}

int sum_to(int n) {
    if (n <= 0) {
        return 0;
    }
    return n + sum_to(n - 1);
}

int main() {
    int original_count{3};
    int advanced_count{next_count(original_count)};

    std::cout << "original count: " << original_count << '\n';
    std::cout << "advanced count: " << advanced_count << '\n';
    std::cout << "capped: " << cap_reading(92.0) << '\n';
    report(4);
    report(4.5);
    std::cout << "sum to 4: " << sum_to(4) << '\n';
}
```

预期输出：

```text
original count: 3
advanced count: 4
capped: 80
count: 4
reading: 4.5
sum to 4: 10
```

观察重点：值传递没有改变 `original_count`；默认实参补出上限 `80.0`；两个 `report` 调用选择不同重载；递归输入严格减小。

## 🐛 错误代码诊断

### 反例 A：重载调用二义性

[`exercises/day04/broken/ambiguous_overload.cpp`](../exercises/day04/broken/ambiguous_overload.cpp) 被显著标记为故意错误，不进入正常构建目标。

分类：**编译错误**。两个候选各自在一个参数位置更好，重载决议找不到唯一最佳函数。

### 反例 B：值传递结果被忽略

```cpp
int add_one(int count) {
    return count + 1;
}

int tasks{3};
add_one(tasks);
```

分类：**有明确定义但结果不符合预期**。`tasks` 仍为 `3`。修复为 `tasks = add_one(tasks);`，让调用方明确接收结果。

### 反例 C：非 `void` 函数漏掉返回路径

当一次调用从值返回函数末尾离开却没有产生返回值时，分类为**未定义行为**。某些警告设置会让构建失败，但语言层面的风险不能简化为“总是编译错误”。修复是检查控制流并覆盖每条正常路径。

## 🎤 高频面试实战

本节保留当日主问题的答题框架。先独立口述，再到[第 4 天面试答案](../interview/answers/day04.md)逐题核对所有连续追问。

### 题 1：形参和实参有什么区别？值传递为什么不修改调用方的 `int`？

**30 秒口头回答**：形参位于函数声明或定义的参数列表中，规定输入位置和类型；实参位于调用表达式中，为这次调用提供输入。值传递会用实参的值初始化独立的非引用形参对象，因此函数里修改 `int` 形参不会修改调用方的原对象；要把结果带回去，可以返回新值并由调用方赋值。

**完整答题逻辑**：先定位函数两侧，再说明调用时的初始化关系；接着用 `int changed{add_one(original)}` 区分形参对象、原对象和返回值；最后保留实现边界：标准语义是独立形参对象，不要求可见的物理复制指令。

**可能连续追问**：

- 实参必须是变量吗？
- 形参名与实参名相同会发生什么？
- 忽略返回值后，函数调用是否仍执行？
- 类对象按值传递是否一定执行一次拷贝构造？
- 如果确实要修改调用方对象，可以使用什么接口？

最后两个问题分别在第 10—11 天和第 6 天完整展开。

**容易失分的说法**：把形参和实参都笼统叫“函数里的变量”；声称同名就共享对象；声称值传递一定对应一条机器复制指令；忘记说明类类型的后续边界。

### 题 2：函数重载和默认实参有什么区别？调用何时会二义？

**30 秒口头回答**：重载是多个同名函数拥有可区分的参数列表，调用时通过重载决议选唯一最佳可行函数；默认实参只是调用省略输入时为同一个函数补上表达式，不会创建新重载。返回类型不能单独区分重载；如果多个可行候选都不比其他候选整体更好，调用就是二义性的编译错误。

**完整答题逻辑**：先回答“实际有几个函数”，再回答调用点怎样补参数或选候选；随后给 `report(int)`/`report(double)` 和 `cap(value, 80.0)` 两组对照；最后说明无可行候选与无唯一最佳候选都会失败。

**可能连续追问**：

- 只有返回类型不同能否重载？
- 默认实参是定义函数时使用，还是调用缺省实参时补足？
- `void f(int)` 与 `void f(int, int = 0)` 同时存在时，`f(1)` 会怎样？
- `calibrate(int, double)` 与 `calibrate(double, int)` 调用 `calibrate(1, 1)` 为什么二义？
- 重载选择是在编译期还是运行期完成？

**容易失分的说法**：把一个带默认实参的函数说成多个重载；认为返回值接收类型可以消除仅返回类型不同的冲突；只说“编译器随机选一个”；忽略隐式转换会影响候选可行性。

## ✍️ 当日练习

完成 [`exercises/day04/README.md`](../exercises/day04/README.md)：

- 5 道概念判断；
- 3 组代码输出或错误分析；
- 一道任务评分函数小程序。

先写预测和理由，再编译合法代码。完成后再查看 [`solutions/day04/README.md`](../solutions/day04/README.md)。

## 🧠 深度思考题

一个函数要根据“已完成任务数”和“每项分值”计算总分，并允许常用分值省略。请设计它的名称、返回类型、形参、默认实参和错误输入处理；再说明是否需要重载、是否值得用递归，以及调用方如何看出函数不会偷偷修改原任务数。

## ✅ 完成标准

- 能在定义和调用中准确指出形参、实参和返回值。
- 能预测基本类型值传递后的函数内外对象状态。
- 能区分重载与默认实参，并识别返回类型不能单独形成重载。
- 能解释重载二义性是没有唯一最佳可行函数，而不是编译器随机选择。
- 能为递归写出终止条件和严格推进步骤，并区分标准语义与常见调用栈实现。
