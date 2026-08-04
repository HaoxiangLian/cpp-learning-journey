#include <iostream>
#include <limits>

constexpr int square(int value) {
    return value * value;
}

int read_runtime_value() {
    return 7;
}

enum class Mode : unsigned char {
    idle = 0,
    active = 1
};

int main() {
    constexpr int squared{square(6)};
    static_assert(squared == 36);

    const int snapshot{read_runtime_value()};

    const double angle{42.9};
    int whole_degrees{};
    if (angle >= static_cast<double>(std::numeric_limits<int>::min()) &&
        angle <= static_cast<double>(std::numeric_limits<int>::max())) {
        whole_degrees = static_cast<int>(angle);
    }

    const int mode_code{static_cast<int>(Mode::active)};

    int value{7};
    const int* view{&value};
    value = 12;

    std::cout << "compile-time square: " << squared << '\n';
    std::cout << "runtime const snapshot: " << snapshot << '\n';
    std::cout << "explicit whole degrees: " << whole_degrees << '\n';
    std::cout << "mode code: " << mode_code << '\n';
    std::cout << "pointer view: " << *view << '\n';
}
