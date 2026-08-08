# 第 17 天练习：声明、定义、名称查找、链接与 ODR

## 练习 1：声明与定义

判断每行是否为定义，并说明依据。

```cpp
int function(int value);
int function(int value) { return value; }
extern int first;
extern int second{2};
struct State;
struct State { int value; };
```

>
>
>
>
>

## 练习 2：名称查找

预测每个输出表达式找到哪个声明，并说明名称隐藏如何发生。

```cpp
int value{1};
namespace robot { int value{2}; }

int main() {
    int value{3};
    // value, ::value, robot::value
}
```

### 预测

>
>
>

### 实际结果

>
>
>

## 练习 3：四个维度

分别说明普通局部变量、块作用域静态变量、命名空间 `static` 变量和外部全局变量的作用域、链接、存储期与生命周期。

>
>
>
>
>

## 练习 4：`extern` 修复

把头文件中的普通可修改变量定义改成“头文件声明 + 一个 `.cpp` 定义”，并写出三个文件。

```cpp

```

## 练习 5：inline 变量修复

把同一错误改为 C++17 inline 变量方案，说明它与普通命名空间 `const` 的实体身份区别。

```cpp

```

>
>

## 练习 6：链接错误预测与验证

预测重复定义实验与缺失定义实验的失败阶段和关键诊断，再实际构建。

### 预测

>
>
>

### 实际结果

>
>
>

## 练习 7：符号观察

用 `nm -C` 检查 `active_mode`、未命名命名空间变量和 namespace-scope `static` 变量，记录全局/局部符号差异。

>
>
>
>

## 练习 8：`static` 三种含义

分别写出命名空间作用域、块作用域和类作用域的 `static` 声明，并解释每处改变什么。类作用域部分只需标注第 19 天继续。

```cpp

```

## 练习 9：语言链接

解释 `extern int value;` 与 `extern "C" int read();` 分别控制什么，为什么后者不等于“从动态库读取”。

>
>
>
>

## 练习 10：ODR 审查

列出哪些定义通常适合放头文件、哪些应只在一个 `.cpp` 中定义，并说明 inline 多定义仍需满足哪些一致性要求。

>
>
>
>
>

## 完成检查

- [ ] 我能区分声明与定义
- [ ] 我能追踪限定/非限定名称查找
- [ ] 我能区分四个维度
- [ ] 我复现了两种链接错误
- [ ] 我检查了内部/外部链接符号
- [ ] 我能正确选择 extern 或 inline 变量方案
