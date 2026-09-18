# Day 22 练习：多文件构建与诊断工具

[返回讲义](../../days/day22.md) · [独立答案](../../solutions/day22/README.md)

先判断问题属于哪个阶段，再使用工具验证。不要先打开答案目录。

## A. 概念判断

判断并说明理由：

1. `#include "reading.hpp"` 会在运行时打开并读取头文件。
2. 一个 `.cpp` 及其经过预处理纳入的内容共同形成一个翻译单元。
3. 只要头文件有保护宏，就可以在其中随意放置非 `inline` 函数定义并被多个源文件包含。
4. CMake 的 target 表示一个待构建产物及其源文件、依赖和使用要求。
5. ASan 与 UBSan 能证明没有任何内存或未定义行为问题。

## B. 阶段诊断

分别判断最先失败的阶段，并写出第一步排查动作：

1. `main.cpp` 包含了不存在的 `sensor.hpp`。
2. `sensor.hpp` 声明 `int read();`，但最终参与链接的文件都没有定义它。
3. 程序成功生成，运行到 `values[3]` 时访问三元素数组边界外。
4. CMake 报告 `Cannot find source file: reading.cpp`。

## C. 编程练习

完成 [`telemetry.cpp`](telemetry.cpp)，并为 `main.cpp` 与 `telemetry.cpp` 写出一个最小 CMake target：

1. 目标采用 C++17。
2. 开启 `-Wall -Wextra -Wpedantic` 或编译器对应警告。
3. 构建后运行，输出 `temperature: 36.5 C`。
4. 在 `format_temperature` 内设置断点，观察形参 `celsius` 和返回字符串。
5. 再启用 ASan/UBSan 构建并运行同一程序。

## 故意错误实验

[`broken/include_twice.cpp`](broken/include_twice.cpp) 重复包含故意缺少保护的头文件，用于验证编译阶段的重定义诊断；它不属于正常构建目标。
