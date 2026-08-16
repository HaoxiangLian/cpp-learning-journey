# 第 8 天：string、array 与 vector

预计用时：60～90 分钟  
今日目标：分别保存文本、固定数量数据和可变数量数据，并安全遍历一组传感样本。

## 1. 先运行关节样本程序

[joint_samples.cpp](../examples/day-08/joint_samples.cpp) 使用三种标准库类型：

```cpp
const std::string robot_name{"UR5e"};
const std::array<double, 3> home_angles{0.0, -90.0, 90.0};
std::vector<double> samples{1.0, 2.0, 3.0};
samples.push_back(4.0);
```

输出：

```text
robot: UR5e
home joint 2: -90
sample count: 4
average: 2.5
```

## 2. 关键术语索引

| 术语 | 本课用它解决什么问题 |
|---|---|
| [string](#string) | 怎样保存和修改文本 |
| [array](#array) | 怎样保存固定数量的同类型元素 |
| [vector](#vector) | 怎样保存数量可变化的同类型元素 |
| [元素访问](#元素访问) | 怎样根据位置取得容器中的值 |
| [范围for](#范围for) | 怎样依次处理容器的所有元素 |

### string

**`std::string` 是标准库中用于拥有和操作字符序列的类类型。**

它负责管理文本所需存储，可以比较、拼接并查询长度。`std::string robot_name{"UR5e"};` 创建一个拥有这段文本的对象，比直接管理字符数组更适合作为普通业务字符串。

### array

**`std::array<T, N>` 是包含恰好 `N` 个 `T` 类型元素的固定长度容器。**

`std::array<double, 3>` 的长度是类型的一部分，创建后不能增加第四个元素。机器人自由度、固定标定参数等数量在编译时已知的数据适合使用它。

### vector

**`std::vector<T>` 是拥有元素、且元素数量可以在运行时变化的顺序容器；对普通元素类型，元素连续存储。**

`samples.push_back(4.0)` 在末尾增加元素，使 `size()` 从 3 变为 4。`vector` 会自动管理其动态存储；本阶段不要用裸 `new[]` 模拟它。

增加元素有时会重新分配存储，使原有指针、引用或迭代器失效。完整失效规则放在第 20 天，不在本课展开。

### 元素访问

**元素访问是通过位置或其他接口取得容器中某个元素。**

索引从 0 开始，因此 `home_angles[1]` 是第二个元素。`operator[]` 不执行范围检查，越界访问会导致未定义行为；`.at(index)` 会检查范围，越界时抛出 `std::out_of_range`。

[at_out_of_range.cpp](../examples/day-08/at_out_of_range.cpp) 的 `angles` 只有三个元素，合法索引是 `0、1、2`，访问 `at(3)` 会在运行期失败。

### 范围for

**范围 `for` 按顺序为容器中的每个元素执行一次循环体。**

```cpp
for (const double sample : samples) {
    sum += sample;
}
```

本例把每个 `double` 元素复制到只读局部变量 `sample`，复制成本很低。遍历大型类对象时常写 `const T&` 避免复制；需要修改元素时可写 `T&`。

## 3. 三种类型怎样选择

| 需求 | 类型 | 原因 |
|---|---|---|
| 保存机器人名称 | `std::string` | 专门管理文本 |
| 保存固定 6 个关节零位 | `std::array<double, 6>` | 数量固定且属于类型 |
| 保存持续到来的采样 | `std::vector<double>` | 运行时可以增加元素 |

选择容器先看数据数量是否固定、怎样访问和怎样修改，不要只背 API 名称。

## 4. 平均值计算过程

```cpp
double sum{};
for (const double sample : samples) {
    sum += sample;
}
const double average{
    sum / static_cast<double>(samples.size())};
```

`sum` 初始为 0，四次循环后依次变成 `1、3、6、10`。`size()` 表示元素数量 4，显式转换后执行浮点除法，得到平均值 `2.5`。

真实代码还必须处理空 `vector`，否则除数为 0。本例已知先放入四个样本；练习会要求加入空容器检查。

## 5. 笔试高频点

### 索引范围

长度为 `N` 的顺序容器，合法索引通常是 `0` 到 `N - 1`。`index == size()` 指向尾后位置，不能解引用或读取元素。

### `size()` 不是最后一个索引

三个元素的 `size()` 为 3，最后一个元素索引为 2。循环通常使用 `i < container.size()`，而不是 `<=`。

### `array` 与 `vector`

`array` 长度固定，`vector` 长度可变；二者都按值拥有元素。把 `vector` 传给函数时若只读且不希望复制整个容器，通常使用 `const std::vector<T>&`。

## 6. 面试回答

### 问题：`std::array` 和 `std::vector` 怎样选择？

> 元素数量在编译时固定并且属于数据模型时使用 `std::array`，例如固定自由度的关节零位；元素数量需要在运行时增长或缩小时使用 `std::vector`，例如采样序列。两者都按值拥有元素并支持连续访问，但 `vector` 的扩容可能重新分配存储并使已有引用或迭代器失效。

常见追问：

1. `operator[]` 与 `.at()` 的边界检查有什么区别？
2. 为什么 `index == size()` 不能读取元素？

## 7. 今日任务与完成标准

完成 [第 8 天练习](../exercises/day-08.md)，并确认：

- [ ] 能根据数据数量选择 `string`、`array` 或 `vector`；
- [ ] 能解释索引从 0 开始及合法范围；
- [ ] 能使用范围 `for` 计算总和和平均值；
- [ ] 能解释 `[]` 与 `.at()` 的差别；
- [ ] 能处理空容器，避免除以 0。
