# 第 13 天练习：`const`、`constexpr`、初始化与类型转换

## 练习 1：概念对比

用自己的话区分 `const` 对象、`constexpr` 变量和常量表达式，并各写一个最小例子。

>
>
>
>

## 练习 2：初始化分类

为每行标出初始化形式，并说明 `a`～`f` 的状态或值。不得实际读取 `a`。

```cpp
int a;
int b{};
int c(3.8);
int d = 3.8;
int e{3};
int f = {3};
```

>
>
>
>
>
>

## 练习 3：窄化预测与验证

判断以下代码能否通过 C++17 编译，并说明原因。随后实际编译。

```cpp
constexpr int small{42};
int a{small};
long long large{5'000'000'000LL};
int b{large};
double c{3};
int d{3.0};
```

### 预测

>
>
>
>

### 实际结果

>
>
>
>

## 练习 4：修正转换

编写函数 `to_int_if_representable(double value, int& result)`：仅当 `value` 是有限值、没有小数部分且在 `int` 范围内时转换并返回 `true`，否则返回 `false`。

```cpp

```

## 练习 5：指针 const 层级

对每条语句判断：能否修改指向、能否经指针修改目标，并写出非法操作的编译原因。

```cpp
int value{1};
const int* a{&value};
int* const b{&value};
const int* const c{&value};
```

>
>
>
>
>

## 练习 6：引用绑定预测与验证

预测每条声明是否合法；实际编译后记录诊断。

```cpp
int value{7};
int& first{value};
const int& second{value};
const int& third{40 + 2};
int& fourth{40 + 2};
```

### 预测

>
>
>

### 实际结果

>
>
>

## 练习 7：`constexpr` 函数

写 `constexpr int triangular(int n)`，计算 `1 + ... + n`，并同时用 `static_assert` 与运行期输入调用验证。

```cpp

```

## 练习 8：解释错误实验

分别编译 `narrowing_error.cpp` 与 `const_assignment_error.cpp`，记录关键诊断，并指出错误发生在完整构建链路的哪个阶段。

>
>
>
>
>

## 练习 9：枚举转换

定义 `enum class JointState : unsigned char`，用 `static_cast` 输出枚举值；说明为什么限定枚举不自动转成整数。

```cpp

```

## 练习 10：深度思考

解释为什么 `const_cast` 能通过编译，却不能保证写入合法。分别讨论底层对象原本为 `int` 和原本为 `const int` 的情况。

>
>
>
>
>

## 完成检查

- [ ] 我能区分 `const`、`constexpr` 与常量表达式
- [ ] 我能区分六种初始化和赋值
- [ ] 我实际观察了窄化与 const 赋值诊断
- [ ] 我能追踪指针的顶层与底层 const
- [ ] 我能在转换前检查值域
- [ ] 我完成了预测与实际结果对照
