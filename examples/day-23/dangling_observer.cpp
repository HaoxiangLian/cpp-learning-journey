#include <iostream>
#include <memory>

int main() {
    auto owner = std::make_unique<int>(42);
    int* observer = owner.get();
    owner.reset();
    std::cout << *observer << '\n';
}
