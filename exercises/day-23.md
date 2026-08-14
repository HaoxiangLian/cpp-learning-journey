# 第 23 天练习：智能指针与所有权

> 本练习使用 C++17。请保留预测、实际结果和错误诊断；不要只写最终答案。

## 1. 所有权判断

说明下列三个名称分别承担“所有者”还是“观察者”角色，并解释判断依据：

```cpp
auto motor = std::make_unique<Motor>();
Motor* view = motor.get();
const Motor& alias = *motor;
```

>
>
>

## 2. 预测移动后的状态

```cpp
auto first = std::make_unique<int>(7);
auto second = std::move(first);
std::cout << (first == nullptr) << ' ' << *second << '\n';
```

**预测**

>
>

**实际结果**

>
>

## 3. 编写独占所有权函数

定义 `std::unique_ptr<std::string> make_name(std::string text)`，用 `std::make_unique` 创建并返回对象；在 `main` 中接收并输出它。

```cpp

```

## 4. 修复悬空观察者

以下代码为什么错误？在不延长对象生命周期的前提下，调整执行顺序，使观察者只在对象仍存活时使用。

```cpp
auto owner = std::make_unique<int>(42);
int* observer = owner.get();
owner.reset();
std::cout << *observer << '\n';
```

>
>

```cpp

```

## 5. `release` 与 `reset`

分别说明 `release()` 和 `reset()` 对“所拥有对象”以及智能指针自身状态的影响。为什么随意调用 `release()` 容易造成泄漏？

>
>
>

## 6. 预测共享计数

```cpp
auto a = std::make_shared<int>(9);
std::cout << a.use_count() << '\n';
{
    auto b = a;
    std::weak_ptr<int> w = b;
    std::cout << a.use_count() << ' ' << w.use_count() << '\n';
}
std::cout << a.use_count() << '\n';
```

**预测**

>
>

**实际结果**

>
>

## 7. 安全使用 `weak_ptr`

编写函数 `print_if_alive(const std::weak_ptr<int>& value)`：调用 `lock()`，若对象仍存活则输出其值，否则输出 `expired`。

```cpp

```

## 8. 识别两个控制块

解释下面代码为什么不是“两个安全的共享所有者”，并改为只建立一个共享所有权组。

```cpp
int* raw = new int{5};
std::shared_ptr<int> a{raw};
std::shared_ptr<int> b{raw};
```

>
>

```cpp

```

## 9. 打破所有权环

将其中一个方向改成非拥有关系，使两个节点都能在离开作用域时析构。

```cpp
struct Node {
    std::shared_ptr<Node> next;
    std::shared_ptr<Node> previous;
};
```

```cpp

```

## 10. 选择工具

为下列情形选择 `unique_ptr`、`shared_ptr`、`weak_ptr`、裸指针或引用，并说明理由：

1. 工厂函数创建对象并把唯一所有权交给调用方。
2. 一个短函数只读取调用方保证存活的对象。
3. 多个异步任务确实需要共同延长同一对象的生命周期。
4. 缓存需要记住共享对象，但不能阻止它被销毁。

>
>
>
>

## 11. 错误实验记录

分别编译或运行 `unique_copy_error.cpp`、`dangling_observer.cpp` 和 `double_control_block.cpp`，记录错误发生在编译期还是运行期，并摘录最关键的一行诊断。

>
>
>

## 12. 小型综合题

设计 `Robot` 类：名字由 `std::string` 保存；主控制器用 `unique_ptr<Robot>` 独占机器人；监视器只在一次函数调用期间观察机器人，不取得所有权。输出构造、观察和析构顺序。

```cpp

```
