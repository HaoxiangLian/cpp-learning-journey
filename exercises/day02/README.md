# 第 2 天练习：对象、类型、初始化与赋值

先写预测和理由，再运行允许运行的代码。`broken/` 下的文件是故意错误示例，不要加入正常构建目标。答案位于 `solutions/day02/`。

## 一、概念判断

判断对错并写一句理由。

1. 在今天讨论的普通声明中，变量名称用于访问对象，但对象不一定都有名称。
2. 函数体内的 `int sample_count;` 一定自动得到值 0。
3. `int sample_count = 0;` 中出现了 `=`，所以它执行的是赋值。
4. `const` 变量仍然是变量，只是相应对象不能被普通赋值修改。
5. `const int id{read_id()};` 一定不合法，因为 `read_id()` 的结果可能到运行时才得到。

## 二、代码输出或错误分析

### 题 A：对象之间是否互相影响

```cpp
#include <iostream>

int main() {
    int first_count{3};
    int second_count{first_count};
    second_count = 7;
    std::cout << first_count << ' ' << second_count << '\n';
}
```

写出精确输出，并说明给 `second_count` 赋值为什么不会改变 `first_count`。

### 题 B：窄化

```cpp
int main() {
    int temperature{20.5};
    return temperature;
}
```

判断是否能够通过 C++17 编译，给出错误分类和符合“温度需要小数”需求的修复。

### 题 C：未初始化读取

```cpp
#include <iostream>

int main() {
    int sample_count;
    std::cout << sample_count << '\n';
}
```

不要运行。判断它属于编译错误、定义明确但结果意外，还是未定义行为。能否预测输出为 0？

## 三、小型编程题：传感器状态快照

补全 [`sensor_snapshot.cpp`](sensor_snapshot.cpp)，满足以下要求：

1. `sensor_id` 的类型为 `const int`，初始值为 `12`。
2. `sample_count` 的类型为 `int`，初始值为 `0`。
3. `position_meter` 的类型为 `double`，初始值为 `1.25`。
4. 输出第一次状态后，把采样次数赋值为 `1`，位置赋值为 `1.5`。
5. 不修改 `sensor_id`。

精确输出：

```text
sensor: 12
sample 0 position: 1.25 m
sample 1 position: 1.5 m
```

构建命令：

```bash
c++ -std=c++17 -Wall -Wextra -Wpedantic -Werror sensor_snapshot.cpp -o sensor_snapshot
```

## 四、错误实验文件

- [`broken/narrowing.cpp`](broken/narrowing.cpp)：预期编译失败。
- [`broken/const_assignment.cpp`](broken/const_assignment.cpp)：预期编译失败。
- [`broken/uninitialized_read.cpp`](broken/uninitialized_read.cpp)：含未定义行为，不要把一次运行结果当作答案。
