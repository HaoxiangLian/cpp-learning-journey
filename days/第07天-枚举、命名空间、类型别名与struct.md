# 第 7 天：枚举、命名空间、类型别名与 struct

预计用时：60～90 分钟  
今日目标：把分散的机器人状态数据组织成有明确含义的类型，并避免名称冲突和不同类型的状态相互混用。

## 1. 先运行机器人状态程序

[robot_state.cpp](../examples/day-07/robot_state.cpp) 定义了一个关节状态模型：

```cpp
namespace robot {

using JointId = int;

enum class MotionState {
    idle,
    moving,
    fault
};

struct JointState {
    JointId id;
    double angle;
    MotionState motion;
};

}  // namespace robot
```

程序输出：

```text
joint 2, angle 45, MOVING
```

## 2. 关键术语索引

| 术语 | 本课用它解决什么问题 |
|---|---|
| [类型别名](#类型别名) | 给现有类型使用更符合领域的名字 |
| [命名空间](#命名空间) | 组织名称并避免冲突 |
| [限定枚举](#限定枚举) | 只允许一组明确的状态值 |
| [struct](#struct) | 把相关数据组合成一个类型 |
| [成员](#成员) | 访问组合类型中的具体数据或行为 |

### 类型别名

**类型别名是现有类型的另一个名称，不会创建新的独立类型。**

```cpp
using JointId = int;
```

`JointId` 让接口意图比裸 `int` 更清楚，但它仍然就是 `int`。因此不能依靠 `JointId` 和 `int` 构成两个重载，也不会阻止把普通整数传给需要 `JointId` 的位置。

### 命名空间

**命名空间是组织名称并建立命名作用域的语言机制。**

`robot::JointState` 使用限定名称，明确指出 `JointState` 来自 `robot`。另一个库即使也定义 `JointState`，只要放在不同命名空间，就不会直接冲突。

命名空间不是运行时对象，也不会为其中变量自动分配一块共同存储。它处理的是源代码中的名称组织。

### 限定枚举

**`enum class` 定义独立的限定枚举类型，其枚举项需要用类型名限定访问，并且不会隐式转换为整数。**

```cpp
robot::MotionState::moving
```

比用 `0、1、2` 表示状态更清楚，也避免把关节编号等普通整数误当作运动状态。[scoped_enum_error.cpp](../examples/day-07/scoped_enum_error.cpp) 证明 `enum class` 不会隐式转换成 `int`。

如果协议确实需要整数编码，可以使用 `static_cast<int>` 明确转换，但不要在业务逻辑中来回把枚举当整数使用。

### struct

**`struct` 定义一种类类型，用于把相关数据和函数组织为同一个概念；其成员默认公开。**

`JointState` 不是一个具体关节，而是描述关节状态对象应包含哪些数据的类型。执行：

```cpp
robot::JointState joint{2, 45.0, robot::MotionState::moving};
```

才会创建名为 `joint` 的对象，并依次初始化三个数据成员。访问控制、构造函数和类不变量在第 13～14 天深入。

### 成员

**成员是声明在类类型内部、属于该类型的数据或函数。**

`joint.id`、`joint.angle` 和 `joint.motion` 分别访问当前对象的成员。不同 `JointState` 对象各自拥有自己的非静态数据成员，修改一个对象的 `angle` 不会自动修改另一个对象。

## 3. 数据怎样从三个值变成一个状态对象

| 表达式 | 类型 | 含义 |
|---|---|---|
| `2` | `int`，也可作为 `JointId` | 关节编号 |
| `45.0` | `double` | 当前角度 |
| `MotionState::moving` | `MotionState` | 运动状态 |
| `joint` | `JointState` | 同时保存以上三个成员的对象 |

把相关值组合后，函数可以接收 `const JointState&`，避免参数顺序混乱，也让后续增加状态字段更集中。

## 4. 笔试高频点

### 类型和对象不能混淆

写下 `struct JointState { ... };` 只是定义类型，不会自动创建一个 `JointState` 对象。必须再写对象定义。

### 类型别名不是新类型

`using JointId = int;` 不提供额外类型安全。如果岗位代码需要强类型编号，应设计真正的类或结构体，而不是只使用别名。

### `enum class` 需要限定

通常写 `MotionState::fault`，不能把 `fault` 当作当前作用域中的普通整数名称。

### `struct` 和 `class`

两者都能定义类类型，主要语言层面差异是默认成员访问权限和默认继承权限。并不存在“`struct` 只能放数据、`class` 才能放函数”的规则。

## 5. 面试回答

### 问题：为什么状态值更适合使用 `enum class` 而不是裸整数？

> `enum class` 创建独立类型，枚举项通过类型名限定，而且不会隐式转换为整数。它用一组有名称的值表达状态，并能避免把关节编号、错误码等其他整数直接误传为运动状态，同时让调试和接口含义更清楚。

常见追问：

1. 类型别名是否创建新类型？
2. `struct` 和 `class` 的主要区别是什么？

## 6. 今日任务与完成标准

完成 [第 7 天练习](../exercises/day-07.md)，并确认：

- [ ] 能区分类型定义和对象创建；
- [ ] 能使用命名空间限定名称；
- [ ] 能定义并使用 `enum class`；
- [ ] 能用 `struct` 组织一组机器人状态数据；
- [ ] 能解释别名不创造新类型。
