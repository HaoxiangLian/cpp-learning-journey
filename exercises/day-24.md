# 第 24 天｜求职版练习

> 配套讲义：[继承、虚函数与运行时多态](../days/第24天-继承、虚函数与运行时多态.md)

## 作答规则

- 先预测，再编译或运行；实际与预测不同就记录第一处差异。
- 所有 C++ 代码使用 C++17，并启用 `-Wall -Wextra -pedantic`；线程题额外使用 `-pthread`。
- 故意失败的程序要写清失败阶段、关键诊断、根因和验证证据。
- 每道题的答案直接写在题目下方。
- 四类任务全部达到验收标准，本单元才算完成。

---

## A｜基础概念检查

1. 虚函数调用的静态类型和动态类型分别有什么作用？

   > 

2. 为什么构造函数通常不用于普通虚分派，而析构函数却常需要是虚的？

   > 

3. 这个方案有什么限制，什么时候不该使用？

   > 

4. 用本课最小示例说明：哪个对象、文件或调用在什么语句前后发生了变化？

   > 

---

## B｜可运行编程任务

### 任务

新增一个 `TemperatureSensor`，让它和 `ForceSensor` 一起存入 `std::vector<std::unique_ptr<Sensor>>`，再通过基类接口输出名字和读数。

### 起始代码

```cpp
#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Sensor {
public:
    virtual ~Sensor() = default;
    virtual std::string name() const = 0;
    virtual double read() const = 0;
};

class ForceSensor final : public Sensor {
public:
    explicit ForceSensor(double value) : value_(value) {}
    std::string name() const override { return "force"; }
    double read() const override { return value_; }
private:
    double value_;
};

// TODO: 定义 TemperatureSensor

int main() {
    std::vector<std::unique_ptr<Sensor>> sensors;
    // TODO: 加入 force=12.5 和 temperature=36.5
    // TODO: 通过 Sensor 接口遍历输出
}
```

### 预期结果

```text
force = 12.5
temperature = 36.5
```

### 验收标准

- [ ] 容器拥有两个动态类型不同的对象
- [ ] 没有裸 `new`/`delete`
- [ ] 基类析构为 virtual，覆盖函数写 `override`

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
class Base {
public:
    ~Base() = default; // 非虚
};

class Derived : public Base {
public:
    ~Derived() { /* 释放派生资源 */ }
};

Base* value = new Derived;
delete value; // 未定义行为
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

- 能够画出基类指针、完整派生对象和所有者关系
- 能够解释一次虚调用选择了哪个实现
- 能识别对象切片和非虚析构删除问题

参考结构是“具体问题 → 对象或文件怎样变化 → 使用的规则 → 一个限制”，不要堆术语。

第一次口述提纲：

> 

面试官追问：

> 虚函数调用的静态类型和动态类型分别有什么作用？

你的回答：

> 

再追问：

> 为什么构造函数通常不用于普通虚分派，而析构函数却常需要是虚的？

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
