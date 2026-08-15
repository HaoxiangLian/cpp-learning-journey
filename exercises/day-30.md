# 第 30 天｜求职版练习

> 配套讲义：[标准算法、Lambda 与可调用对象](../days/第30天-标准算法、Lambda与可调用对象.md)

## 作答规则

- 先预测，再编译或运行；实际与预测不同就记录第一处差异。
- 所有 C++ 代码使用 C++17，并启用 `-Wall -Wextra -pedantic`；线程题额外使用 `-pthread`。
- 故意失败的程序要写清失败阶段、关键诊断、根因和验证证据。
- 每道题的答案直接写在题目下方。
- 四类任务全部达到验收标准，本单元才算完成。

---

## A｜基础概念检查

1. `end()` 为什么不能解引用？

   > 

2. `[limit]` 与 `[&limit]` 在生命周期上有什么区别？

   > 

3. 为什么 `sort` 需要随机访问迭代器而 `find` 不需要？

   > 

4. 用本课最小示例说明：哪个对象、文件或调用在什么语句前后发生了变化？

   > 

---

## B｜可运行编程任务

### 任务

对温度向量排序，再用 `count_if` 统计超过 80 的值，并用 `transform` 生成华氏温度；业务条件写在 Lambda 中。

### 起始代码

```cpp
#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::vector<double> celsius{20.0, 100.0, 85.0};
    // TODO: sort
    // TODO: count_if(value > 80)
    std::vector<double> fahrenheit(celsius.size());
    // TODO: transform，公式 value * 9 / 5 + 32
    // TODO: 输出
}
```

### 预期结果

```text
sorted C: 20 85 100
above 80 = 2
F: 68 185 212
```

### 验收标准

- [ ] 算法区间均为 `[begin, end)`
- [ ] Lambda 捕获只包含确实需要的外层状态
- [ ] 输出向量在 transform 前已具有正确大小

### 你的实现

```cpp

```

使用的构建命令：

```bash

```

运行前的输出预测：

> 

实际输出：

```text

```

预测与实际的第一处差异：

> 

---

## C｜调试任务

### 故障代码

```cpp
#include <algorithm>
#include <vector>

int main() {
    std::vector<double> values{9.0, 12.5};
    double limit = 10.0;
    auto count = std::count_if(
        values.begin(), values.end(),
        [](double value) { return value > limit; });
}
```

### 调试要求

先不要修改。判断问题首先出现在编译、链接、运行、结果还是接口设计；运行或构建后，只保留最关键的诊断。修复时说明为什么最小修改能针对根因，而不是只让报错消失。

运行或构建前的预测：

> 

问题首先出现的阶段：

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

修复后的验证输入与结果：

```text

```

---

## D｜求职面试口述

先用 30～60 秒回答本课核心问题。回答必须包含：

- 能用 `sort` 与 `count_if` 完成数据处理
- 能解释 `[first, last)`
- 能写出不会悬空的 Lambda 捕获

参考结构是“具体问题 → 对象或文件怎样变化 → 使用的规则 → 一个限制”，不要堆术语。

第一次口述提纲：

> 

面试官追问：

> `end()` 为什么不能解引用？

你的回答：

> 

再追问：

> `[limit]` 与 `[&limit]` 在生命周期上有什么区别？

你的回答：

> 

删掉所有不能回到代码、输出、诊断或测试证据的句子，再写第二版提纲：

> 

---

## 学习复盘

今天最清楚的机制：

> 

仍然不能通过代码解释的地方：

> 

我能否在不看讲义时复现程序和故障：

> 

本单元是否达到四类任务的验收标准：

> 
