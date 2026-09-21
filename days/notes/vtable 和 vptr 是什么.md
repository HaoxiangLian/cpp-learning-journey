可以把 **vtable** 和 **vptr** 先理解成 C++ 为“虚函数动态绑定”准备的一套查表机制。

先看例子：

```cpp
class Sensor {
public:
    virtual double read() const {
        return 0;
    }
};

class Lidar : public Sensor {
public:
    double read() const override {
        return 64;
    }
};
```

然后：

```cpp
Lidar lidar;
Sensor* p = &lidar;

p->read();
```

虽然 `p` 的类型是：

```cpp
Sensor*
```

但最后却调用：

```cpp
Lidar::read()
```

这通常就是靠 **vptr + vtable** 实现的。

### 1. vtable 是什么？

`vtable` 全称 **virtual table，虚函数表**。

你可以把它理解成：

> **一张“这个类的虚函数应该去哪里执行”的表。**

比如概念上，`Sensor` 可能有一张表：

```text
Sensor 的 vtable

read()  → Sensor::read
```

而 `Lidar` 覆盖了 `read()`：

```text
Lidar 的 vtable

read()  → Lidar::read
```

所以不同类的表里，记录的函数可能不同。

---

### 2. vptr 是什么？

`vptr` 可以理解成：

> **对象内部一个隐藏的指针，用来指向这个对象应该使用的 vtable。**

比如：

```cpp
Sensor sensor;
```

概念上可能是：

```text
Sensor 对象

┌──────────────┐
│ vptr ─────────────→ Sensor vtable
└──────────────┘
```

而：

```cpp
Lidar lidar;
```

可能是：

```text
Lidar 对象

┌──────────────┐
│ vptr ─────────────→ Lidar vtable
└──────────────┘
```

关键就在这里。

虽然：

```cpp
Sensor* p = &lidar;
```

`p` 是 `Sensor*`，

但 `p` 指向的实际对象是：

```text
Lidar 对象
```

这个对象内部的 `vptr` 指向：

```text
Lidar 的 vtable
```

所以：

```cpp
p->read();
```

可以粗略理解成：

```text
p 指向 Lidar 对象
       ↓
找到对象里的 vptr
       ↓
找到 Lidar 的 vtable
       ↓
查 read() 对应的是谁
       ↓
Lidar::read()
       ↓
返回 64
```

所以整个过程可以画成：

```text
Sensor* p
    │
    ↓
┌───────────────┐
│ Lidar 对象    │
│               │
│ vptr ───────────────┐
└───────────────┘      │
                       ↓
                ┌─────────────────┐
                │ Lidar vtable    │
                │                 │
                │ read → Lidar::read
                └─────────────────┘
```

### 3. 为什么需要它？

因为程序运行前，编译器看到：

```cpp
Sensor* p;
```

它并不知道 `p` 将来到底会指向：

```cpp
Sensor
Lidar
Camera
Radar
```

中的哪一个。

例如：

```cpp
Sensor* p;

if (...) {
    p = &lidar;
} else {
    p = &camera;
}

p->read();
```

这时候真正执行哪个 `read()`，要等**运行时**才能知道。

于是常见做法就是：

> 对象通过 `vptr` 告诉程序：“我的虚函数表在这里。”

然后程序查 `vtable`，找到最终应该调用的函数。

---

### 你可以这样记

**vtable：函数目录**

```text
read() → 去哪里执行
name() → 去哪里执行
```

**vptr：目录地址**

```text
这个对象 → 应该查哪一张目录
```

两者配合：

```text
对象
 ↓
vptr
 ↓
vtable
 ↓
正确的虚函数
```

因此：

> **vptr 负责“找到表”，vtable 负责“找到函数”。**

最后补一个严谨的小点：**C++ 标准并没有强制编译器一定使用名为 `vptr` 和 `vtable` 的结构。** 但主流 C++ 编译器通常采用类似机制，所以学习虚函数底层原理时，一般就用 `vptr + vtable` 来解释。