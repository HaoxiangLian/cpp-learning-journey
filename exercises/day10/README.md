# 第 10 天练习：拷贝与 Rule of Three

[返回第 10 天讲义](../../days/day10.md) · [完成后查看答案](../../solutions/day10/README.md)

## 一、概念判断

1. `T copy{source};` 先默认构造 `copy`，再调用拷贝赋值。
2. 默认拷贝 `int*` 会自动创建并复制整段动态数组。
3. 独占资源类的深拷贝应让源对象和目标对象拥有不同资源。
4. `target = source` 时，目标可能已经拥有必须正确处理的旧资源。
5. Rule of Three 是标准要求所有类都必须声明三个函数。

## 二、代码分析

### A：调用哪个函数

```cpp
Buffer first{3};
Buffer second{first};
Buffer third{1};
third = first;
```

三行创建/复制分别调用普通构造、拷贝构造还是拷贝赋值？

### B：默认指针拷贝

```cpp
class Buffer {
public:
    Buffer() : data_{new int[2]{1, 2}} {}
    ~Buffer() { delete[] data_; }
private:
    int* data_;
};

Buffer a;
Buffer b{a};
```

`a`、`b` 的 `data_` 值有什么关系？离开作用域时风险是什么？不得只回答“浅拷贝不好”。

### C：自赋值

```cpp
Buffer& Buffer::operator=(const Buffer& other) {
    delete[] data_;
    data_ = new int[other.size_];
    // 再从 other.data_ 复制
    return *this;
}
```

执行 `buffer = buffer;` 时，哪一步让源数据失效？还存在什么失败路径问题？

## 三、编程题：独占样本数组

打开 [`owned_samples.cpp`](owned_samples.cpp)。起始代码用 `= delete` 禁止拷贝，因此安全但不可复制。请：

- 把已删除的两个拷贝操作替换为深拷贝构造和拷贝赋值；
- 保留析构函数，构成 Rule of Three；
- 赋值时处理不同长度、旧资源和自赋值；
- 先分配替代数组，再释放旧数组；
- 扩展 `main`，证明修改副本不影响原对象；
- 不使用 `std::vector`，因为本题目的是理解手动资源类；工程代码之后应优先容器。

## 四、故意错误观察

- [`broken/copy_disabled.cpp`](broken/copy_disabled.cpp)：预期编译失败，展示被删除拷贝操作。
- [`broken/shallow_copy.cpp`](broken/shallow_copy.cpp)：可能编译成功但运行会产生双重释放风险，禁止运行且不进入正常构建。
