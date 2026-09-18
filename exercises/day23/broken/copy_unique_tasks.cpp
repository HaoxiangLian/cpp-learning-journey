// INTENTIONALLY INVALID: copying unique ownership must fail to compile.
#include <memory>
#include <vector>

int main() {
    std::vector<std::unique_ptr<int>> first;
    first.push_back(std::make_unique<int>(7));
    auto second = first;
    return *second.front();
}

