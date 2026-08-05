# 第 14 天练习：表达式求值、顺序、值类别与未定义行为

## 练习 1：值计算与副作用

逐行写出值计算结果、发生的副作用和完整表达式结束后的对象状态。

```cpp
int value{2};
int a{++value};
int b{value++};
int c{value + 3};
```

>
>
>
>
>

## 练习 2：短路预测与验证

预测函数分别被调用几次，再实际运行验证。

```cpp
int calls{};
auto probe = [&calls]() {
    ++calls;
    return true;
};

bool first{false && probe()};
bool second{true || probe()};
bool third{true && probe()};
```

### 预测

>
>
>

### 实际结果

>
>
>

## 练习 3：顺序分类

分别判断下列求值关系属于“先序于”“不确定顺序”还是“未定序”，并说明能否依赖输出顺序。

```cpp
left() && right();
function(first(), second());
first() + second();
(first(), second());
```

>
>
>
>
>

## 练习 4：修复未定序修改

把下面代码改写为没有未定义行为、且结果意图明确的代码，并解释新顺序。

```cpp
int i{1};
int result{i++ + i++};
```

```cpp

```

## 练习 5：值类别

判断每个表达式是 lvalue、xvalue 还是 prvalue，并说明依据。

```cpp
int value{3};
value;
value + 1;
42;
static_cast<int&&>(value);
++value;
value++;
```

>
>
>
>
>
>

## 练习 6：行为分类

将下列情况分类为规定行为、未指定行为、实现定义行为或未定义行为。

- 普通 `char` 的符号性
- 两个函数实参谁先求值
- 有符号整数溢出
- 无符号整数回绕
- 解引用数组尾后指针
- `false && function()` 是否调用函数

>
>
>
>
>
>

## 练习 7：边界预测与验证

分别预测两个错误实验的表现，再用 ASan/UBSan 运行。

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

## 练习 8：尾后指针

编写不使用下标的数组求和程序，只用 `begin`、尾后 `end` 和指针递增；说明为什么循环条件必须在解引用前检查。

```cpp

```

## 练习 9：范围 `for` 展开

把下面循环改写为 C++17 的说明性展开形式，并指出 `make_values()` 被求值几次。

```cpp
for (const int value : make_values()) {
    consume(value);
}
```

```cpp

```

## 练习 10：常量求值

解释为什么常量表达式中的有符号溢出必须被诊断，而同样的运行期表达式可能编译成功却仍是未定义行为。

>
>
>
>

## 完成检查

- [ ] 我能区分值计算与副作用
- [ ] 我能判断三类顺序关系
- [ ] 我能画出五种值类别之间的集合关系
- [ ] 我能区分四种行为分类
- [ ] 我实际运行了 ASan 与 UBSan 实验
- [ ] 我能解释尾后指针与范围 `for` 展开
