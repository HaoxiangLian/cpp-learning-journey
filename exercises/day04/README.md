# 第 4 天练习：函数与调用

先写预测和理由，再运行允许运行的代码。`broken/` 下的文件是故意错误示例，不要加入正常构建目标。答案位于 `solutions/day04/`。

## 一、概念判断

判断对错并写一句理由。

1. 形参出现在函数参数列表中，实参出现在具体调用表达式中。
2. 对 `int` 使用值传递时，函数内修改形参会自动修改调用方原对象。
3. 两个同名、同参数列表的函数只要返回类型不同，就能构成合法重载。
4. 默认实参会为每种省略方式生成一个新的重载函数。
5. 递归函数即使写了停止分支，也还要证明每次递归调用都向停止条件推进。

## 二、代码输出或错误分析

### 题 A：值传递与返回值

```cpp
#include <iostream>

int add_two(int value) {
    value = value + 2;
    return value;
}

int main() {
    int original{5};
    int result{add_two(original)};
    std::cout << original << ' ' << result << '\n';
}
```

写出精确输出，并指出哪个对象在函数内被修改。

### 题 B：重载选择

```cpp
#include <iostream>

void show(int) {
    std::cout << "int\n";
}

void show(double) {
    std::cout << "double\n";
}

int main() {
    show(2);
    show(2.5);
}
```

写出两行输出，并说明每次调用为什么存在唯一最佳匹配。

### 题 C：遗漏返回路径

```cpp
int classify(int value) {
    if (value > 0) {
        return 1;
    }
}
```

如果调用 `classify(0)`，应分类为编译错误、结果有定义但意外，还是未定义行为？编译器给出警告是否改变语言层面的分类？

## 三、小型编程题：任务评分函数

补全 [`mission_score.cpp`](mission_score.cpp)，满足以下要求：

1. `advance_tasks(int tasks)` 用值传递把形参加一并返回，不直接修改调用方原对象。
2. `mission_score(int completed_tasks, int points_per_task = 10)` 返回两者乘积。
3. 重载 `print_metric(int)` 与 `print_metric(double)`，分别输出 `metric(int):` 和 `metric(double):`。
4. `sum_to(int n)` 用递归计算从 `1` 到 `n` 的和，包含终止条件并让 `n` 逐次减小。
5. 输出必须与下方完全一致。

```text
original tasks: 3
advanced tasks: 4
score: 30
metric(int): 30
metric(double): 30.5
recursive sum: 6
```

构建命令：

```bash
c++ -std=c++17 -Wall -Wextra -Wpedantic -Werror mission_score.cpp -o mission_score
```

## 四、错误实验文件

- [`broken/ambiguous_overload.cpp`](broken/ambiguous_overload.cpp)：两个候选各自在一个参数位置更好，预期编译失败。
