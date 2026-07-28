# 第 2 天：变量、类型、初始化与表达式

预计用时：90～120 分钟  
语言标准：C++17

## 今日目标

学完今天的内容，你应该能够：

1. 用自己的话解释变量、值、类型、初始化、赋值和表达式
2. 使用 `int`、`double`、`char`、`bool` 和 `std::string`
3. 使用花括号为变量提供初始值
4. 区分“创建变量时给初始值”和“以后修改变量的值”
5. 完成基本的加、减、乘、除、取余运算
6. 使用 `std::cin` 读取简单输入，并用 `std::cout` 输出结果

今天不讨论变量位于栈还是堆，也不深入讨论对象和内存。这些知识会在完成必要的语法铺垫后再学习。

---

## 一、实际问题：程序怎样记住数据？

第 1 天的程序只能输出写死的文字：

```cpp
std::cout << "Robot learning started!\n";
```

真实的机器人程序还需要记住许多会变化的数据，例如：

- 机器人名称
- 关节数量
- 目标角度
- 当前速度
- 是否已经启动

C++ 使用**变量**保存这些数据。

例如：

```cpp
int joint_count{6};
```

可以先把它读成：

> 创建一个名为 `joint_count` 的变量，它保存整数，初始值是 `6`。

---

## 二、六张名词卡

### 1. 值

值就是程序处理的具体数据，例如：

```text
6
45.5
Z
true
Robot-A
```

### 2. 变量

通俗理解：变量是程序中一个有名字、能够保存数据的位置。

例如：

```cpp
int joint_count{6};
```

- 变量名：`joint_count`
- 当前保存的值：`6`

变量的值通常可以在程序运行过程中改变。

### 3. 类型

类型告诉编译器：

- 这个变量准备保存哪一类数据
- 可以对它进行哪些操作
- 编译器应该怎样解释这份数据

例如：

```cpp
int joint_count{6};
double angle{45.5};
```

`joint_count` 保存整数，`angle` 保存带小数的数。

### 4. 变量名

变量名是程序员给变量起的名字，也叫标识符。

推荐使用能够说明用途的名字：

```cpp
int joint_count{6};  // 清楚
int x{6};            // 能用，但含义不清楚
```

变量名：

- 可以包含字母、数字和下划线
- 不能以数字开头
- 不能包含空格
- 不能使用 `int`、`return` 等 C++ 关键字
- 区分大小写，`speed` 和 `Speed` 是两个不同的名字

### 5. 初始化

初始化是：

> 创建变量时，为它提供最开始的值。

```cpp
int speed{10};
```

这里的 `10` 是 `speed` 的初始值。

### 6. 表达式

表达式是一段能够计算出一个值的代码。

```cpp
speed + 5
```

如果 `speed` 当前为 `10`，这个表达式的结果就是 `15`。

---

## 三、读懂一条变量定义

```cpp
int joint_count{6};
```

逐部分解释：

| 部分 | 含义 |
|---|---|
| `int` | 变量的类型，表示整数 |
| `joint_count` | 变量名 |
| `{6}` | 用 `6` 初始化变量 |
| `;` | 这条语句结束 |

创建后可以输出它：

```cpp
std::cout << joint_count << '\n';
```

这里不要给变量名加双引号：

```cpp
std::cout << joint_count << '\n';    // 输出变量保存的值：6
std::cout << "joint_count" << '\n';  // 输出文字：joint_count
```

---

## 四、五种常用类型

### 1. `int`：整数

```cpp
int joint_count{6};
int error_code{-1};
```

适合保存没有小数部分的整数。

### 2. `double`：带小数的数

```cpp
double angle{45.5};
double speed{0.25};
```

初学阶段，遇到需要小数的数值通常优先使用 `double`。

### 3. `char`：单个字符

```cpp
char axis{'Z'};
```

`char` 通常只保存一个字符，并使用**单引号**。

```cpp
char axis{'Z'};       // 单个字符
std::string name{"Z"}; // 一段文本
```

### 4. `bool`：真假

```cpp
bool motor_enabled{true};
bool emergency_stop{false};
```

`bool` 只有两个值：`true` 和 `false`，适合表示“是/否”“开启/关闭”。

### 5. `std::string`：一段文本

使用 `std::string` 前需要：

```cpp
#include <string>
```

定义字符串：

```cpp
std::string robot_name{"Robot-A"};
```

字符串使用**双引号**，可以包含多个字符。

| 数据 | 合适的类型 | 示例 |
|---|---|---|
| 关节数量 | `int` | `int count{6};` |
| 目标角度 | `double` | `double angle{45.5};` |
| 坐标轴字母 | `char` | `char axis{'Z'};` |
| 是否启动 | `bool` | `bool started{true};` |
| 机器人名称 | `std::string` | `std::string name{"Arm-A"};` |

---

## 五、初始化和赋值不是同一件事

### 初始化：变量刚创建时获得第一个值

```cpp
int speed{10};
```

此时 `speed` 刚刚被创建，初始值是 `10`。

### 赋值：变量已经存在，再修改它保存的值

```cpp
speed = 20;
```

执行过程：

| 时刻 | `speed` 的值 |
|---|---:|
| 初始化之后 | 10 |
| 执行 `speed = 20;` 之后 | 20 |

注意，赋值语句左侧不再写类型：

```cpp
int speed{10}; // 创建并初始化
speed = 20;    // 修改已有变量
```

如果再次写 `int`，含义就不再是“修改原变量”，而是在尝试定义变量。

### 为什么推荐花括号初始化？

本课程默认使用：

```cpp
int count{6};
```

花括号可以帮助编译器阻止某些会丢失信息的初始化：

```cpp
int count{6.8}; // 编译错误：6.8 不能安全地放进 int
```

如果确实需要小数，应选择合适的类型：

```cpp
double count{6.8};
```

### 不要让基本类型变量没有可靠初始值

初学阶段不要这样写：

```cpp
int count;
std::cout << count << '\n';
```

这个局部 `int` 没有得到可靠的初始值，直接读取会产生严重问题。先养成初始化习惯：

```cpp
int count{}; // 初始化为 0
```

更完整的原因会在第 9 天和第 14 天解释。

---

## 六、表达式和基本运算符

假设：

```cpp
int a{7};
int b{2};
```

| 运算 | 写法 | 结果 |
|---|---|---:|
| 加法 | `a + b` | 9 |
| 减法 | `a - b` | 5 |
| 乘法 | `a * b` | 14 |
| 整数除法 | `a / b` | 3 |
| 取余 | `a % b` | 1 |

可以用表达式初始化另一个变量：

```cpp
int sum{a + b};
```

右侧的 `a + b` 先计算出 `9`，再用结果初始化 `sum`。

### 整数除法会舍去小数部分

```cpp
int result{7 / 2};
std::cout << result << '\n'; // 输出 3
```

因为 `7` 和 `2` 都是整数，使用的是整数除法。

想得到 `3.5`，至少让参与除法的数是小数类型：

```cpp
double result{7.0 / 2.0};
```

### `%` 表示取余

```cpp
int remainder{7 % 2}; // 结果为 1
```

初学阶段只对整数使用 `%`。

### 使用括号明确顺序

```cpp
int result1{2 + 3 * 4};   // 14
int result2{(2 + 3) * 4}; // 20
```

乘除通常先于加减。拿不准时使用括号明确自己的意图。

### 除数不能为零

```cpp
int result{10 / 0}; // 错误做法
```

暂时先记住：除法和取余的右侧不能为 `0`。第 3 天学习 `if` 后，我们会检查输入是否为零。

---

## 七、使用 `std::cin` 接收输入

第 1 天使用 `std::cout` 输出：

```cpp
std::cout << "Please enter joint count: ";
```

使用 `std::cin` 可以从终端读取用户输入：

```cpp
int joint_count{};
std::cin >> joint_count;
```

可以把：

```cpp
std::cin >> joint_count;
```

暂时读作：

> 从标准输入读取一个整数，保存到 `joint_count`。

完整示例：

```cpp
#include <iostream>

int main() {
    int joint_count{};

    std::cout << "Joint count: ";
    std::cin >> joint_count;

    std::cout << "You entered: " << joint_count << '\n';
    return 0;
}
```

输入：

```text
6
```

输出：

```text
You entered: 6
```

`std::cin >> name` 读取字符串时，空格通常会结束本次读取。因此今天输入机器人名称时先使用不含空格的名称，例如 `Robot-A`。带空格的一整行文本以后再学习。

---

## 八、完整示例：机器人目标角度

仓库示例：[variables.cpp](../examples/day-02/variables.cpp)

```cpp
#include <iostream>
#include <string>

int main() {
    std::string robot_name{};
    int joint_count{};
    double target_angle{};

    std::cout << "Robot name (no spaces): ";
    std::cin >> robot_name;

    std::cout << "Joint count: ";
    std::cin >> joint_count;

    std::cout << "Target angle for each joint: ";
    std::cin >> target_angle;

    double total_target_angle{joint_count * target_angle};

    std::cout << "\nRobot: " << robot_name << '\n';
    std::cout << "Joints: " << joint_count << '\n';
    std::cout << "Target angle: " << target_angle << '\n';
    std::cout << "Total target angle: " << total_target_angle << '\n';

    return 0;
}
```

如果依次输入：

```text
Arm-A
6
45.5
```

关键计算为：

```cpp
joint_count * target_angle
```

也就是：

```text
6 × 45.5 = 273
```

程序会把结果保存到 `total_target_angle`。

---

## 九、跟踪变量变化

阅读：

```cpp
int speed{10};
int increase{5};
speed = speed + increase;
int doubled{speed * 2};
```

按顺序跟踪：

| 执行完的语句 | `speed` | `increase` | `doubled` |
|---|---:|---:|---:|
| `int speed{10};` | 10 | 尚未创建 | 尚未创建 |
| `int increase{5};` | 10 | 5 | 尚未创建 |
| `speed = speed + increase;` | 15 | 5 | 尚未创建 |
| `int doubled{speed * 2};` | 15 | 5 | 30 |

其中：

```cpp
speed = speed + increase;
```

不是数学等式，而是一条赋值语句。它的执行顺序是：

1. 读取右侧旧的 `speed`，得到 `10`
2. 计算 `10 + 5`，得到 `15`
3. 把 `15` 保存回左侧的 `speed`

---

## 十、常见错误

### 错误 1：使用尚未定义的变量

```cpp
speed = 10; // 前面没有定义 speed
```

应先定义：

```cpp
int speed{10};
```

### 错误 2：字符串和字符的引号混淆

```cpp
char axis{"Z"};              // 错误
std::string name{'Robot'};   // 错误
```

正确：

```cpp
char axis{'Z'};
std::string name{"Robot"};
```

### 错误 3：把小数放入 `int`

```cpp
int angle{45.5}; // 编译错误
```

正确：

```cpp
double angle{45.5};
```

### **错误 4：误以为整数除法会保留小数**

```cpp
double average{7 / 2}; // 结果仍然是 3.0
```

`7 / 2` 会先进行整数除法得到 `3`，之后才转成 `double`。

改为：

```cpp
double average{7.0 / 2.0}; // 3.5
```

---

## 十一、今日动手流程

1. 不运行代码，预测示例会怎样读取和输出数据
2. 编译并运行 `examples/day-02/variables.cpp`
3. 分别输入整数和小数，观察输出
4. 修改变量的类型或初始值，观察编译器反应
5. 完成第 2 天练习单
6. 从空白文件独立写一个“读取两个角度并计算平均值”的程序

编译命令：

```bash
g++ -std=c++17 -Wall -Wextra -pedantic examples/day-02/variables.cpp -o day02
```

Windows PowerShell 运行：

```powershell
.\day02.exe
```

Linux 或 macOS 运行：

```bash
./day02
```

---

## 十二、今日完成标准

完成以下事项即可认为第 2 天达标：

- 能解释变量、类型、初始化、赋值和表达式
- 能为关节数量、目标角度、轴名称和启用状态选择基本类型
- 能用花括号初始化变量
- 能解释为什么 `7 / 2` 得到 `3`
- 能用 `std::cin` 读取整数和小数
- 能独立写出读取两个角度并计算平均值的程序

练习题见：[第 2 天练习单](../exercises/day-02.md)。每道题的作答区都直接放在题目下方，不再使用文末对话提交模板。

下一单元将学习：`if`、`switch`、循环与代码块。
