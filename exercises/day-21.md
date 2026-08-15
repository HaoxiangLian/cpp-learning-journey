# 第 21 天：拷贝构造、拷贝赋值与 Rule of Three｜求职版练习

## 作答规则

- 先预测，再编译或运行；实际结果与预测不同时，必须记录差异。
- 所有代码使用 C++17，并开启 `-Wall -Wextra -pedantic`。
- 故意失败的程序必须写明失败阶段、关键诊断和修复依据。
- 每道题的答案直接写在题目下方。
- 四类必做任务全部通过后，本单元才算完成。

---

## A｜基础概念检查

1. 复制构造与复制赋值在对象状态上有什么区别？

   > 

2. 浅复制为什么可能导致重复释放？

   > 

3. 结合本课最小代码，按顺序说明其中至少三个对象、值或文件发生了什么变化。

   > 

---

## B｜可运行编程任务

### 任务

实现一个以 `std::vector<int>` 为成员的 `Buffer`，验证默认复制产生两个独立序列，并说明为什么这是 Rule of Zero。

### 起始代码

```cpp
#include <iostream>
#include <vector>

class Buffer {
public:
    explicit Buffer(std::size_t size) : data_(size) {}
    int& operator[](std::size_t i) { return data_.at(i); }
private:
    std::vector<int> data_;
};

int main() {
    Buffer first{3};
    Buffer second{first};
    // 修改 second，验证 first 不变
}
```

### 预期结果

```text
修改副本不改变原对象
```

### 验收标准

- [ ] 不手写析构和复制操作
- [ ] 能指出复制构造发生在哪一行
- [ ] 能解释 `vector` 成员为什么提供正确深复制语义

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
class Buffer {
public:
    explicit Buffer(std::size_t n) : data_{new int[n]}, size_{n} {}
    ~Buffer() { delete[] data_; }
private:
    int* data_{};
    std::size_t size_{};
};

Buffer second = first;
```

### 调试要求

默认复制只复制哪个成员值？使用 ASan 观察重复释放；给出深复制、禁止复制或改用 `vector` 三种设计方向。

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

> 复制构造与复制赋值在对象状态上有什么区别？

答案必须能够回到本课实验中的具体事实，例如：

- 复制构造为 `second` 建立独立资源并复制元素
- 复制赋值先准备新资源，再安全替换 `third` 的旧资源
- 编译器生成的逐成员复制对裸拥有指针只复制地址，形成浅复制
- 自赋值时必须保持对象有效

第一次口述提纲：

> 

面试追问：

> 浅复制为什么可能导致重复释放？

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
