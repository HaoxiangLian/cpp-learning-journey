# 第 8 天：枚举、类型别名、命名空间与类对象｜求职版练习

## 作答规则

- 先预测，再编译或运行；实际结果与预测不同时，必须记录差异。
- 所有代码使用 C++17，并开启 `-Wall -Wextra -pedantic`。
- 故意失败的程序必须写明失败阶段、关键诊断和修复依据。
- 每道题的答案直接写在题目下方。
- 四类必做任务全部通过后，本单元才算完成。

---

## A｜基础概念检查

1. `enum class` 相比普通整数状态码解决了什么问题？

   > 

2. 类型、对象和成员分别是什么？

   > 

3. 结合本课最小代码，按顺序说明其中至少三个对象、值或文件发生了什么变化。

   > 

---

## B｜可运行编程任务

### 任务

定义 `robot::Mode` 和 `robot::JointState`，编写 `print_state` 输出编号、角度和模式。

### 起始代码

```cpp
#include <iostream>
#include <string>

namespace robot {

enum class Mode { idle, running, fault };

struct JointState {
    int id{};
    double angle{};
    Mode mode{Mode::idle};
};

// 编写 print_state

} // namespace robot

int main() {
    robot::JointState shoulder{1, 30.0, robot::Mode::running};
    robot::print_state(shoulder);
}
```

### 预期结果

```text
Joint 1: 30, running
```

### 验收标准

- [ ] 枚举项使用限定名称
- [ ] 函数参数不复制对象
- [ ] 名称位于 `robot` 命名空间
- [ ] 能区分类型 `JointState` 和对象 `shoulder`

### 你的实现

```cpp

```

使用的构建命令：

```bash

```

实际输出：

```text

```

预测与实际的差异：

> 

---

## C｜调试任务

### 故障代码

```cpp
enum class Mode { idle, running };

int main() {
    Mode mode{Mode::running};
    return mode == 1 ? 0 : 1;
}
```

### 调试要求

记录限定枚举与整数直接比较的编译诊断。修复时不要把状态退化回随意整数。

运行或构建前的预测：

> 

错误发生的阶段：

> 

关键诊断或异常现象：

```text

```

根本原因：

> 

修复后的代码：

```cpp

```

为什么这个修复有效：

> 

---

## D｜求职面试口述

先用 30～60 秒回答：

> `enum class` 相比普通整数状态码解决了什么问题？

答案必须能够回到本课实验中的具体事实，例如：

- `Mode` 是新类型，只允许有限的命名状态
- `JointState` 把同一关节的相关数据组成一个类类型
- `robot::JointState` 中的 `robot::` 通过命名空间限定名称
- `JointState shoulder{...};` 才创建具体对象

第一次口述提纲：

> 

面试追问：

> 类型、对象和成员分别是什么？

你的回答：

> 

删掉任何只能背诵、不能由代码或诊断推导出的句子，再写第二版提纲：

> 

---

## 学习复盘

今天最清楚的机制：

> 

仍然不能通过代码解释的地方：

> 

本单元是否达到四类任务的验收标准：

> 
