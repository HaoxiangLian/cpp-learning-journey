# 第 6 天练习单

主题：值传递、引用传递、函数重载、默认参数与递归  
语言标准：C++17

## 作答方式

- 直接在每道题下方的空白区作答。
- 预测题必须先预测，再编译运行并记录实际结果。
- 代码题在紧随题目的空 `cpp` 代码块中完成。
- 错误实验保留关键诊断，并说明发生阶段。

---

## 练习 1：预测值传递

```cpp
#include <iostream>

void change(int value) {
    value = value + 5;
    std::cout << "Inside: " << value << '\n';
}

int main() {
    int speed{10};
    change(speed);
    std::cout << "Outside: " << speed << '\n';
    return 0;
}
```

### 1.1 预测

> 

### 1.2 实际结果

> 

### 1.3 为什么 `speed` 没有随形参一起改变？

> 

---

## 练习 2：预测引用传递

把函数改为：

```cpp
void change(int& value) {
    value = value + 5;
    std::cout << "Inside: " << value << '\n';
}
```

### 2.1 预测

> 

### 2.2 实际结果

> 

### 2.3 `value` 与 `speed` 是两个独立 `int` 对象吗？

> 

---

## 练习 3：编写引用函数

编写：

```cpp
void order_pair(int& first, int& second);
```

如果 `first > second`，交换两个调用者对象的值。不能使用尚未学习的库交换函数。

```cpp

```

### 测试数据与预测

> 

### 实际结果

> 

---

## 练习 4：引用绑定错误

```cpp
void reset(int& value) {
    value = 0;
}

int main() {
    reset(10);
    return 0;
}
```

### 4.1 能否通过编译？

> 

### 4.2 关键报错

> 

### 4.3 为什么 `int&` 不能绑定这里的 `10`？

> 

### 4.4 如果函数只读参数，怎样修改形参类型才能接受 `10`？

```cpp

```

---

## 练习 5：判断重载选择

```cpp
void report(int value);
void report(double value);
```

分别写出选择结果及理由。

### 5.1 `report(8)`

> 

### 5.2 `report(8.0)`

> 

### 5.3 `char code{'A'}; report(code);`

> 

### 5.4 使用“候选函数、可行函数、最佳可行函数”解释 5.3

> 

---

## 练习 6：制造重载歧义

```cpp
void set_level(long value);
void set_level(double value);

int main() {
    set_level(10);
    return 0;
}
```

### 6.1 预测

> 

### 6.2 实际编译结果

> 

### 6.3 为什么没有唯一最佳可行函数？

> 

### 6.4 用显式改变实参类型的方式分别选择两个重载

```cpp

```

---

## 练习 7：自己编写重载

编写两个同名函数：

```cpp
int area(int side);
double area(double width, double height);
```

第一个计算正方形面积，第二个计算矩形面积。在 `main` 中分别调用并输出。

```cpp

```

### 运行结果

> 

### 为什么这两个函数可以构成重载？

> 

---

## 练习 8：默认实参

声明并定义：

```cpp
int travel_time(int distance, int speed = 5);
```

分别调用：

- `travel_time(20)`
- `travel_time(20, 10)`

```cpp

```

### 预测

> 

### 实际结果

> 

### 默认实参应该主要写在调用点可见的声明中，还是只写在定义中？

> 

---

## 练习 9：找出默认实参错误

### 9.1 非尾部空缺

```cpp
void configure(int speed = 10, int power, bool enabled = true);
```

> 

### 9.2 重复指定

```cpp
int cost(int count, int unit = 2);

int cost(int count, int unit = 2) {
    return count * unit;
}
```

> 

### 9.3 与重载组合造成歧义

```cpp
void move(int distance);
void move(int distance, int speed = 1);

move(5);
```

> 

---

## 练习 10：跟踪递归

```cpp
int sum_to(int value) {
    if (value <= 0) {
        return 0;
    }

    return value + sum_to(value - 1);
}
```

### 10.1 写出 `sum_to(4)` 的完整展开过程

> 

### 10.2 每层调用的 `value` 是否为同一个对象？

> 

### 10.3 基例

> 

### 10.4 递归步骤怎样接近基例？

> 

---

## 练习 11：修复错误递归

```cpp
int countdown(int value) {
    if (value == 0) {
        return 0;
    }

    return countdown(value + 1);
}
```

### 问题

> 

### 修复后的函数

```cpp

```

### 使用负数调用时还需要考虑什么？

> 

---

## 练习 12：循环与递归对照

分别使用循环和递归计算 `1` 到 `value` 的乘积。规定 `value <= 0` 时返回 `1`；只测试不会超出 `int` 范围的小值。

### 循环版本

```cpp

```

### 递归版本

```cpp

```

### 两个版本的停止条件

> 

### 对很大的 `value`，递归版本还多出什么风险？

> 

---

## 练习 13：错误发生在哪一阶段

### 13.1 把字面量传给 `int&`

> 

### 13.2 两个重载都可行且没有唯一更优选择

> 

### 13.3 编译器已选中一个重载，但整个程序没有它的定义

> 

### 13.4 递归函数没有可达基例

> 

---

## 练习 14：独立编程——机器人校准工具

编写完整程序，至少包含：

```cpp
void clamp_angle(double& angle);
int calibration_cost(int joints, int cost_per_joint = 4);
double calibration_cost(double hours, double cost_per_hour);
int recursive_check_count(int joints);
```

要求：

1. `clamp_angle` 把角度限制到 `-180.0`～`180.0`
2. 两个 `calibration_cost` 构成重载
3. 至少一次省略默认实参，一次显式提供
4. `recursive_check_count` 使用递归计算 `1 + 2 + ... + joints`
5. 测试正常值、边界值和超出范围值

```cpp

```

### 测试 1：数据、预测、实际结果

> 

### 测试 2：数据、预测、实际结果

> 

### 测试 3：数据、预测、实际结果

> 

---

## 学习复盘

### 1. 值传递和引用传递最核心的区别

> 

### 2. 重载决议的三个基本阶段

> 

### 3. 默认实参为什么必须对调用点可见？

> 

### 4. 正确递归必须具备什么？

> 

### 5. 本单元最容易混淆的内容

> 

