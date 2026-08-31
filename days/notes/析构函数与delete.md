这里要区分两个很容易混淆的东西：

- **析构函数 `~ScanTask()`**：负责“清理对象内部的资源”。
    
- **`delete p`**：负责“销毁 `p` 指向的对象，并释放 `new` 申请的内存”。
    

所以，析构函数里面**不一定要写 `delete`**。

你的代码：

```cpp
class ScanTask : public Task {
public:
    ~ScanTask() override {
        std::cout << "destroy ScanTask\n";
    }
};
```

析构函数只是说：

> “当 `ScanTask` 对象要被销毁时，我需要做这些清理工作。”

而真正发起“销毁这个对象”的，是：

```cpp
delete p;
```

---

### `delete p` 到底做了什么？

这里：

```cpp
ScanTask* p = new ScanTask;
```

可以理解为：

```text
new ScanTask
    ↓
在动态内存中创建一个 ScanTask 对象
    ↓
返回它的地址
    ↓
p 保存这个地址
```

然后：

```cpp
delete p;
```

大致会做两件事：

```text
delete p
   │
   ├─ ① 调用析构函数
   │      ~ScanTask()
   │          ↓
   │      ~Task()
   │
   └─ ② 释放 new ScanTask 得到的那块内存
```

所以你不需要自己写：

```cpp
~ScanTask() {
    delete ???;
}
```

因为**当前这个类根本没有需要手动 `delete` 的成员变量。**

---

### 那什么时候析构函数里面会出现 `delete`？

假如一个类内部自己又 `new` 了一块内存：

```cpp
class Data {
private:
    int* p_;

public:
    Data() {
        p_ = new int(10);
    }

    ~Data() {
        delete p_;   // ✅ 释放自己拥有的动态资源
    }
};
```

然后：

```cpp
Data* d = new Data;

delete d;
```

这里实际上发生：

```text
delete d
   ↓
调用 Data::~Data()
   ↓
delete p_           ← 释放对象内部申请的 int
   ↓
Data 对象析构完成
   ↓
释放 d 指向的 Data 对象本身占用的内存
```

也就是说有**两层不同的内存**：

```text
d
│
↓
┌──────────────┐
│ Data 对象    │ ← new Data 创建
│              │
│ p_ ──────────┼────→ [ int 10 ] ← new int 创建
└──────────────┘
```

所以：

```cpp
~Data() {
    delete p_;
}
```

里面的 `delete p_` 是：

> 清理 **Data 自己拥有的资源**。

外面的：

```cpp
delete d;
```

是：

> 销毁 **Data 对象本身**。

---

### 回到你的 `ScanTask`

你的类只有：

```cpp
class ScanTask : public Task {
public:
    ~ScanTask() override {
        std::cout << "destroy ScanTask\n";
    }
};
```

没有：

```cpp
int* data_;
double* buffer_;
SomeType* ptr_;
```

之类自己用 `new` 申请的资源。

所以析构函数只打印一句话完全没问题：

```cpp
~ScanTask() override {
    std::cout << "destroy ScanTask\n";
}
```

真正销毁对象：

```cpp
ScanTask* p = new ScanTask;

delete p;
```

---

还有一个特别重要的点：**不要把“析构函数”和 `delete` 理解成同一个东西。**

可以先记住：

> **`delete` 是“发出销毁命令”；析构函数是“对象收到销毁命令后，执行自己的收尾工作”。**

因此：

```cpp
delete p;
```

会自动调用析构函数，你通常**不需要自己手动调用析构函数**。