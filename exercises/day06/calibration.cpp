#include <iostream>

void clamp_non_negative(int& value) {
    // TODO: 负数改为 0。
    (void)value;
}

bool write_if_present(int* target, int value) {
    // TODO: 先处理 nullptr，再通过指针写入。
    (void)target;
    (void)value;
    return false;
}

int main() {
    int speed{-4};
    // TODO: 校准 speed，再写入 12，并测试空指针分支。
    clamp_non_negative(speed);
    (void)write_if_present(&speed, 12);

    int* empty{nullptr};
    std::cout << "speed: " << speed << '\n';
    std::cout << std::boolalpha
              << "empty write: " << write_if_present(empty, 5) << '\n';
}
