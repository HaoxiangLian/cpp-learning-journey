# 第 1 天练习单

先独立完成，再回到 ChatGPT 对话中提交。暂时不要让 AI 直接代写答案。

## 练习 1：预测输出

不运行代码，先写出你认为的输出：

```cpp
#include <iostream>

int main() {
    std::cout << "Robot\n";
    std::cout << "Learning\n";
    return 0;
}
```

然后亲自运行并核对。


## 练习 2：修改程序

修改 `examples/day-01/hello.cpp`，使它输出：

```text
Robot learning started!
```

## 练习 3：独立编写

新建一个 `.cpp` 文件，让它分三行输出：

```text
Name: HaoxiangLian
Direction: Robotics
Day: 1
```

## 练习 4：制造并修复错误

1. 删除一条输出语句末尾的分号
2. 尝试编译
3. 记录错误信息中最关键的一行
4. 恢复分号并再次编译运行

## 练习 5：用自己的话解释

每题用一两句话回答：

1. 什么是源代码？
2. 编译器做什么？
3. 什么是可执行文件？
4. `main` 有什么作用？
5. 修改源代码后为什么需要重新编译？

## 对话提交模板

```text
练习1的预测：

Robot
Learning 

练习2的代码：

#include <iostream>

int main() {
    std::cout << "Robot learning started!\n";
    return 0;
}

练习3的代码：

#include <iostream>

int main() {
    std::cout << "Name: HaoxiangLian\n";
    std::cout << "Direction: Robotics\n";
    std::cout << "Day: 1\n";
    return 0;
}

练习4的关键报错：

练习5的回答：
1.是程序员编写的可以让人读懂的代码文本
2.编译器是把源文件翻译成计算机可以读懂的代码
3.编译生成的可执行程序
4.程序运行时跑的主函数
5.编译以后才能将改动更新到新的可执行文件中。

我仍然不理解的地方：
```

把填写后的内容发到 ChatGPT 对话中，我会逐项检查，并根据你的答案决定是否补讲或进入第 2 天。
