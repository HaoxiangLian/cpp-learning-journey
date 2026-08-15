# 第 26 天｜求职版练习

> 配套讲义：[机器人设备模型综合训练](../days/第26天-机器人设备模型综合训练.md)

## 作答规则

- 先预测，再编译或运行；实际与预测不同就记录第一处差异。
- 所有 C++ 代码使用 C++17，并启用 `-Wall -Wextra -pedantic`；线程题额外使用 `-pthread`。
- 故意失败的程序要写清失败阶段、关键诊断、根因和验证证据。
- 每道题的答案直接写在题目下方。
- 四类任务全部达到验收标准，本单元才算完成。

---

## A｜基础概念检查

1. 为什么这里选择 `unique_ptr` 而不是 `shared_ptr`？

   > 

2. 哪些操作应该放进基类，哪些不应该？

   > 

3. 这个方案有什么限制，什么时候不该使用？

   > 

4. 用本课最小示例说明：哪个对象、文件或调用在什么语句前后发生了变化？

   > 

---

## B｜可运行编程任务

### 任务

完成 `DeviceManager::add` 和 `update_all`，加入 `Motor` 与 `Camera` 两种设备；空 `unique_ptr` 必须被拒绝。

### 起始代码

```cpp
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

class Device {
public:
    virtual ~Device() = default;
    virtual std::string name() const = 0;
    virtual void update() = 0;
};

class DeviceManager {
public:
    void add(std::unique_ptr<Device> device) {
        // TODO: 拒绝空所有者并转移进容器
    }

    void update_all() {
        // TODO: 通过统一接口更新
    }

private:
    std::vector<std::unique_ptr<Device>> devices_;
};

// TODO: Motor 和 Camera
// TODO: main
```

### 预期结果

```text
update motor
update camera
```

### 验收标准

- [ ] 管理器是设备唯一所有者
- [ ] `add` 的参数与 `std::move` 明确所有权转移
- [ ] 至少两种设备通过同一接口执行且空指针路径有测试

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
void add(Device* device) {
    devices_.push_back(std::unique_ptr<Device>{device});
}

// 调用者不清楚 add 是否接管，之后又 delete device
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

- 至少实现两种设备并通过同一接口更新
- 所有权图与代码一致
- 严格警告、ASan/UBSan 和边界测试通过

参考结构是“具体问题 → 对象或文件怎样变化 → 使用的规则 → 一个限制”，不要堆术语。

第一次口述提纲：

> 

面试官追问：

> 为什么这里选择 `unique_ptr` 而不是 `shared_ptr`？

你的回答：

> 

再追问：

> 哪些操作应该放进基类，哪些不应该？

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
