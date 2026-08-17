# 第 5 天练习：作用域、生命周期、存储期、链接属性与 `static`

先独立作答，不要提前查看 `solutions/day05/`。

## 一、概念判断

判断正误并用“名称、对象、存储、跨声明身份”中的准确主语解释。

1. 一个名称离开作用域，必然说明对应对象的生命周期同时结束。
2. 块作用域普通局部变量通常具有自动存储期，但 C++ 标准不要求它一定占用机器栈内存。
3. 命名空间作用域变量写上 `static` 后，主要变化是名称获得内部链接。
4. 局部静态变量只在函数块内可见，因此每次调用都会重新创建并初始化。
5. 类的静态数据成员是每个对象内部各自保存的一份成员子对象。

## 二、代码输出或错误分析

### A. 名称隐藏

```cpp
#include <iostream>

int main() {
    int mode{1};
    {
        int mode{4};
        ++mode;
        std::cout << mode << '\n';
    }
    std::cout << mode << '\n';
}
```

写出两行输出，并分别指出两次名称查找找到哪个声明。

### B. 局部静态状态

```cpp
#include <iostream>

int next_id() {
    static int id{40};
    return ++id;
}

int main() {
    std::cout << next_id() << ' ' << next_id() << '\n';
}
```

写出输出，并解释 `id` 的作用域、生命周期与存储期。

### C. 缺少静态数据成员定义

阅读 [`broken/missing_static_definition.cpp`](broken/missing_static_definition.cpp)。它被标为故意错误并排除在正常构建之外。判断失败阶段，说明类内声明提供了什么、还缺少什么，并给出两种 C++17 修复方式。

## 三、小型编程题：连续任务编号

补全 [`task_id.cpp`](task_id.cpp)：

1. `next_task_id()` 第一次返回 `501`，随后每次调用递增 `1`。
2. 计数状态只能通过该函数访问，不创建可修改的命名空间作用域全局变量。
3. `main` 连续调用三次并输出 `501 502 503`。
4. 再建立一个内层同名局部变量，证明它不会改变编号状态；名称要有清楚注释，避免把隐藏写成实际工程风格。

编译建议：

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror task_id.cpp -o task_id
./task_id
```

完成后再查看 [`../../solutions/day05/README.md`](../../solutions/day05/README.md)。
