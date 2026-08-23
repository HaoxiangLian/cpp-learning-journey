#include <iostream>
#include <string>
#include <type_traits>

// TODO 1：定义 larger(T, T)，返回两个同类型值中较大的一个。

// TODO 2：定义 Reading<T>，保存一个数值和一个单位字符串。
// 对 T 使用 static_assert，要求 std::is_arithmetic_v<T> 为 true。

// TODO 3：定义 TypeName<T> 主模板，get() 返回 "other"。

// TODO 4：为 TypeName<bool> 编写显式特化，get() 返回 "boolean"。

int main() {
    std::cout << "完成 TODO 后应输出 max count、temperature 和 bool label\n";
}
