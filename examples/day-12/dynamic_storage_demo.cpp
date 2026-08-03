#include <cstddef>
#include <iostream>

int main() {
    int* owner{new int{42}};
    int* observer{owner};

    std::cout << "single: " << *observer << '\n';
    *owner = 55;
    std::cout << "updated through owner: " << *observer << '\n';

    delete owner;
    owner = nullptr;
    observer = nullptr;

    constexpr std::size_t count{3};
    int* samples{new int[count]{4, 8, 12}};

    int total{};
    for (std::size_t index{}; index < count; ++index) {
        total += samples[index];
    }
    std::cout << "array total: " << total << '\n';

    delete[] samples;
    samples = nullptr;

    delete owner; // delete nullptr; has no effect.
}
