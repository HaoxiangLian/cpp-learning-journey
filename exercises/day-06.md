# 第 6 天：值传递、引用传递、函数重载、默认参数与递归｜求职版练习

## 作答规则

- 先预测，再编译或运行；实际结果与预测不同时，必须记录差异。
- 所有代码使用 C++17，并开启 `-Wall -Wextra -pedantic`。
- 故意失败的程序必须写明失败阶段、关键诊断和修复依据。
- 每道题的答案直接写在题目下方。
- 四类必做任务全部通过后，本单元才算完成。

---

## A｜基础概念检查

1. 为什么 `void f(int)` 与 `double f(int)` 不能构成重载？

   > 

2. 什么时候应使用 `const T&`，什么时候值传递反而更合适？

   > 

3. 结合本课最小代码，按顺序说明其中至少三个对象、值或文件发生了什么变化。

   > 

---

## B｜可运行编程任务

### 任务

实现 `apply_offset(double& angle, double offset)` 和 `clamp_value(double value, double lower, double upper)`；验证前者会修改调用者，后者返回新值。

### 起始代码

```cpp
#include <iostream>

void apply_offset(double& angle, double offset) {
    // 补全
}

double clamp_value(double value, double lower, double upper) {
    // 补全
}

int main() {
    double angle{175.0};
    apply_offset(angle, 10.0);
    std::cout << clamp_value(angle, -180.0, 180.0) << '\n';
}
```

### 预期结果

```text
180
```

### 验收标准

- [ ] 引用形参确实修改 `angle`
- [ ] 限幅函数所有返回路径都有返回值
- [ ] 能解释为什么 `offset` 使用值形参

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
void apply_offset(double angle, double offset) {
    angle += offset;
}

int main() {
    double angle{10.0};
    apply_offset(angle, 2.0);
    return angle == 12.0 ? 0 : 1;
}
```

### 调试要求

程序没有修改调用者。画出调用者对象和独立形参对象，修复为明确的引用接口。

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

> 为什么 `void f(int)` 与 `double f(int)` 不能构成重载？

答案必须能够回到本课实验中的具体事实，例如：

- `angle` 是非常量引用形参，绑定调用者对象，修改会反馈给调用者
- `offset` 是值形参，本次调用得到独立副本
- 只读且复制代价大的对象通常考虑 `const T&`
- 同名不同参数列表可以形成重载；只改返回类型不能区分调用

第一次口述提纲：

> 

面试追问：

> 什么时候应使用 `const T&`，什么时候值传递反而更合适？

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
