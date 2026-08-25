因为这条语句不是“先创建 `robot`，再调用一个普通函数”，而是在定义并初始化一个 `Robot` 对象。C++ 规定：类类型对象初始化时，要选择合适的构造函数来完成初始化。

```cpp
Robot robot{"机械臂"};
```

可以拆成三部分理解：

```cpp
Robot       robot       {"机械臂"};
类型名称     对象名称       初始化参数
```

编译器看到：

1. 要创建一个类型为 `Robot` 的对象。
    
2. 初始化参数有一个：`"机械臂"`。
    
3. 到 `Robot` 类中寻找能够接收一个对应参数的构造函数。
    
4. 找到后，用它构造 `robot`。
    

例如：

```cpp
class Robot {
public:
    Robot(const std::string& name)
        : name_{name} {
        std::cout << "构造 Robot：" << name_ << '\n';
    }

private:
    std::string name_;
};
```

编译器找到的是：

```cpp
Robot(const std::string& name)
```

因为它需要一个字符串参数，所以能够匹配：

```cpp
Robot robot{"机械臂"};
```

其中字符串字面量 `"机械臂"` 可以被转换成 `std::string`，再绑定到参数 `name`。

## 实际执行过程

执行下面这行：

```cpp
Robot robot{"机械臂"};
```

可以按照以下顺序理解：

```text
1. 准备存放 robot 对象的位置
2. 寻找匹配的 Robot 构造函数
3. 用“机械臂”初始化构造函数的参数 name
4. 执行 name_{name}，初始化成员 name_
5. 执行构造函数体
6. 构造完成，robot 可以正常使用
```

对应代码：

```cpp
Robot(const std::string& name)
    : name_{name} {                          // 第一步：初始化成员
    std::cout << "构造 Robot：" << name_;    // 第二步：执行函数体
}
```

因此输出：

```text
构造 Robot：机械臂
```

## 可以使用一个理解模型

可以暂时把：

```cpp
Robot robot{"机械臂"};
```

想象成编译器在做：

```text
在 robot 所在的位置，
使用参数“机械臂”执行 Robot 的构造过程。
```

它不是下面这种过程：

```cpp
Robot robot;                // 先创建
robot.Robot("机械臂");      // 再手动调用构造函数
```

后面这种写法是错误的，因为构造函数不能像普通成员函数一样手动调用。构造函数属于对象初始化过程。

## 构造函数根据参数选择

假设类中有多个构造函数：

```cpp
class Robot {
public:
    Robot() {
        std::cout << "默认构造\n";
    }

    Robot(const std::string& name) {
        std::cout << "使用名字构造：" << name << '\n';
    }

    Robot(const std::string& name, int id) {
        std::cout << "使用名字和编号构造\n";
    }
};
```

不同写法会选择不同构造函数：

```cpp
Robot first{};
```

没有提供参数，调用：

```cpp
Robot()
```

```cpp
Robot second{"机械臂"};
```

提供一个参数，调用：

```cpp
Robot(const std::string& name)
```

```cpp
Robot third{"机械臂", 7};
```

提供两个参数，调用：

```cpp
Robot(const std::string& name, int id)
```

如果没有匹配的构造函数：

```cpp
Robot robot{100, 200, 300};
```

编译器找不到接收三个整数的构造函数，就会产生编译错误。

## 为什么必须调用构造函数？

假设 `Robot` 有以下成员：

```cpp
class Robot {
private:
    std::string name_;
    int battery_;
};
```

对象刚创建时，必须确定：

- `name_` 是什么？
    
- `battery_` 是多少？
    
- 对象是否满足使用条件？
    

构造函数就是用来建立这个初始状态的：

```cpp
Robot(const std::string& name)
    : name_{name}, battery_{100} {}
```

执行：

```cpp
Robot robot{"机械臂"};
```

构造完成后：

```text
robot.name_    = "机械臂"
robot.battery_ = 100
```

所以，准确说法是：

> `Robot robot{"机械臂"};` 定义并初始化了一个 `Robot` 对象。因为 `Robot` 是类类型，编译器会根据初始化参数选择匹配的构造函数来完成对象初始化。花括号提供构造参数，真正触发构造的是“创建并初始化类对象”这件事。