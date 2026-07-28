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
   - 类型：
   - 初始值：
2. `angle`
   - 类型：
   - 初始值：
3. `axis`
   - 类型：
   - 初始值：
4. `ready`
   - 类型：
   - 初始值：
5. `name`
   - 类型：
   - 初始值：

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

   > 

2. 哪一行给已经存在的 `speed` 赋新值？

   > 

3. `speed + 5` 是什么？它的结果是多少？

   > 

4. 三行全部执行后，`speed` 和 `result` 分别是多少？

   > `speed`：  
   > `result`：

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

```

**输入和实际输出：**

```text

```

---

## 练习 7：用自己的话解释

每题用一两句话回答。

1. 什么是变量？

   > 

2. 类型有什么作用？

   > 

3. 初始化和赋值有什么区别？

   > 

4. 什么是表达式？

   > 

5. 为什么 `7 / 2` 的结果是 `3`，而不是 `3.5`？

   > 

---

## 学习复盘

**今天最清楚的知识点：**

> 

**仍然不理解或想继续追问的地方：**

> 

完成后保存并同步到 GitHub，然后在对话中告诉我：

> 请批改第 2 天
