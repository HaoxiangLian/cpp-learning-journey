#include <memory>

int main() {
    int* raw = new int{42};
    std::shared_ptr<int> first{raw};
    std::shared_ptr<int> second{raw};
    return *first + *second;
}
