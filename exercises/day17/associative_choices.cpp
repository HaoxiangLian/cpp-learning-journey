#include <cstddef>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <unordered_map>

struct LastDigitHash {
    std::size_t operator()(int key) const {
        return static_cast<std::size_t>(key % 10);
    }
};

int main() {
    std::map<int, std::string> ordered_status;
    std::set<int> alarm_ids;
    std::unordered_map<int, std::string, LastDigitHash> labels;

    static_cast<void>(ordered_status);
    static_cast<void>(alarm_ids);
    static_cast<void>(labels);

    // TODO 1：加入三个状态，并按键序输出 7、12、20。
    // TODO 2：向 alarm_ids 插入 7、7、12，输出唯一键数量。
    // TODO 3：向 labels 加入 12 -> force、22 -> vision，验证二者同桶但都可查找。
    // TODO 4：保存 12 对应字符串的指针，强制 rehash 后继续通过指针读取。
    // TODO 5：丢弃 rehash 前的迭代器，重新 find(12) 并输出键。

    std::cout << "完成 TODO 后应输出有序与哈希容器的状态\n";
}
