#include <iostream>

void clamp_non_negative(int& value) {
    if (value < 0) {
        value = 0;
    }
}

bool write_if_present(int* target, int value) {
    if (target == nullptr) {
        return false;
    }
    *target = value;
    return true;
}

int main() {
    int speed{-4};
    clamp_non_negative(speed);
    write_if_present(&speed, 12);

    int* empty{nullptr};
    std::cout << "speed: " << speed << '\n';
    std::cout << std::boolalpha
              << "empty write: " << write_if_present(empty, 5) << '\n';
}
