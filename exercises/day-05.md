# 第 5 天练习单

主题：函数的声明、定义、调用、参数与返回  
语言标准：C++17

## 作答方式

- 每道题直接在题目下方作答。
- 预测题先填写预测，再编译运行并填写实际结果。
- 代码题在紧随题目的空代码块中编写。
- 保留编译器或链接器的关键报错，不必粘贴无关的整段终端内容。

---

## 练习 1：拆解函数

阅读：

```cpp
double calculate_speed(double distance, double time);
```

### 1.1 返回类型

> 

### 1.2 函数名

> 

### 1.3 形参数量与每个形参的类型

> 

### 1.4 这一行是声明还是定义？为什么？

> 

---

## 练习 2：区分声明、定义和调用

分别判断 A～D：

```cpp
// A
int add(int, int);

// B
int result{add(2, 3)};

// C
int add(int left, int right) {
    return left + right;
}

// D
add(4, 5);
```

### A

> 

### B

> 

### C

> 

### D

> 

---

## 练习 3：区分形参与实参

阅读：

```cpp
int multiply(int left, int right) {
    return left * right;
}

int main() {
    int factor{4};
    int result{multiply(factor, 3)};
    return 0;
}
```

### 3.1 形参

> 

### 3.2 实参

> 

### 3.3 调用后 `left`、`right` 和 `result` 的值

> 

---

## 练习 4：预测并验证

```cpp
#include <iostream>

int adjust(int value) {
    if (value < 0) {
        return 0;
    }

    return value + 2;
}

int main() {
    int first{adjust(-3)};
    int second{adjust(5)};

    std::cout << first << '\n';
    std::cout << second << '\n';
    return 0;
}
```

### 4.1 预测

> 

### 4.2 实际结果

> 

### 4.3 第一次调用为什么没有执行 `return value + 2;`？

> 

---

## 练习 5：补写声明、定义和调用

编写一个完整程序：

- 函数名为 `square`
- 接收一个 `int`
- 返回这个整数的平方
- 函数声明写在 `main` 前
- 函数定义写在 `main` 后
- 在 `main` 中调用 `square(6)` 并输出结果

```cpp

```

### 编译命令

> 

### 运行结果

> 

---

## 练习 6：编写 `void` 函数

编写函数：

```cpp
void print_motor_state(bool enabled);
```

要求：

- `enabled` 为 `true` 时输出 `Motor: on`
- 否则输出 `Motor: off`
- 在 `main` 中分别用 `true` 和 `false` 调用

```cpp

```

### 运行结果

> 

---

## 练习 7：制造真实的链接错误

新建 `missing_definition.cpp`：

```cpp
int motor_power(int level);

int main() {
    return motor_power(3);
}
```

先只编译：

```bash
g++ -std=c++17 -Wall -Wextra -pedantic \
    -c missing_definition.cpp \
    -o missing_definition.o
```

再链接：

```bash
g++ missing_definition.o -o missing_definition
```

### 7.1 编译阶段是否成功？

> 

### 7.2 链接阶段的关键报错

> 

### 7.3 为什么声明足以通过编译，却不足以完成链接？

> 

### 7.4 补上定义并写出最终代码

```cpp

```

---

## 练习 8：修复所有返回路径

下面的函数有什么问题？

```cpp
int battery_level(bool sensor_ok) {
    if (sensor_ok) {
        return 80;
    }
}
```

### 问题说明

> 

### 修复后的函数

```cpp

```

### 使用严格警告选项时的关键提示

> 

---

## 练习 9：错误分类

分别判断主要在预处理、狭义编译、汇编、链接还是运行阶段暴露，并说明理由。

### 9.1 调用 `add(1)`，但声明要求两个 `int` 实参

> 

### 9.2 已声明并调用 `add(int, int)`，整个程序中没有定义

> 

### 9.3 程序读取到运行时除数为 `0`，随后执行整数除法

> 

### 9.4 `#include "missing.hpp"`

> 

---

## 练习 10：准确解释 `main`

判断并订正：

> 操作系统装载程序后，会直接执行 `main` 的第一条 C++ 语句；`return 0;` 会把数字 `0` 输出到终端。

### 判断

> 

### 订正

> 

### 为什么普通有返回值函数不能依赖“执行到末尾自动返回 0”？

> 

---

## 练习 11：独立编程——机器人安全判断

编写完整程序，至少包含以下三个函数：

```cpp
bool temperature_safe(double temperature);
bool battery_safe(int battery_percent);
void print_safety_result(bool safe);
```

要求：

1. 温度在 `0.0`～`70.0` 之间时安全
2. 电量不低于 `20` 时安全
3. 两项都安全才输出 `Robot can start`
4. 否则输出 `Robot cannot start`
5. 声明写在 `main` 前，定义写在 `main` 后
6. 至少测试三组不同数据

```cpp

```

### 测试 1：输入或初始值、预测、实际结果

> 

### 测试 2：输入或初始值、预测、实际结果

> 

### 测试 3：输入或初始值、预测、实际结果

> 

---

## 学习复盘

### 1. 声明、定义和调用分别解决什么问题？

> 

### 2. 形参与实参的区别是什么？

> 

### 3. `return` 和 `std::cout` 的数据去向有何不同？

> 

### 4. 本单元最容易混淆的知识点

> 
