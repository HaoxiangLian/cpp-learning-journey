可以把构造函数和析构函数理解为对象的两个生命周期关卡：

- 构造函数：对象“上岗”前，负责初始化。
    
- 析构函数：对象“下岗”前，负责清理。


## 一、构造函数

### 1. 定义

构造函数是类中的特殊成员函数：

- 函数名与类名完全相同。
    
- 没有返回类型，连 `void` 都不能写。
    
- 创建对象时自动调用。
    
- 可以有参数，也可以重载。


```cpp
class Robot {
public:
    Robot() {
        // 构造函数
    }
};
```

下面是错误写法：

```cpp
void Robot() {}   // 错误：构造函数不能写返回类型
```

### 2. 作用

构造函数最主要的作用是：

1. 初始化成员变量。
    
2. 检查传入参数是否合法。
    
3. 申请对象需要的资源，例如动态内存、文件、网络连接等。
    
4. 保证对象创建完成后处于可用状态。
    

例如，一个机器人对象创建出来以后，名称、编号等信息不应该是不确定的，因此可以通过构造函数设置这些信息。

## 二、构造函数的具体例子

```cpp
#include <iostream>
#include <string>

class Robot {
private:
    std::string name_;
    int id_;

public:
    // 无参数构造函数
    Robot()
        : name_("未命名机器人"), id_(0)
    {
        std::cout << name_ << " 构造完成\n";
    }

    // 有参数构造函数
    Robot(const std::string& name, int id)
        : name_(name), id_(id)
    {
        std::cout << name_ << " 构造完成\n";
    }

    void showInfo() const {
        std::cout << "名称：" << name_
                  << "，编号：" << id_ << '\n';
    }
};

int main() {
    Robot robot1;
    Robot robot2("手术机器人", 101);

    robot1.showInfo();
    robot2.showInfo();
}
```

输出大致为：

```text
未命名机器人 构造完成
手术机器人 构造完成
名称：未命名机器人，编号：0
名称：手术机器人，编号：101
```

执行：

```cpp
Robot robot1;
```

时，程序会自动调用：

```cpp
Robot()
```

执行：

```cpp
Robot robot2("手术机器人", 101);
```

时，程序会根据参数自动调用：

```cpp
Robot(const std::string& name, int id)
```

这说明构造函数可以重载：一个类可以有多个参数形式不同的构造函数。

## 三、成员初始化列表

下面这部分叫作成员初始化列表：

```cpp
Robot(const std::string& name, int id)
    : name_(name), id_(id)
{
}
```

它表示：

```cpp
name_ 使用 name 进行初始化
id_ 使用 id 进行初始化
```

执行顺序可以理解为：

```text
为对象准备内存
      ↓
根据初始化列表初始化成员变量
      ↓
执行构造函数函数体
      ↓
对象可以正常使用
```

也可以在构造函数体内写赋值：

```cpp
Robot(const std::string& name, int id) {
    name_ = name;
    id_ = id;
}
```

但这和初始化列表并不完全相同。

前一种方式：

```cpp
Robot(const std::string& name, int id)
    : name_(name), id_(id)
{
}
```

是直接初始化成员。

后一种方式：

```cpp
Robot(const std::string& name, int id) {
    name_ = name;
    id_ = id;
}
```

通常是成员先被默认初始化，然后再进行赋值。因此通常推荐使用成员初始化列表。

另外，下面这些成员必须使用初始化列表：

- `const` 成员变量。
    
- 引用成员变量。
    
- 没有默认构造函数的成员对象。
    

需要注意：成员真正的初始化顺序由它们在类中的声明顺序决定，而不是初始化列表中的书写顺序。

## 四、默认构造函数

不需要提供参数就能调用的构造函数，叫作默认构造函数：

```cpp
class Robot {
public:
    Robot() {
    }
};
```

使用方式：

```cpp
Robot robot;
```

如果类中没有写任何构造函数，编译器通常会尝试生成默认构造函数：

```cpp
class Robot {
private:
    int id_;
};

Robot robot;  // 编译器尝试调用隐式生成的默认构造函数
```

不过此时普通的内置类型成员，例如 `int id_`，可能没有得到有意义的初始值。可以写成：

```cpp
class Robot {
private:
    int id_{0};
};
```

还有一个容易出错的情况：

```cpp
class Robot {
public:
    Robot(int id) {
    }
};

int main() {
    Robot robot;   // 错误：没有无参数构造函数
}
```

只要自己声明了构造函数，编译器通常就不会再自动生成无参数构造函数。可以显式补上：

```cpp
class Robot {
public:
    Robot() = default;

    Robot(int id) {
    }
};
```

## 五、析构函数

### 1. 定义

析构函数也是类中的特殊成员函数，其格式为：

```cpp
~类名()
```

例如：

```cpp
class Robot {
public:
    ~Robot() {
        // 析构函数
    }
};
```

析构函数具有以下特点：

- 名字是类名前面加上 `~`。
    
- 没有返回类型。
    
- 不能有参数。
    
- 一个类只能有一个析构函数，不能重载。
    
- 对象生命周期结束时自动调用。
    

### 2. 作用

析构函数主要负责清理对象拥有的资源，例如：

- 释放动态申请的内存。
    
- 关闭文件。
    
- 断开网络连接。
    
- 释放互斥锁。
    
- 关闭设备连接。
    
- 执行其他结束工作。
    

如果类没有显式编写析构函数，编译器通常会自动生成一个析构函数。

## 六、观察对象的构造和析构过程

```cpp
#include <iostream>
#include <string>

class Robot {
private:
    std::string name_;

public:
    Robot(const std::string& name)
        : name_(name)
    {
        std::cout << name_ << "：构造函数被调用\n";
    }

    ~Robot() {
        std::cout << name_ << "：析构函数被调用\n";
    }
};

int main() {
    std::cout << "进入代码块\n";

    {
        Robot robot1("机器人1");
        Robot robot2("机器人2");

        std::cout << "正在使用机器人\n";
    }

    std::cout << "离开代码块\n";
}
```

输出为：

```text
进入代码块
机器人1：构造函数被调用
机器人2：构造函数被调用
正在使用机器人
机器人2：析构函数被调用
机器人1：析构函数被调用
离开代码块
```

对象在离开作用域时自动析构。

而且，同一个作用域中的局部对象通常按照与构造相反的顺序析构：

```text
先构造 robot1
再构造 robot2

先析构 robot2
再析构 robot1
```

可以把它理解为“后进先出”：

```text
构造顺序：robot1 → robot2
析构顺序：robot2 → robot1
```

## 七、析构函数释放动态内存

下面的类在构造时申请动态内存，在析构时释放：

```cpp
#include <cstddef>
#include <iostream>

class Buffer {
private:
    std::size_t size_;
    int* data_;

public:
    explicit Buffer(std::size_t size)
        : size_(size), data_(new int[size]{})
    {
        std::cout << "申请了 " << size_
                  << " 个 int 的内存\n";
    }

    ~Buffer() {
        delete[] data_;
        std::cout << "动态内存已经释放\n";
    }

    // 暂时禁止复制，避免两个对象释放同一块内存
    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;
};

int main() {
    {
        Buffer buffer(1000);
        std::cout << "正在使用 buffer\n";
    }

    std::cout << "buffer 已经销毁\n";
}
```

执行过程是：

```text
创建 buffer
    ↓
构造函数申请动态内存
    ↓
使用 buffer
    ↓
离开作用域
    ↓
析构函数执行 delete[]
    ↓
内存被释放
```

如果只申请而不释放：

```cpp
data_ = new int[size];
```

却没有对应的：

```cpp
delete[] data_;
```

就可能产生内存泄漏。

不过现代 C++ 中，实际开发通常优先使用：

```cpp
std::vector<int>
std::unique_ptr<int[]>
```

它们能够自动管理内存，减少手动使用 `new` 和 `delete` 带来的错误。

## 八、栈对象和动态对象的析构

### 局部对象

```cpp
void test() {
    Robot robot("机器人");
}
```

函数执行结束后，`robot` 自动调用析构函数。

### 动态对象

```cpp
Robot* robot = new Robot("机器人");

delete robot;
```

执行 `new` 时调用构造函数，执行 `delete` 时调用析构函数。

如果忘记：

```cpp
delete robot;
```

就会造成资源泄漏。现代 C++ 更推荐：

```cpp
#include <memory>

auto robot = std::make_unique<Robot>("机器人");
```

这样对象不再使用时会自动被销毁。

## 九、构造函数和析构函数对比

|对比项|构造函数|析构函数|
|---|---|---|
|名称|与类名相同|类名前加 `~`|
|返回类型|没有|没有|
|参数|可以有|不能有|
|能否重载|可以|不可以|
|调用时间|对象初始化时|对象生命周期结束时|
|主要作用|初始化对象、申请资源|清理对象、释放资源|

最核心的理解是：

```text
构造函数：让对象从“还不能使用”变成“可以使用”
析构函数：让对象在消失之前完成资源清理
```

这种“构造时获得资源、析构时释放资源”的思想，在 C++ 中叫作 RAII，也是 C++ 资源管理的核心思想。