#include <iostream>

bool assign_if_present(int* target, int value) {
    if (target == nullptr) {
        return false;
    }
    *target = value;
    return true;
}

void increase(int& value) {
    ++value;
}

int main() {
    int speed{10};
    int* speed_pointer{&speed};
    int& speed_reference{speed};

    *speed_pointer = 12;
    increase(speed_reference);
    assign_if_present(&speed, 17);

    const int limit{20};
    const int* limit_view{&limit};
    int* optional_target{nullptr};

    std::cout << "speed: " << speed << '\n';
    std::cout << "through pointer: " << *speed_pointer << '\n';
    std::cout << "through reference: " << speed_reference << '\n';
    std::cout << "limit: " << *limit_view << '\n';
    std::cout << std::boolalpha
              << "empty update: " << assign_if_present(optional_target, 5) << '\n';
}
