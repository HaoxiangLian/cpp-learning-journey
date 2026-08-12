# 第 21 天练习：拷贝语义与 Rule of Three

## 练习 1：复制构造还是复制赋值

逐行判断调用复制构造函数、复制赋值运算符，还是不发生 `IntBuffer` 复制，并说明对象在该行之前是否已经存在。

```cpp
IntBuffer a{3};
IntBuffer b{a};
IntBuffer c = a;
IntBuffer d{1};
d = a;
IntBuffer& alias{a};
```

>
>
>
>
>

## 练习 2：预测深复制结果

阅读 `examples/day-21/main.cpp`，先预测四行输出，再运行主示例并记录实际结果。解释为什么修改副本不会修改 `original`。

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
>

## 练习 3：实现复制构造函数

为一个拥有动态整数数组的类写出深复制构造函数。先分配副本自己的数组，再复制元素；考虑大小为 0 的情况。

```cpp

```

## 练习 4：复制赋值的正确顺序

解释为什么“先 `delete[] data_`，再分配并复制”在分配失败时会破坏原对象状态。写出 copy-and-swap 形式的复制赋值运算符。

>
>
>
>

```cpp

```

## 练习 5：自赋值

跟踪 `assigned = assigned;` 中 `this` 与 `&other` 的值。说明当前实现如何安全处理自赋值，以及 copy-and-swap 是否必须依赖显式自赋值检查。

>
>
>
>

## 练习 6：预测浅复制错误

阅读 `shallow_copy_error.cpp`，画出 `first.data_` 和 `second.data_` 指向的动态数组关系，预测两个析构函数执行时会发生什么，再运行 ASan 实验。

### 预测

>
>
>

### 实际结果

>
>
>
>

## 练习 7：按值传参与引用传参

解释 `void print_first(IntBuffer buffer)` 为什么会构造形参对象。把接口改为不复制且不允许修改实参的形式，并说明形参与实参、引用绑定和对象生命周期之间的关系。

```cpp

```

>
>
>

## 练习 8：禁止复制

解释 `deleted_copy_error.cpp` 的错误发生在哪个构建阶段。设计一个只能表示唯一所有权、明确禁止复制的类接口。

>
>
>

```cpp

```

## 练习 9：Rule of Three 与 Rule of Zero

分别说明什么情况下应考虑 Rule of Three，以及为什么使用 `std::vector<int>` 成员通常能让外层类遵循 Rule of Zero。

>
>
>
>

## 练习 10：`std::string` 与 `std::vector` 的复制

写一个包含 `std::string name` 和 `std::vector<double> values` 的类，不手写析构、复制构造或复制赋值。构造一个副本并修改副本，验证两个外层对象拥有各自的值语义状态。

```cpp

```

### 预测

>
>

### 实际结果

>
>
>

## 完成核对

- [ ] 已区分复制构造和复制赋值
- [ ] 已画出浅复制与深复制的资源状态
- [ ] 已解释参数对象如何由实参复制构造
- [ ] 已实现自赋值安全、具强异常保证的复制赋值
- [ ] 已复现浅复制导致的重复释放
- [ ] 已解释 Rule of Three、禁止复制与 Rule of Zero 的适用边界
