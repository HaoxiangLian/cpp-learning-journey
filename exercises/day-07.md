# 第 7 天练习单

主题：原生数组、`std::string` 与 `std::vector`  
语言标准：C++17

## 作答方式

- 直接在每道题下方的空白区作答。
- 预测题先预测，再编译运行并记录实际结果。
- 代码题在紧随题目的空 `cpp` 代码块中完成。
- 错误实验保留关键诊断，并判断发生阶段。

---

## 练习 1：类型与用途

分别填写类型类别、元素数量能否在创建后改变，以及主要用途。

### 1.1 `double angles[4]{}`

> 

### 1.2 `std::string robot_name{}`

> 

### 1.3 `std::vector<double> commands{}`

> 

### 1.4 为什么不能把三者都简单称作“数组”？

> 

---

## 练习 2：预测原生数组初始化

```cpp
#include <iostream>
#include <iterator>

int main() {
    int values[5]{7, 8};

    for (std::size_t index{0}; index < std::size(values); ++index) {
        std::cout << values[index] << ' ';
    }
    std::cout << '\n';
    return 0;
}
```

### 2.1 预测

> 

### 2.2 实际结果

> 

### 2.3 为什么后三个元素具有这些值？

> 

---

## 练习 3：编写原生数组遍历

创建一个包含 6 个关节编号的 `int` 原生数组，使用 `std::size` 和基于下标的 `for` 输出：

```text
Joint 0: ...
Joint 1: ...
...
```

不能把元素个数 `6` 再写进循环条件。

```cpp

```

### 预测

> 

### 实际结果

> 

---

## 练习 4：`std::string` 的大小和下标

```cpp
#include <iostream>
#include <string>

int main() {
    std::string name{"Robot"};
    name[0] = 'r';

    std::cout << name << '\n';
    std::cout << name.size() << '\n';
    std::cout << name[1] << '\n';
    return 0;
}
```

### 4.1 预测

> 

### 4.2 实际结果

> 

### 4.3 `name.size()` 返回的是最后一个下标吗？

> 

### 4.4 为什么不能普遍把 UTF-8 字符串的 `size()` 当作人看到的字符数？

> 

---

## 练习 5：跟踪 `vector` 的变化

```cpp
std::vector<int> values{10, 20};
values.push_back(30);
values[0] = 15;
```

### 5.1 每条语句执行后的元素序列和 `size()`

> 

### 5.2 `capacity()` 是否必须始终等于 `size()`？

> 

### 5.3 可以访问 `[size(), capacity())` 中的位置吗？为什么？

> 

---

## 练习 6：预测范围 `for`

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<int> values{1, 2, 3};

    for (int value : values) {
        value = value * 10;
    }

    for (int value : values) {
        std::cout << value << ' ';
    }
    std::cout << '\n';
    return 0;
}
```

### 6.1 预测

> 

### 6.2 实际结果

> 

### 6.3 第一个循环为什么没有修改元素？

> 

### 6.4 把第一个循环改成真正修改元素

```cpp

```

---

## 练习 7：三种范围循环变量

说明每种写法中循环变量与元素的关系。

### 7.1 `for (double value : values)`

> 

### 7.2 `for (double& value : values)`

> 

### 7.3 `for (const double& value : values)`

> 

### 7.4 对小型 `double` 只读遍历时，是否必须使用 `const double&`？

> 

---

## 练习 8：找出循环越界

```cpp
std::vector<int> values{10, 20, 30};

for (std::size_t index{0}; index <= values.size(); ++index) {
    std::cout << values[index] << '\n';
}
```

### 8.1 哪一次迭代首次越界？

> 

### 8.2 为什么 `index == values.size()` 不表示最后一个元素？

> 

### 8.3 修复后的循环

```cpp

```

---

## 练习 9：`operator[]` 与 `at()`

对大小为 3 的 `vector` 访问下标 3。

### 9.1 `values[3]` 的边界行为

> 

### 9.2 `values.at(3)` 的边界行为

> 

### 9.3 为什么“`values[3]` 这次输出了一个数字”不能证明代码正确？

> 

---

## 练习 10：越界错误实验

编译并运行 `examples/day-07/bounds_error.cpp`：

```bash
g++ -std=c++17 -Wall -Wextra -pedantic \
    -fsanitize=address,undefined -fno-omit-frame-pointer \
    examples/day-07/bounds_error.cpp \
    -o bounds_error
```

### 10.1 程序是否能通过编译和链接？

> 

### 10.2 Sanitizer 的关键诊断

> 

### 10.3 错误实际发生在哪个阶段？

> 

### 10.4 使用下标检查修复

```cpp

```

---

## 练习 11：安全访问函数

编写：

```cpp
void print_if_valid(const std::vector<double>& values, std::size_t index);
```

若下标合法，输出元素；否则输出：

```text
Invalid index
```

```cpp

```

### 测试数据与预测

> 

### 实际结果

> 

---

## 练习 12：修改所有关节角度

编写：

```cpp
void add_offset(std::vector<double>& angles, double offset);
```

使用范围 `for` 和引用，把每个元素增加 `offset`。

```cpp

```

### 为什么形参和循环变量都需要引用？

> 

### 实际结果

> 

---

## 练习 13：原生数组函数形参陷阱

```cpp
void inspect(double values[4]);
```

### 13.1 这个形参能否保证调用者传入的数组一定有 4 个元素？

> 

### 13.2 函数形参调整后，它本质上是哪类形参？

> 

### 13.3 为什么函数内部不能用这个形参恢复原数组界限？

> 

---

## 练习 14：`vector` 重分配风险

```cpp
std::vector<double> values{10.0, 20.0};
double& first{values[0]};
values.push_back(30.0);
```

### 14.1 `push_back` 可能改变哪两个状态？

> 

### 14.2 什么情况下 `first` 可能失效？

> 

### 14.3 为什么不能假定每次容量都恰好翻倍？

> 

---

## 练习 15：独立编程——机器人关节序列分析器

编写完整程序，要求：

1. 用原生数组保存 4 个固定的关节编号
2. 用 `std::string` 保存机器人名称并逐字符输出
3. 用 `std::vector<double>` 保存至少 4 个关节角度
4. 编写只读函数计算平均角度
5. 编写引用函数为全部角度增加偏移
6. 让用户输入一个下标，检查合法后再访问
7. 测试合法下标、等于 `size()` 的下标和明显更大的下标
8. 使用严格警告和 Sanitizer 验证

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

### 1. 原生数组、`std::string`、`std::vector` 最核心的类型差异

> 

### 2. 为什么合法下标范围写成 `[0, size())`？

> 

### 3. `size()` 与 `capacity()` 分别表示什么？

> 

### 4. 范围 `for` 中按值与按引用有什么区别？

> 

### 5. 本单元仍留到后续补齐的内容

> 

### 6. 本单元最容易混淆的内容

> 

