# 第 2 天练习单

主题：变量、类型、初始化与表达式

## 作答方式

每道题下方已经留出作答区。请直接在相应位置填写，不需要再复制到文末模板。

- 文字题：直接在题目下方的空白处填写答案
- 代码题：直接在 `cpp` 代码块中编写
- 预测题：先填写预测，再运行验证
- 完成后保存并同步到 GitHub，然后在对话中告诉我“请批改第 2 天”

---

## 练习 1：识别类型和初始值

阅读：

```cpp
int joint_count{6};
double angle{45.5};
char axis{'Z'};
bool ready{true};
std::string name{"Arm-A"};
```

分别写出五个变量的类型和初始值。

**你的答案：**

1. `joint_count`
   - 类型：int
   - 初始值：6
1. `angle`
   - 类型：double
   - 初始值：45.5
1. `axis`
   - 类型：char
   - 初始值：Z
1. `ready`
   - 类型：bool
   - 初始值：true
1. `name`
   - 类型：std::string
   - 初始值：Arm-A

---

## 练习 2：预测表达式结果

先不要运行：

```cpp
#include <iostream>

int main() {
    int a{7};
    int b{2};
    double c{7.0};

    std::cout << a + b << '\n';
    std::cout << a / b << '\n';
    std::cout << a % b << '\n';
    std::cout << c / b << '\n';
    return 0;
}
```

**你的预测：**

```text
9
3
1
3.5
```

**实际运行结果：**

```text

```

**如果预测与结果不同，原因是什么？**

> 

---

## 练习 3：区分初始化、赋值和表达式

阅读：

```cpp
int speed{10};
speed = 20;
int result{speed + 5};
```

回答：

1. 哪一行创建并初始化了 `speed`？

   > 第一行

2. 哪一行给已经存在的 `speed` 赋新值？

   > 第二行

3. `speed + 5` 是什么？它的结果是多少？

   > 表达式；25

4. 三行全部执行后，`speed` 和 `result` 分别是多少？

   > `speed`：  20
   > `result`：25

---

## 练习 4：发现并修复错误

下面代码包含多处问题：

```cpp
#include <iostream>
#include <string>

int main() {
    int joint_count;
    std::cout << joint_count << '\n';

    int speed{3.5};
    char axis{"Z"};
    std::string name{'Arm'};

    return 0;
}
```

请写出修复后的完整代码。要求：

- `joint_count` 初始值为 `0`
- `speed` 能保存 `3.5`
- `axis` 保存字符 `Z`
- `name` 保存字符串 `Arm`

**你的代码：**

```cpp
#include <iostream>
#include <string>

int main() {
    int joint_count;
    std::cin << joint_count << '\n';

    int speed{3};
    char axis{'Z'};
    std::string name{"Arm"};

    return 0;
}
```

**你一共发现了几处问题？分别是什么？**

> 

---

## 练习 5：修改示例

打开 `examples/day-02/variables.cpp`，增加一个变量：

```cpp
double cycle_time{};
```

让程序：

1. 提示用户输入单次运动周期，单位为秒
2. 计算所有关节依次完成一次运动所需的总时间
3. 输出总时间

计算式可以写成：

```cpp
double total_time{joint_count * cycle_time};
```

**请粘贴修改后的完整代码：**

```cpp

```

**输入示例与实际输出：**

```text

```

---

## 练习 6：独立编写

从空白文件编写一个程序：

1. 使用 `double` 读取左关节角度
2. 使用 `double` 读取右关节角度
3. 计算两个角度的平均值
4. 输出平均值

平均值公式：

```text
(左关节角度 + 右关节角度) / 2.0
```

**你的完整代码：**

```cpp
#include <iostream>
#include <string>

int main() {
    double left_joint;
    std::cin << left_joint << '\n';
	
	double right_joint;
    std::cin << right_joint << '\n';
    
    double average_value{(left_joint+right_joint)/2};
    std::cout >> average_value >> '\n';
    
    return 0;
}
```

**输入和实际输出：**

```text

```

---

## 练习 7：用自己的话解释

每题用一两句话回答。

1. 什么是变量？

   > 程序中一个有名字，可以存储数据的位置

2. 类型有什么作用？

   > 确定对象的种类
   > 确定对象可以进行怎样的操作
   > 程序怎么保存该对象

3. 初始化和赋值有什么区别？

   > 初始化只进行一次；
   > 赋值在初始化之后，可以进行很多次；

4. 什么是表达式？

   > 可以计算并得到结果的一段代码

5. 为什么 `7 / 2` 的结果是 `3`，而不是 `3.5`？

   > 因为进行的是整数除法

---

## 学习复盘

**今天最清楚的知识点：**

> 

**仍然不理解或想继续追问的地方：**

> 我虽然知道初始化、赋值以及表达式等这些基础概念所表示的东西是干什么的，但是让我复述出其定义有点困难

完成后保存并同步到 GitHub，然后在对话中告诉我：

> 请批改第 2 天
