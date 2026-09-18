你先把 `explicit` 理解成一句很简单的话：

> **`explicit` 是加在构造函数前面的关键字，用来禁止编译器“偷偷帮你做类型转换”。**

它最常用于**单参数构造函数**。

---

## 1. 先看一个没有 `explicit` 的例子

```cpp
class Age {
public:
    Age(int x) {
        // ...
    }
};
```

这里：

```cpp
Age(int x)
```

是一个构造函数，它表示：

> 可以用一个 `int` 来创建一个 `Age` 对象。

正常写法很好理解：

```cpp
Age a(18);
```

或者：

```cpp
Age a{18};
```

意思都是：

> 用整数 `18` 构造一个 `Age` 对象。

但是 C++ 还允许一种你可能觉得比较奇怪的写法：

```cpp
Age a = 18;
```

为什么一个 `Age` 类型的变量，右边居然可以直接写一个 `int`？

因为编译器发现：

```cpp
Age(int x)
```

可以接收一个 `int`。

于是编译器会自动把：

```cpp
18
```

转换成：

```cpp
Age(18)
```

你可以粗略理解为：

```cpp
Age a = 18;
```

编译器偷偷帮你变成了：

```cpp
Age a = Age(18);
```

这种行为就叫：

> **隐式类型转换（implicit conversion）**

---

# 2. `explicit` 就是禁止这种“偷偷转换”

现在我们把构造函数写成：

```cpp
class Age {
public:
    explicit Age(int x) {
        // ...
    }
};
```

注意前面多了：

```cpp
explicit
```

这时候：

```cpp
Age a(18);    // 可以
Age b{18};    // 可以
```

但是：

```cpp
Age c = 18;   // 不可以
```

因为你加了：

```cpp
explicit
```

相当于告诉编译器：

> **不要擅自把 `int` 转换成 `Age`。如果程序员真的想创建 `Age`，必须明确写出来。**

所以：

```cpp
Age c = 18;
```

不行。

但：

```cpp
Age c{18};
```

可以。

因为这里程序员已经明确表达：

> “我要用 18 构造一个 Age 对象。”

---

# 3. `explicit` 的定义是什么？

可以这样记：

> **`explicit` 是 C++ 的关键字，通常用于构造函数，作用是禁止该构造函数被用于隐式类型转换。**

比如：

```cpp
class Money {
public:
    explicit Money(int yuan) {
    }
};
```

这里：

```cpp
explicit Money(int yuan)
```

意味着：

> `int` 可以用来显式构造 `Money`，但不能让编译器自动把 `int` 当成 `Money`。

---

# 4. 为什么需要 `explicit`？

这才是最重要的地方。

假设有一个类：

```cpp
class Distance {
public:
    Distance(double meter) {
    }
};
```

然后有一个函数：

```cpp
void move(Distance d) {
}
```

正常来说，我们希望调用者传入：

```cpp
Distance d{10.0};

move(d);
```

或者：

```cpp
move(Distance{10.0});
```

这都非常清晰。

但是因为：

```cpp
Distance(double meter)
```

没有 `explicit`，所以居然还可以写：

```cpp
move(10.0);
```

编译器会偷偷做：

```text
10.0
 ↓
调用 Distance(10.0)
 ↓
得到 Distance 对象
 ↓
传给 move()
```

也就是说：

```cpp
move(10.0);
```

大致相当于：

```cpp
move(Distance{10.0});
```

---

## 5. 这有什么危险？

因为：

```cpp
move(10.0);
```

这个 `10.0` 到底是什么意思？

是：

```text
10 米？
10 厘米？
10 毫米？
10 英寸？
```

单看代码不够明确。

如果写：

```cpp
move(Distance{10.0});
```

就清楚很多：

> 我要把 `10.0` 转换成一个 `Distance` 对象。

所以我们可以：

```cpp
class Distance {
public:
    explicit Distance(double meter) {
    }
};
```

这时候：

```cpp
move(10.0);              // 错误
```

必须明确写：

```cpp
move(Distance{10.0});    // 正确
```

这就是 `explicit` 最核心的价值：

> **强迫程序员把类型转换意图写清楚，避免编译器自动转换导致语义错误。**

---

# 6. 一个更生活化的例子

假设：

```cpp
class Person {
public:
    Person(int age) {
    }
};
```

因为没有 `explicit`：

```cpp
Person p = 18;
```

居然成立。

编译器理解成：

```cpp
Person p = Person(18);
```

但从人的角度看：

```cpp
Person p = 18;
```

很奇怪：

> 一个“人”怎么直接等于整数 18？

如果写：

```cpp
class Person {
public:
    explicit Person(int age) {
    }
};
```

那么：

```cpp
Person p = 18;   // 错误
```

必须：

```cpp
Person p{18};
```

或者：

```cpp
Person p(18);
```

这就明确表示：

> “我要用年龄 18 来构造一个 Person 对象。”

---

# 7. `explicit` 不是禁止这个构造函数使用

这一点非常容易误解。

看到：

```cpp
explicit Age(int x);
```

千万不要理解成：

> 这个构造函数不能用了。

不是。

下面照样可以：

```cpp
Age a(18);
Age b{18};
```

`explicit` 禁止的是：

```cpp
Age a = 18;
```

以及类似的自动转换：

```cpp
void func(Age age);

func(18);   // 不允许
```

所以区别是：

```text
explicit 不是：
“禁止构造”

而是：
“禁止偷偷构造”
```

这个说法非常适合初学时记忆。

---

# 8. 为什么常说“单参数构造函数要注意 explicit”？

例如：

```cpp
class A {
public:
    A(int x);
};
```

这里只需要一个参数：

```cpp
int
```

就能制造一个 `A`。

所以编译器很容易认为：

```text
int
 ↓
A
```

是一种允许的转换。

比如：

```cpp
A a = 10;
```

因此单参数构造函数尤其容易产生隐式类型转换。

于是工程代码中经常写：

```cpp
class A {
public:
    explicit A(int x);
};
```

---

# 9. 对比一下最清楚

### 没有 `explicit`

```cpp
class A {
public:
    A(int x) {
    }
};
```

下面都可以：

```cpp
A a(10);     // 可以
A b{10};     // 可以
A c = 10;    // 可以，发生隐式转换
```

甚至：

```cpp
void func(A a) {
}

func(10);    // 可以
```

编译器自动帮你：

```cpp
10
↓
A(10)
↓
传给 func
```

---

### 有 `explicit`

```cpp
class A {
public:
    explicit A(int x) {
    }
};
```

这时候：

```cpp
A a(10);        // 可以
A b{10};        // 可以

A c = 10;       // 错误
```

函数调用：

```cpp
void func(A a) {
}

func(10);       // 错误
```

必须写：

```cpp
func(A{10});    // 正确
```

---

# 10. 这里顺便认识两个初始化方式

你以后会经常看到：

```cpp
A a(10);
```

和：

```cpp
A a = 10;
```

它们看起来很像，但有区别。

### 直接初始化

```cpp
A a(10);
```

或者：

```cpp
A a{10};
```

叫：

> **直接初始化**

程序员明确说：

> 调用 `A` 的构造函数创建对象。

`explicit` 构造函数可以用于这种情况。

---

### 复制初始化

```cpp
A a = 10;
```

叫：

> **复制初始化**

这里编译器需要考虑：

> 能不能把 `10` 自动转换成 `A`？

如果构造函数是：

```cpp
A(int);
```

可以。

如果是：

```cpp
explicit A(int);
```

就不允许。

---

# 11. 你可以把 `explicit` 想象成一道门

没有 `explicit`：

```text
int 10
  │
  │ 编译器：我帮你转换吧
  ↓
A对象
```

比如：

```cpp
A a = 10;
```

---

加上 `explicit`：

```text
int 10
  │
  │ ❌ 不准自动转换
  ↓

程序员必须自己明确写：

A{10}
```

所以：

```cpp
explicit A(int x);
```

可以理解为：

> **“必须显式地告诉我你要构造 A。”**

这也正是 `explicit` 这个英文单词本身的意思：

```text
explicit
=
明确的
显式的
```

---

# 12. 一个你现在很适合记住的例子

```cpp
class RetryLimit {
public:
    explicit RetryLimit(int count)
        : count_(count) {
    }

private:
    int count_;
};
```

假设有：

```cpp
void schedule(RetryLimit limit) {
}
```

没有 `explicit` 的时候：

```cpp
schedule(3);
```

也可以。

编译器偷偷做：

```cpp
schedule(RetryLimit{3});
```

但 `3` 是什么？

```text
3 秒？
3 次？
3 个任务？
```

不够明确。

有了：

```cpp
explicit RetryLimit(int count)
```

以后：

```cpp
schedule(3);                // 错误
```

必须写：

```cpp
schedule(RetryLimit{3});    // 正确
```

这时候你一看就知道：

> `3` 表示的是“重试次数”。

---

# 13. 最终你只需要牢牢记住这句话

看到：

```cpp
explicit A(int x);
```

脑子里把它翻译成：

> **“可以用 `int` 构造 `A`，但是必须明确地写出构造 `A`，不允许编译器偷偷把 `int` 转成 `A`。”**

所以：

```cpp
A a{10};     // ✅ 明确构造
```

而：

```cpp
A a = 10;    // ❌ explicit 不允许这种隐式转换
```

再浓缩成一句：

> **`explicit` 的核心作用：防止构造函数参与意料之外的隐式类型转换，让代码的类型转换意图更加明确。**