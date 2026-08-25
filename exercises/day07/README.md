# 第 7 天练习：数组、字符串与动态内存

先独立作答，不要提前查看 `solutions/day07/`。

## 一、概念判断

1. 数组和指针是同一种类型，因此 `sizeof(array)` 与 `sizeof(pointer)` 总是相同。
2. 原生数组元素按标准保证连续存储，但内建下标操作不会自动抛出越界异常。
3. 任意 `char` 数组都可以安全传给要求 C 风格字符串的函数。
4. `new int[4]` 必须与 `delete[]` 配对；用普通 `delete` 是未定义行为。
5. 动态对象的唯一裸指针离开作用域后，对象会自动释放。

## 二、代码输出或错误分析

### A. 数组不是指针

```cpp
#include <iostream>

int main() {
    int values[4]{1, 2, 3, 4};
    int* pointer{values};
    std::cout << sizeof(values) / sizeof(values[0]) << ' '
              << (pointer == &values[0]) << '\n';
}
```

写出输出，并说明两个 `sizeof` 运算为什么仍在数组可见处使用。

### B. 终止字符

```cpp
char first[]{"imu"};
char second[3]{'i', 'm', 'u'};
```

分别写出数组元素数量，判断哪个是有效 C 风格字符串。若把 `second` 交给 `std::strlen`，错误类别是什么？

### C. 释放错误

分析下面三段，不要运行故意错误代码：

```cpp
int* a{new int[3]{}};
delete a;

int* b{new int{7}};
delete b;
delete b;

int* c{new int{9}};
delete c;
int value{*c};
```

分别标注错误类型，并说明为什么不能预测固定症状。阅读 [`broken/mismatched_delete.cpp`](broken/mismatched_delete.cpp) 中的醒目标记。

## 三、小型编程题：动态采样缓冲区

补全 [`sample_buffer.cpp`](sample_buffer.cpp)：

1. 使用 `std::string sensor{"lidar"}` 保存传感器名。
2. 使用 `new int[count]` 创建 4 个元素的动态数组，并初始化为 `3, 6, 9, 12`。
3. 使用严格 `[0, count)` 循环求和。
4. 用匹配形式释放一次，并把当前指针设为 `nullptr`。
5. 输出 `lidar sum: 30` 与 `released: true`。

这道题故意练习手动配对；实际业务代码应在后续课程中改用容器或智能指针。

编译建议：

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror sample_buffer.cpp -o sample_buffer
./sample_buffer
```

完成后再查看 [`../../solutions/day07/README.md`](../../solutions/day07/README.md)。
