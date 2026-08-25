# 第 1 天答案与解析

## 一、概念判断

1. **错**。声明足以让调用形式被检查，但程序需要定义时仍必须找到相符定义。
2. **对**。定义属于声明的子集；`int f();` 可以只是声明，`int f() { return 1; }` 同时是声明和定义。
3. **错**。缺少分号会妨碍当前编译输入通过语法检查，通常是编译错误。
4. **错**。未定义行为表示标准不约束结果；崩溃、错误输出或表面正常都可能被观察到，不能断言固定结果。
5. **对**。目标文件与独立链接器是主流实现模型，不是标准规定的文件命名和流程外形。

## 二、代码分析

- **题 A**：可以构建，输出 `42`。`read_sensor` 在调用前已有完整定义。
- **题 B**：调用处能依据声明通过编译；完整程序需要定义时找不到它，通常在链接阶段失败。
- **题 C**：`syntax_error.cpp` 缺少分号，是编译错误；`missing_definition_main.cpp` 只有声明和调用，是链接错误。二者用于学习诊断，若进入正常目标会让 CI 永久失败，因此必须显著标注并排除。

## 三、小型编程题

单文件答案见 [`mission.cpp`](mission.cpp)。拆分版本应保持声明与定义一致：

```cpp
// mission.hpp
#ifndef CPP_LEARNING_DAY01_MISSION_HPP
#define CPP_LEARNING_DAY01_MISSION_HPP
int mission_code();
#endif
```

```cpp
// mission.cpp
#include "mission.hpp"
int mission_code() { return 17; }
```

```cpp
// main.cpp
#include "mission.hpp"
#include <iostream>
int main() {
    std::cout << "mission code: " << mission_code() << '\n';
    return 0;
}
```

如果只编译 `main.cpp` 而漏掉 `mission.cpp`，声明仍能帮助编译器检查调用，但链接会缺少定义。

## 深度思考题参考框架

1. 保留完整构建命令和首条链接诊断，确认失败确实发生在组合阶段。
2. 从未解析名称反查声明与定义的拼写和调用形式，排除名称或签名不一致。
3. 检查实现源文件或库是否属于当前构建目标，排除“写了实现但没有参与链接”。
4. 若使用构建系统，查看实际展开的编译/链接命令，而不是只看目录中是否存在某个文件。

这里给的是排查顺序，不表示链接错误只有这些原因。
