// INTENTIONALLY INVALID: dereferencing an iterator invalidated by reallocation is UB.
#include <vector>

int main() {
    std::vector<int> values{4, 9, 2};
    auto old = values.begin();
    values.reserve(values.capacity() + 1);
    return *old;
}
