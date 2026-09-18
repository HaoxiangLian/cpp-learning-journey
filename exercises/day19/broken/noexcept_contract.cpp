// INTENTIONALLY INVALID: 静态断言要求清理函数满足 noexcept，但声明尚未给出该保证。
// 修复方向：确认实现确实不让异常离开后，将接口声明为 noexcept。
void close_device() {}

static_assert(noexcept(close_device()), "close_device must be noexcept");

int main() {
    close_device();
}
