# 第 1 天练习：构建阶段与多文件程序

先把答案写在自己的笔记或分支中，再查看 `solutions/day01/`。

## 一、概念判断

判断对错并写一句理由。

1. 只要一个函数有声明，完整程序就一定能成功构建。
2. 定义也是声明，但声明不一定是定义。
3. 源文件缺少分号通常属于链接错误。
4. 未定义行为一定会让操作系统报告段错误。
5. 主流工具链常把源文件分别编译后链接，但 C++ 标准不强制中间文件必须叫 `.o`。

## 二、代码输出或错误分析

### 题 A

```cpp
#include <iostream>

int read_sensor() {
    return 42;
}

int main() {
    std::cout << read_sensor() << '\n';
}
```

它能否按 C++17 构建？若能，输出是什么？

### 题 B

```cpp
int read_sensor();

int main() {
    return read_sensor();
}
```

若完整构建中没有其他文件定义 `read_sensor`，失败发生在哪个阶段？声明为什么仍有作用？

### 题 C

[`broken/syntax_error.cpp`](broken/syntax_error.cpp) 与 [`broken/missing_definition_main.cpp`](broken/missing_definition_main.cpp) 都被故意写错。分别预测失败阶段，并说明为什么它们不应进入正常 CMake 目标。

## 三、小型编程题

把 [`mission.cpp`](mission.cpp) 中的 `mission_code` 改为返回 `17`，使程序精确输出：

```text
mission code: 17
```

然后把它拆成三个文件：

- `mission.hpp`：函数声明；
- `mission.cpp`：函数定义；
- `main.cpp`：调用和输出。

使用下面命令构建，保留所有警告：

```bash
c++ -std=c++17 -Wall -Wextra -Wpedantic main.cpp mission.cpp -o mission
```

不要在练习目录提交生成的可执行文件。
