#include <memory>

int main() {
    auto first = std::make_unique<int>(42);
    auto second = first;
    return *second;
}
