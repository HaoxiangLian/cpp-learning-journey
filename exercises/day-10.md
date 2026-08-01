# 第 10 天练习单

主题：地址、指针与引用  
语言标准：C++17

## 作答方式

- 直接在每道题下方的空白区填写答案。
- 预测题先写预测，再编译运行并记录实际结果。
- 代码题在紧随题目的空 `cpp` 代码块中完成。
- 错误实验保留关键诊断，并判断错误发生阶段。

---

## 练习 1：区分五个概念

阅读：

```cpp
int joint_id{10};
int* pointer{&joint_id};
int& reference{joint_id};
```

### 1.1 `joint_id` 是什么？

> 

### 1.2 `&joint_id` 的类型和值分别表示什么？

> 

### 1.3 `pointer` 是指针类型、指针对象还是被指向对象？

> 

### 1.4 `pointer` 当前保存的指针值指向谁？

> 

### 1.5 `reference` 与 `joint_id` 是什么关系？

> 

---

## 练习 2：判断 `&` 和 `*` 的语法角色

分别说明每个符号的含义：

```cpp
int value{6};
int* pointer{&value};
int& reference{value};
int product{value * 2};
int bits{value & 3};
int copied{*pointer};
```

| 位置 | 语法角色 | 产生的结果或声明含义 |
|---|---|---|
| `int* pointer` 中的 `*` |  |  |
| `&value` 中的 `&` |  |  |
| `int& reference` 中的 `&` |  |  |
| `value * 2` 中的 `*` |  |  |
| `value & 3` 中的 `&` |  |  |
| `*pointer` 中的 `*` |  |  |

---

## 练习 3：预测通过指针和引用修改对象

```cpp
#include <iostream>

int main() {
    int value{10};
    int* pointer{&value};
    int& reference{value};

    *pointer = 20;
    reference = 30;

    std::cout << value << '\n';
    std::cout << *pointer << '\n';
    std::cout << reference << '\n';
    return 0;
}
```

### 3.1 预测

> 

### 3.2 实际结果

> 

### 3.3 为什么三行输出相同？

> 

---

## 练习 4：指针重新指向

补全程序，使 `pointer` 先指向 `first`，再改为指向 `second`，最后通过指针把 `second` 改为 50；`first` 必须保持 10。

```cpp

```

### 预测

> 

### 实际结果

> 

### 被修改的是指针对象还是被指向对象？分别发生在哪一步？

> 

---

## 练习 5：引用赋值不是重新绑定

```cpp
#include <iostream>

int main() {
    int first{10};
    int second{20};
    int& reference{first};

    reference = second;
    second = 30;

    std::cout << first << '\n';
    std::cout << second << '\n';
    std::cout << reference << '\n';
    return 0;
}
```

### 5.1 预测

> 

### 5.2 实际结果

> 

### 5.3 `reference = second` 为什么没有让引用改绑到 `second`？

> 

---

## 练习 6：空指针检查

实现：

```cpp
bool set_value(int* target, int new_value);
```

要求：

- `target == nullptr` 时返回 `false`
- 否则修改被指向对象并返回 `true`
- 不得解引用空指针

```cpp

```

### 测试有效对象与 `nullptr`

```cpp

```

### 实际结果

> 

---

## 练习 7：选择指针参数还是引用参数

对以下接口分别选择 `T*`、`T&`、`const T&` 或按值传递，并说明理由。

### 7.1 函数必须修改调用方的关节编号，而且调用时必须提供对象

> 

### 7.2 函数可以选择性输出一个关节；没有关节也是合法情况

> 

### 7.3 函数只读取一个较大的 `JointState` 对象，且必须提供对象

> 

### 7.4 函数只接收一个小型 `bool` 状态

> 

### 这些选择是否是覆盖所有工程场景的绝对规则？

> 

---

## 练习 8：数组不是指针

```cpp
int values[4]{2, 4, 6, 8};
int* pointer{values};
```

### 8.1 `values` 的类型是什么？

> 

### 8.2 `pointer` 的类型是什么？

> 

### 8.3 初始化 `pointer` 时发生了什么转换？

> 

### 8.4 `sizeof(values)` 与 `sizeof(pointer)` 分别针对什么对象？

> 

### 8.5 为什么不能说“数组就是指针”？

> 

---

## 练习 9：使用首元素与尾后指针遍历

对数组：

```cpp
int values[4]{2, 4, 6, 8};
```

创建 `begin` 和 `end` 指针，用指针循环输出全部元素。禁止下标访问，禁止解引用 `end`。

```cpp

```

### `end` 是否指向最后一个元素？

> 

### 为什么 `end` 可以比较却不能解引用？

> 

### 实际结果

> 

---

## 练习 10：数组形参调整

阅读：

```cpp
void inspect(int values[10]) {
}
```

### 10.1 函数参数类型形成后，`values` 的实际参数类型是什么？

> 

### 10.2 写在方括号中的 `10` 是否让函数得到数组长度？

> 

### 10.3 若函数要安全遍历，第一层做法是什么？

> 

### 10.4 改写函数声明，显式传入数量

```cpp

```

---

## 练习 11：实现数组更新与输出

编写：

```cpp
bool set_at(int values[], int count, int index, int new_value);
void print_values(const int values[], int count);
```

要求：

- `index` 不在 `[0, count)` 时返回 `false`
- 有效时修改真实数组元素并返回 `true`
- 输出函数不得修改元素
- 测试首元素、末元素、`-1` 和 `count`

```cpp

```

### 预测

> 

### 实际结果

> 

---

## 练习 12：错误实验——指针类型不匹配

编译：

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic \
    examples/day-10/pointer_type_error.cpp \
    -o pointer_type_error
```

### 12.1 是否生成可执行文件？

> 

### 12.2 关键诊断

> 

### 12.3 `&joint_id` 的类型是什么？

> 

### 12.4 错误发生在哪个构建阶段？

> 

---

## 练习 13：错误实验——空指针解引用

使用 ASan/UBSan 构建并运行：

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic \
    -fsanitize=address,undefined -fno-omit-frame-pointer \
    examples/day-10/null_dereference.cpp \
    -o null_dereference_san

./null_dereference_san
```

### 13.1 程序退出状态是否为 0？

> 

### 13.2 关键诊断

> 

### 13.3 为什么没有 Sanitizer 时“偶尔没崩溃”也不能证明合法？

> 

### 13.4 写出解引用前的最小检查

```cpp

```

---

## 练习 14：悬空指针分析

```cpp
int* pointer{nullptr};

{
    int local_value{42};
    pointer = &local_value;
}
```

### 14.1 离开代码块后，`pointer` 是否自动变为 `nullptr`？

> 

### 14.2 为什么不能再通过它读取 `local_value`？

> 

### 14.3 它与空指针有什么区别？

> 

### 14.4 本题还依赖哪个后续单元的完整知识？

> 

---

## 练习 15：完整编程——关节编号与采样数组

编写一个完整程序：

1. 创建三个 `int` 对象表示关节编号
2. 使用一个指针依次指向并修改其中两个对象
3. 为第一个对象创建引用，并证明引用赋值不会重新绑定
4. 编写指针参数函数，允许传入 `nullptr`
5. 编写引用参数函数，要求必须提供对象
6. 创建长度为 5 的采样数组
7. 使用首元素和尾后指针遍历数组
8. 编写数组形参函数，并单独传入元素数量
9. 使用严格警告和 ASan/UBSan 验证

```cpp

```

### 预测输出

> 

### 实际输出

> 

### 编译警告

> 

### Sanitizer 结果

> 

---

## 练习 16：解释构建与诊断阶段

### 16.1 按顺序写出主示例从源码到进入 `main` 的完整链路

> 

### 16.2 指针类型不匹配通常在哪一阶段诊断？

> 

### 16.3 空指针解引用为什么不是链接错误？

> 

### 16.4 Sanitizer 是编译器警告、链接器规则还是运行期检测机制？

> 

---

## 学习复盘

### 1. 对象、地址、指针对象和被指向对象之间是什么关系？

> 

### 2. 指针赋值与解引用后赋值有什么区别？

> 

### 3. 引用为什么不能被普通赋值操作重新绑定？

> 

### 4. 什么时候指针参数比引用参数更能表达接口含义？

> 

### 5. 数组到指针转换与数组形参调整有什么区别？

> 

### 6. 尾后指针的用途和限制分别是什么？

> 

### 7. 空指针与悬空指针有什么区别？

> 

### 8. 本单元仍然暂缓到后续的内容有哪些？

> 

### 9. 本单元最容易混淆的内容

> 
