#include <limits>

constexpr int maximum{std::numeric_limits<int>::max()};
constexpr int invalid{maximum + 1};

int main() {
    return invalid;
}
