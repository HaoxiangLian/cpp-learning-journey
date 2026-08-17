#include <iostream>

int next_task_id() {
    static int current{500};
    return ++current;
}

int main() {
    std::cout << next_task_id() << ' ';

    {
        int current{900};  // 仅用于演示：这是另一个作用域中的另一个对象。
        std::cout << next_task_id() << ' ';
        (void)current;
    }

    std::cout << next_task_id() << '\n';
}
