#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

void set_level(int& level, int candidate) {
    if (candidate < 0 || candidate > 100) {
        throw std::invalid_argument{"level out of range"};
    }
    level = candidate;
}

void parse_and_append(std::vector<int>& values, const std::string& text) {
    std::size_t used{0};
    const int candidate{std::stoi(text, &used)};
    if (used != text.size() || candidate < 0) {
        throw std::invalid_argument{"invalid non-negative integer"};
    }
    values.push_back(candidate);
}

void close_device() noexcept {
    // 清理失败若可恢复，应在这里记录或转成状态，不能让异常越过 noexcept 边界。
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

    close_device();
}
