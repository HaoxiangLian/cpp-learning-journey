#include <iostream>

int main() {
    int* dangling{nullptr};

    {
        int local_value{42};
        dangling = &local_value;
    }

    std::cout << *dangling << '\n';
    return 0;
}
