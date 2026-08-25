- **非静态成员函数**：操作某一个具体对象，内部存在隐含的 `this` 指针。
- **静态成员函数**：属于类本身，不对应某个具体对象，因此没有 `this` 指针。

```
#include <iostream>

class Robot {
private:
    int speed_ = 0;          // 非静态成员变量：每个对象各有一份
    static int robotCount_;  // 静态成员变量：所有对象共享一份

public:
    // 非静态成员函数
    void setSpeed(int speed) {
        speed_ = speed;      // 实际上相当于 this->speed_ = speed
        ++robotCount_;       // 也可以访问静态成员变量
    }

    // 非静态成员函数
    void showSpeed() const {
        std::cout << speed_ << '\n';
    }

    // 静态成员函数
    static void showRobotCount() {
        std::cout << robotCount_ << '\n';

        // std::cout << speed_;  // 错误：不知道要访问哪个对象的 speed_
    }
};

// 在类外定义静态成员变量
int Robot::robotCount_ = 0;
```

调用方式：
```
int main() {

    Robot robot1;

    Robot robot2;

  

    robot1.setSpeed(10);

    robot2.setSpeed(20);

  

    robot1.showSpeed();          // 输出 10

    robot2.showSpeed();          // 输出 20

  

    Robot::showRobotCount();     // 推荐：通过类名调用静态成员函数

}

```

## 1. 是否需要具体对象

非静态成员函数必须针对某个对象调用：

```
Robot robot1;

robot1.setSpeed(10);
```

这里的意思是：“把 `robot1` 的速度设置为 10。”

下面通常是错误的：
```
Robot::setSpeed(10);  // 错误：没有说明设置哪个 Robot 对象
```
静态成员函数不需要对象，可以通过类名调用：
```
Robot::showRobotCount();
```
因为它处理的是整个类共享的信息，而不是某一个机器人的信息。

## 2. 是否具有 `this` 指针

非静态成员函数内部具有隐含的 `this` 指针。

例如：
```
void setSpeed(int speed) {

    speed_ = speed;

}
```

可以理解成：
```
void setSpeed(int speed) {

    this->speed_ = speed;

}
```

假设调用：
```
robot1.setSpeed(10);
```

那么函数中的 `this` 指向 `robot1`。

静态成员函数没有 `this` 指针：
```
static void showRobotCount() {

    // 没有 this

}
```

因为调用静态成员函数时可能根本没有创建任何对象：
```
Robot::showRobotCount();
```
## 3. 可以访问哪些成员

|函数类型|直接访问非静态成员|直接访问静态成员|
|---|---|---|
|非静态成员函数|可以|可以|
|静态成员函数|不可以|可以|