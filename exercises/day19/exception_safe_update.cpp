#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

void set_level(int& level, int candidate) {
    // TODO: 仅接受 0—100；失败时抛出 invalid_argument，且不能改变 level。
    level = candidate;
}

void parse_and_append(std::vector<int>& values, const std::string& text) {
    // TODO: 完整解析非负整数后再提交修改；失败时 values 保持不变。
    (void)values;
    (void)text;
}

int main() {
    int level{20};
    std::vector<int> values{1, 2};

    try {
        set_level(level, -5);
    } catch (const std::exception&) {
        std::cout << "level: " << level << '\n';
    }

    try {
        parse_and_append(values, "7x");
    } catch (const std::exception&) {
        std::cout << "size: " << values.size() << '\n';
    }
}
