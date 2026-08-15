# 第 23 天：智能指针与所有权｜求职版练习

## 作答规则

- 先预测，再编译或运行；实际结果与预测不同时，必须记录差异。
- 所有代码使用 C++17，并开启 `-Wall -Wextra -pedantic`。
- 故意失败的程序必须写明失败阶段、关键诊断和修复依据。
- 每道题的答案直接写在题目下方。
- 四类必做任务全部通过后，本单元才算完成。

---

## A｜基础概念检查

1. 什么时候不应该使用 `shared_ptr`？

   > 

2. `weak_ptr::lock()` 为什么比先 `expired()` 再访问更合适？

   > 

3. 结合本课最小代码，按顺序说明其中至少三个对象、值或文件发生了什么变化。

   > 

---

## B｜可运行编程任务

### 任务

为三个接口选择并实现合适所有权：工厂创建独占设备、只读观察设备、缓存弱观察共享节点。

### 起始代码

```cpp
#include <memory>
#include <string>

struct Device { std::string name; };

std::unique_ptr<Device> make_device(std::string name);
void print_device(const Device& device);
std::shared_ptr<Device> lock_device(const std::weak_ptr<Device>& view);
```

### 预期结果

```text
独占设备可移动不可复制；观察函数不延长生命周期；弱观察在对象销毁后返回空
```

### 验收标准

- [ ] 默认使用 `make_unique`/`make_shared`
- [ ] 没有从同一裸指针建立两个控制块
- [ ] 使用 `weak_ptr::lock()` 后检查结果
- [ ] 能画出每个对象的所有权边

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
Device* raw{new Device{"arm"}};
std::shared_ptr<Device> first{raw};
std::shared_ptr<Device> second{raw};
```

### 调试要求

解释为什么两个 `shared_ptr` 看见同一地址却不共享控制块；使用 ASan 观察重复释放，并改为从一个 `shared_ptr` 复制。

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

> 什么时候不应该使用 `shared_ptr`？

答案必须能够回到本课实验中的具体事实，例如：

- `unique_ptr` 表达唯一所有者，可移动、不可复制
- `observer` 只是观察者，不能延长对象生命周期
- `shared_ptr` 仅用于确实有共同存活责任的组件
- `weak_ptr` 观察共享所有权组，使用前通过 `lock()` 临时取得所有者

第一次口述提纲：

> 

面试追问：

> `weak_ptr::lock()` 为什么比先 `expired()` 再访问更合适？

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
