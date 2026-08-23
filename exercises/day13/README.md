# 第 13 天练习：虚函数、抽象类与对象表示

[返回第 13 天讲义](../../days/day13.md) · [完成后查看答案](../../solutions/day13/README.md)

每道调用题先写四项：完整对象是什么、表达式写成什么类型、函数是否为虚函数、派生声明是否真正覆盖。不要先画 vtable。

## 一、概念判断题

判断正误并用一句话说明理由。

1. 只要函数参数写成 `const Base&`，所有同名成员函数都会根据派生类型动态选择。
2. 基类把 `read()` 声明为虚函数后，派生类真正覆盖的 `read()` 即使不重复写 `virtual`，仍然是虚函数。
3. 基类声明 `virtual int read() const`，派生类声明 `int read() override` 可以正确覆盖。
4. 抽象类不能直接创建对象，但可以声明指向具体派生对象的抽象类指针或引用。
5. 每个含虚函数的对象都由 C++ 标准保证在起始位置保存一个指针大小的 vptr。

## 二、代码输出与错误分析

1. 阅读下面代码，分别写出两行输出，并说明是否发生切片：

   ```cpp
   class Base {
   public:
       const char* name() const { return "Base"; }
       virtual int value() const { return 1; }
   };

   class Derived : public Base {
   public:
       const char* name() const { return "Derived"; }
       int value() const override { return 2; }
   };

   Derived object;
   const Base& view{object};
   std::cout << view.name() << '\n';
   std::cout << view.value() << '\n';
   ```

2. 查看 [`broken/mismatched_override.cpp`](broken/mismatched_override.cpp)，指出哪一处不匹配、错误发生在哪个阶段，以及为什么不应通过删除 `override` 修复。
3. 查看 [`broken/instantiate_abstract.cpp`](broken/instantiate_abstract.cpp)，说明为什么 `Sensor*` 可以声明而 `Sensor sensor;` 不能。
4. 对 `struct Record { char valid; int value; };`，说明 `sizeof(Record)` 和 `alignof(Record)` 分别回答什么。哪些数字必须运行当前编译器后才能知道？

## 三、小型编程题：统一传感器报告接口

打开 [`polymorphic_sensors.cpp`](polymorphic_sensors.cpp)，完成：

- 把 `Sensor` 设计成抽象基类，声明纯虚 `name()` 与 `read()`，并保留适合多态删除的析构接口。
- 定义 `TemperatureSensor` 和 `ForceSensor`，使用 `override` 提供不同名称与读数。
- 编写 `print_sensor(const Sensor&)`，不得用类型判断分支，输出传感器名称和读数。
- 分别创建两个具体对象，经同一个报告函数输出。
- 定义含 `char` 与 `int` 的 `LayoutProbe`，只输出当前实现的 `sizeof` 和 `alignof`，不要写死预期数字。

预期稳定输出包含：

```text
temperature: 23.5
force: 12.5
```

大小与对齐数字因实现而异，不属于固定答案。
