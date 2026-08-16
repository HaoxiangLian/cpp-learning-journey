# 第 6 天练习：参数传递、引用与函数重载

## A. 笔试基础

### 1. 预测最终 `number` 的值

```cpp
void change_value(int value) { value = 10; }
void change_ref(int& value) { value = 20; }

int number{3};
change_value(number);
change_ref(number);
```

> 
>

### 2. 为下列需求选择 `T`、`T&` 或 `const T&`

- 读取一个 `double`：
- 修改调用方的 `double`：
- 只读访问大型配置对象且不复制：

> 
>
>

### 3. 为什么只改变返回类型不能形成重载

> 
>

## B. 编程题

编写 `void calibrate(double& reading, double offset)` 修改读数；再重载 `clamp`，分别支持 `int` 和 `double`。

```cpp

```

实际输出：

```text

```

## C. 调试题

构建 `examples/day-06/non_const_reference_error.cpp`，记录诊断，说明为什么字面值不能绑定到 `double&`。再改为先创建具名对象后调用。

```text

```

> 
>

## D. 面试口述

用 30～60 秒回答：值传递、引用传递和 `const` 引用怎样选择？

> 
>
>
>

