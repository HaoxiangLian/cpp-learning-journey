// INTENTIONALLY INVALID: unique_ptr 的复制构造被禁用，本文件应在编译阶段失败。
// 修复方向：使用 std::move 转移所有权，或重新检查是否真的需要共享所有权。
#include <memory>

int main() {
    auto first = std::make_unique<int>(7);
    auto second = first;
    return *second;
}
