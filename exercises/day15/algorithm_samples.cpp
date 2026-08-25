#include <algorithm>
#include <iostream>
#include <iterator>

// TODO 1：定义 bool is_even(int value)。

int main() {
    int samples[]{4, 9, 2, 7, 1};
    static_cast<void>(samples);  // TODO 完成前避免“未使用变量”警告。

    // TODO 2：用 std::find 查找 7，检查结果后输出 found: 7。
    // TODO 3：捕获 threshold，用 std::count_if 统计不小于阈值的元素。
    // TODO 4：把 is_even 作为谓词，统计偶数数量。
    // TODO 5：用 std::sort 排序，并输出全部元素。

    std::cout << "完成 TODO 后应输出查找、统计和排序结果\n";
}
