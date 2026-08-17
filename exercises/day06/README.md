# 第 6 天练习：指针、引用与生命周期

先独立作答，不要提前查看 `solutions/day06/`。

## 一、概念判断

1. 只要一个对象具有自动存储期，C++ 标准就保证它一定在机器栈上。
2. 指针形参按值传递，所以函数不能通过它修改所指对象。
3. `pointer != nullptr` 只能排除空指针，不能排除悬空或越界。
4. 执行 `reference = other;` 会让普通左值引用改绑到 `other`。
5. 裸指针和引用本身都不会自动延长普通局部对象的生命周期。

## 二、代码输出或错误分析

### A. 指针形参

```cpp
#include <iostream>

void set_value(int* target) {
    *target = 9;
    target = nullptr;
}

int main() {
    int value{3};
    int* pointer{&value};
    set_value(pointer);
    std::cout << value << ' ' << std::boolalpha << (pointer == nullptr) << '\n';
}
```

写出输出，并区分形参指针对象与调用方指针对象。

### B. 引用赋值

```cpp
#include <iostream>

int main() {
    int first{1};
    int second{8};
    int& alias{first};
    alias = second;
    std::cout << first << ' ' << second << '\n';
}
```

引用是否改绑？写出输出。

### C. 悬空分析

```cpp
int* pointer{nullptr};
{
    int sample{42};
    pointer = &sample;
}
```

到达块外时，分别说明 `pointer` 和 `sample` 是否仍处于生命周期内。若随后读取 `*pointer`，错误类别是什么？不得预测固定症状。

阅读 [`broken/uninitialized_reference.cpp`](broken/uninitialized_reference.cpp)，确认它是故意编译错误，不进入正常构建。

## 三、小型编程题：可选写入与原地校准

补全 [`calibration.cpp`](calibration.cpp)：

1. `clamp_non_negative(int& value)` 把负数改为 `0`，非负数保持不变。
2. `write_if_present(int* target, int value)` 在 `target == nullptr` 时返回 `false`；否则写入并返回 `true`。
3. `main` 先把 `speed{-4}` 校准为 `0`，再通过指针写为 `12`。
4. 对空指针调用一次，输出 `speed: 12` 与 `empty write: false`。

编译建议：

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror calibration.cpp -o calibration
./calibration
```

完成后再查看 [`../../solutions/day06/README.md`](../../solutions/day06/README.md)。
