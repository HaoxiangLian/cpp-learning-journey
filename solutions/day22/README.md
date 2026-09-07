# Day 22 练习答案与解析

[返回练习](../../exercises/day22/README.md) · [返回讲义](../../days/day22.md)

## A. 概念判断

1. **错误。** `#include` 在翻译前的预处理阶段纳入文件内容，不是程序运行时的文件读取。
2. **正确。** 每个源文件分别经过预处理后成为一个翻译单元，再分别编译。
3. **错误。** 保护宏只避免同一翻译单元内重复纳入；多个翻译单元仍可能各自产生同一个不符合多重定义规则的定义。普通函数通常在头文件声明、在一个 `.cpp` 中定义。
4. **正确。** target 是 CMake 组织可执行文件或库及其源、依赖和使用要求的核心单位。
5. **错误。** Sanitizer 只能报告本次带插桩构建实际执行到且工具能够检测的问题，不能证明所有路径和错误类型都不存在。

## B. 阶段诊断

1. **预处理/编译入口失败。** 先核对包含路径、文件名大小写和 `target_include_directories`。
2. **链接失败。** 先确认定义签名完全一致，并确认实现文件或库已经加入正确 target 和链接依赖。
3. **运行时未定义行为。** 先用 ASan 运行能触发问题的测试，再检查索引合法范围；不应根据一次“没崩溃”判断安全。
4. **CMake 配置阶段失败。** 先按 `CMakeLists.txt` 所在目录核对相对路径以及文件是否进入版本库。

## C. 编程练习

参考实现：[`telemetry.cpp`](telemetry.cpp)。预期输出：

```text
temperature: 36.5 C
```

一个对应练习目录原文件的最小目标可以写成：

```cmake
cmake_minimum_required(VERSION 3.16)
project(day22_exercise LANGUAGES CXX)
add_executable(telemetry main.cpp telemetry.cpp)
target_compile_features(telemetry PRIVATE cxx_std_17)
target_compile_options(telemetry PRIVATE -Wall -Wextra -Wpedantic)
```

编译选项应按编译器条件设置；这里的短片段只用于 GCC/Clang 练习。调试时在 `format_temperature` 入口停下可见 `celsius == 36.5`；继续到返回前可观察构造出的字符串。

## 故意错误解析

预处理把 `unguarded.hpp` 的类定义纳入同一翻译单元两次，于是编译器看到两个同名类定义并拒绝程序。修复是在真实头文件中加入唯一保护宏（或在适用工具链中使用 `#pragma once`），同时仍要正确安排跨翻译单元的声明与定义。
