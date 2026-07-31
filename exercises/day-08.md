# 第 8 天练习单

主题：枚举、类型别名、命名空间与类对象  
语言标准：C++17

## 作答方式

- 直接在每道题下方的空白区填写答案。
- 预测题先写预测，再编译运行并记录实际结果。
- 代码题在紧随题目的空 `cpp` 代码块中完成。
- 错误实验保留关键诊断，并判断错误发生阶段。

---

## 练习 1：区分类型、对象与成员

阅读：

```cpp
struct JointState {
    int id{};
    double angle{};
};

JointState shoulder{1, 30.0};
```

### 1.1 `JointState` 是什么？

> 

### 1.2 `shoulder` 是什么？

> 

### 1.3 `shoulder.angle` 是什么？

> 

### 1.4 类定义本身是否创建了 `JointState` 对象？

> 

---

## 练习 2：预测限定枚举

```cpp
#include <iostream>

enum class Mode {
    idle,
    running,
    fault
};

int main() {
    Mode mode{Mode::running};

    if (mode == Mode::running) {
        std::cout << "Robot running\n";
    } else {
        std::cout << "Robot stopped\n";
    }
    return 0;
}
```

### 2.1 预测

> 

### 2.2 实际结果

> 

### 2.3 `Mode::running` 中两个名称分别表示什么？

> 

---

## 练习 3：设计机器人状态枚举

定义 `enum class RobotState`，包含：

- `offline`
- `initializing`
- `ready`
- `moving`
- `fault`

创建一个值为 `ready` 的对象。

```cpp

```

### 为什么这里优先使用 `enum class`，而不是 `int`？

> 

---

## 练习 4：枚举与整数的边界

阅读：

```cpp
enum class Mode {
    idle = 0,
    running = 1
};

Mode mode{Mode::running};
```

### 4.1 `mode == 1` 能否通过编译？

> 

### 4.2 这种限制防止了什么问题？

> 

### 4.3 若确实需要取得整数值，应怎样明确表达？

```cpp

```

---

## 练习 5：类型别名不是新类型

```cpp
using JointId = int;
using ErrorCode = int;
```

### 5.1 `JointId` 和 `ErrorCode` 是两个不同类型吗？

> 

### 5.2 编译器能否阻止把 `ErrorCode` 传给接收 `JointId` 的函数？

> 

### 5.3 类型别名的主要价值是什么？

> 

---

## 练习 6：类型别名与函数重载

```cpp
using JointId = int;

void inspect(int value) {
}

void inspect(JointId value) {
}
```

### 6.1 预测编译结果

> 

### 6.2 实际诊断

> 

### 6.3 为什么这不是两个合法重载？

> 

---

## 练习 7：使用命名空间解决名称冲突

分别在 `robot` 和 `simulator` 命名空间中定义 `Mode`：

- `robot::Mode` 包含 `idle`、`running`
- `simulator::Mode` 包含 `paused`、`realtime`

在 `main` 中各创建一个对象，必须使用限定名称。

```cpp

```

### 两个 `Mode` 为什么可以同时存在？

> 

---

## 练习 8：限定名称与非限定名称

```cpp
namespace robotics {
    enum class Mode { idle, running };
}
```

### 8.1 写出类型 `Mode` 的完整限定名称

> 

### 8.2 写出枚举项 `running` 的完整限定名称

> 

### 8.3 为什么初学阶段优先使用完整限定名称？

> 

### 8.4 命名空间是否是运行时对象？

> 

---

## 练习 9：创建两个独立的结构体对象

定义：

```cpp
struct SensorReading {
    int id{};
    double value{};
    bool valid{};
};
```

创建两个不同对象，修改第一个对象的 `value`，输出两者全部成员，证明状态彼此独立。

```cpp

```

### 预测

> 

### 实际结果

> 

---

## 练习 10：跟踪聚合初始化

```cpp
struct JointState {
    int id{};
    double angle{};
    bool enabled{};
};

JointState joint{3, 45.5, true};
```

### 10.1 三个初始化值分别对应哪个成员？

> 

### 10.2 对应顺序由什么决定？

> 

### 10.3 是否可以断言所有 `struct` 永远都能这样初始化？

> 

---

## 练习 11：预测对象复制

```cpp
#include <iostream>

struct JointState {
    int id{};
    double angle{};
};

int main() {
    JointState original{1, 20.0};
    JointState copied{original};
    copied.angle = 80.0;

    std::cout << original.angle << '\n';
    std::cout << copied.angle << '\n';
    return 0;
}
```

### 11.1 预测

> 

### 11.2 实际结果

> 

### 11.3 为什么修改 `copied.angle` 没有修改 `original.angle`？

> 

### 11.4 能否据此断言所有类复制都只是安全复制每个字节？

> 

---

## 练习 12：`struct` 与 `class`

### 12.1 二者是否都能定义类类型？

> 

### 12.2 二者的默认成员访问权限分别是什么？

> 

### 12.3 二者的默认继承权限分别是什么？

> 

### 12.4 “`struct` 只能存数据，`class` 才能有函数”是否正确？

> 

---

## 练习 13：建立最小公开接口

编写 `class Motor`：

1. 私有成员 `bool enabled_{}`
2. 公开成员函数 `void enable()`
3. 公开成员函数 `void disable()`
4. 公开只读成员函数 `bool is_enabled() const`
5. 在 `main` 中通过公开接口切换并输出状态

```cpp

```

### 为什么类外不能直接访问 `enabled_`？

> 

### 实际结果

> 

---

## 练习 14：错误实验——限定枚举与整数比较

编译：

```bash
g++ -std=c++17 -Wall -Wextra -pedantic \
    examples/day-08/scoped_enum_error.cpp \
    -o scoped_enum_error
```

### 14.1 是否成功生成可执行文件？

> 

### 14.2 关键诊断

> 

### 14.3 错误发生在预处理、狭义编译、汇编、链接还是运行阶段？

> 

### 14.4 修复代码，使程序通过枚举项进行比较

```cpp

```

---

## 练习 15：对象大小与填充

对下面类型输出 `sizeof(JointState)`：

```cpp
struct JointState {
    int id{};
    double angle{};
    bool enabled{};
};
```

### 当前平台输出

> 

### 为什么不能简单断言结果等于三个成员大小之和？

> 

### 为什么不能把当前结果推广到所有平台和编译器？

> 

---

## 练习 16：完整编程——机器人状态模型

编写一个完整程序：

1. 所有类型放入 `robotics` 命名空间
2. 使用 `using JointId = int`
3. 使用 `enum class Mode` 表示至少四种运行状态
4. 使用 `struct JointState` 保存编号、角度和使能状态
5. 使用 `class Robot` 保存名称、当前模式和关节序列
6. 数据成员设为 `private`
7. 通过 `public` 成员函数修改和读取状态
8. 创建至少两个关节对象
9. 输出机器人状态和全部关节状态
10. 复制一个关节对象并修改副本，证明原对象未改变
11. 使用严格警告编译并记录结果

```cpp

```

### 测试数据与预测

> 

### 实际结果

> 

### 编译警告

> 

---

## 学习复盘

### 1. 类型、对象、名称和成员之间是什么关系？

> 

### 2. `enum class` 相比整数状态码解决了什么问题？

> 

### 3. 为什么类型别名不能提供真正的类型隔离？

> 

### 4. 命名空间解决的主要问题是什么？

> 

### 5. `struct` 和 `class` 的语言差异与工程习惯分别是什么？

> 

### 6. 一个类的不同对象是否共享普通非静态数据成员？

> 

### 7. 本单元暂缓到后续的内容有哪些？

> 

### 8. 本单元最容易混淆的内容

> 
