# 第 3 天练习单

主题：条件、循环与代码块

## 作答方式

- 文字题直接在题目下方的空白处填写
- 代码题直接在紧随题目的 `cpp` 代码块中编写
- 预测题先填写预测，再运行并填写实际结果
- 完成后保存并同步到 GitHub

---

## 练习 1：判断条件的真假

已知：

```cpp
double battery{35.0};
int mode{2};
bool ready{false};
```

分别判断以下表达式的结果是 `true` 还是 `false`，并用一句话说明原因。

1. `battery < 20.0`

   结果：

   > 

   原因：

   > 

2. `battery >= 20.0 && battery < 50.0`

   结果：

   > 

   原因：

   > 

3. `mode == 1 || mode == 2`

   结果：

   > 

   原因：

   > 

4. `!ready`

   结果：

   > 

   原因：

   > 

---

## 练习 2：预测 `if` 判断

先不要运行：

```cpp
#include <iostream>

int main() {
    int temperature{65};

    if (temperature > 80) {
        std::cout << "High\n";
    } else if (temperature >= 40) {
        std::cout << "Normal\n";
    } else {
        std::cout << "Low\n";
    }

    return 0;
}
```

**预测：**

```text

```

**实际运行结果：**

```text

```

**程序为什么选择这一条路线？**

> 

---

## 练习 3：修复条件错误

下面的程序本想判断 `mode` 是否等于 `1`：

```cpp
#include <iostream>

int main() {
    int mode{2};

    if (mode = 1) {
        std::cout << "Manual\n";
    } else {
        std::cout << "Other\n";
    }

    return 0;
}
```

1. 哪一处写错了？

   > 

2. `=` 和 `==` 在这里分别表示什么？

   > 

3. 写出修复后的完整代码：

```cpp



```

---

## 练习 4：理解代码块和作用域

阅读：

```cpp
#include <iostream>

int main() {
    int speed{10};

    if (speed > 0) {
        int doubled{speed * 2};
        std::cout << doubled << '\n';
    }

    std::cout << doubled << '\n';
    return 0;
}
```

1. 这段代码能否成功编译？

   > 

2. 如果不能，哪一行有问题？

   > 

3. 为什么 `speed` 能在 `if` 代码块中使用，而 `doubled` 不能在代码块外使用？

   > 

4. 写出一种修复方法：

```cpp



```

---

## 练习 5：补全 `switch`

编写完整程序，读取整数 `mode`：

- 输入 `1`，输出 `Manual`
- 输入 `2`，输出 `Automatic`
- 输入其他整数，输出 `Unknown`

必须使用 `switch`。

```cpp





```

**一组输入与实际输出：**

```text

```

---

## 练习 6：跟踪 `for` 循环

先不要运行：

```cpp
#include <iostream>

int main() {
    int total{0};

    for (int i{1}; i <= 4; ++i) {
        total = total + i;
        std::cout << i << ' ' << total << '\n';
    }

    return 0;
}
```

填写每轮结束时的值：

| 第几轮 | `i` | `total` |
|---:|---:|---:|
| 1 |  |  |
| 2 |  |  |
| 3 |  |  |
| 4 |  |  |

**预测的完整输出：**

```text

```

**实际运行结果：**

```text

```

---

## 练习 7：编写 `while` 循环

使用 `while` 输出倒计时：

```text
3
2
1
Start
```

要求：

- 使用一个 `int` 变量保存当前数字
- 每轮循环让它减少 `1`
- 循环结束后输出 `Start`

```cpp





```

**实际输出：**

```text

```

---

## 练习 8：独立编写机器人安全检查

编写一个完整程序：

1. 读取一个 `double` 类型的关节角度
2. 角度小于 `-180.0` 或大于 `180.0` 时，输出 `Invalid angle`
3. 否则输出 `Angle accepted`
4. 如果角度有效，再读取一个正整数 `repeat_count`
5. 使用 `for` 循环输出 `Move`，次数等于 `repeat_count`

```cpp








```

**第一组输入与实际输出：**

```text

```

**第二组输入与实际输出：**

```text

```

---

## 练习 9：用自己的话解释

不要求背教材原句。每题说明“它解决什么问题”和“程序怎样执行”即可。

1. `if` 有什么作用？

   > 

2. `for` 和 `while` 都能循环，它们通常分别适合什么情况？

   > 

3. 什么是代码块？

   > 

4. 什么是变量的作用域？

   > 

5. 为什么循环中经常需要修改计数变量？

   > 

---

## 学习复盘

**今天最清楚的知识点：**

> 

**仍然不理解或想继续追问的地方：**

> 

完成后保存并同步到 GitHub，然后在对话中告诉我：

> 请批改第 3 天
