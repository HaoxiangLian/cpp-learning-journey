#include <deque>
#include <iostream>
#include <list>
#include <string>
#include <vector>

int main() {
    std::vector<int> readings{4, 9, 2};
    std::deque<std::string> jobs{"normal"};
    std::list<int> checkpoints{10, 20, 30};

    static_cast<void>(readings);
    static_cast<void>(jobs);
    static_cast<void>(checkpoints);

    // TODO 1：为 readings 预留空间并在末尾加入 7。
    // TODO 2：按“urgent、normal、later”的顺序组织 jobs。
    // TODO 3：删除 checkpoints 中的 20，同时验证原 first 迭代器仍可用。
    // TODO 4：让 readings 必然重新分配，用下标重新取得值 9 的位置。
    // TODO 5：按 README 要求输出三个容器的结果。

    std::cout << "完成 TODO 后应输出三种顺序容器的状态\n";
}
