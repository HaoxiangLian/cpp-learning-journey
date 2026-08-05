#include <limits>

int main() {
    volatile int maximum{std::numeric_limits<int>::max()};
    volatile int result{maximum + 1};
    return result;
}
