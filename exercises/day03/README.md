# 第 3 天练习：表达式、转换与控制流

先写预测和理由，再运行允许运行的代码。`broken/` 下的文件是故意错误示例，不要加入正常构建目标。答案位于 `solutions/day03/`。

## 一、概念判断

判断对错并写一句理由。

1. 运算符优先级既决定表达式怎样分组，也足以决定所有子表达式的求值先后顺序。
2. `double result{5 / 2};` 中的除法先按 `int` 规则得到 `2`，再把结果转换成 `double`。
3. 写出 `static_cast<int>(value)` 就能证明转换无精度损失且一定符合业务需求。
4. 在 `left && right` 中，如果 `left` 为 `false`，则 `right` 不会求值。
5. `switch` 中故意省略 `break` 的贯穿只要语法合法就是未定义行为。

## 二、代码输出或错误分析

### 题 A：分组和类型

```cpp
#include <iostream>

int main() {
    double result{7 / 2 + 0.5};
    std::cout << result << '\n';
}
```

写出精确输出，并按顺序解释整数除法、加法和初始化。

### 题 B：短路保护

```cpp
#include <iostream>

int main() {
    int total{8};
    int count{0};
    bool usable{count != 0 && total / count > 2};
    if (usable) {
        std::cout << "yes\n";
    } else {
        std::cout << "no\n";
    }
}
```

写出输出。右侧除法是否执行？把 `&&` 两侧交换会引入哪一类问题？

### 题 C：赋值写进条件

```cpp
#include <iostream>

int main() {
    bool ready{false};
    if (ready = true) {
        std::cout << "ready\n";
    }
}
```

判断它属于编译错误、未定义行为，还是有定义但结果不符合预期。写出更直接的修复。

## 三、小型编程题：传感器读数筛选

补全 [`sensor_filter.cpp`](sensor_filter.cpp)，满足以下要求：

1. `temperature_celsius` 为 `31.8`，显式转换得到整数显示值 `31`。
2. 只有 `sensor_ready` 为真且温度不低于 `30.0` 时，`alarm` 才为真。
3. 用 `for` 遍历索引 `0` 到 `4`；索引为 `2` 时用 `continue` 跳过，其余计入 `processed_samples`。
4. 用 `switch` 处理 `mode == 2`，输出 `mode: inspect`；其他合理分支也必须用 `break` 收束。
5. 输出必须与下方完全一致。

```text
display: 31
alarm: on
processed samples: 4
mode: inspect
```

构建命令：

```bash
c++ -std=c++17 -Wall -Wextra -Wpedantic -Werror sensor_filter.cpp -o sensor_filter
```

## 四、错误实验文件

- [`broken/duplicate_case.cpp`](broken/duplicate_case.cpp)：同一个 `switch` 中存在重复 `case`，预期编译失败。
