#include <algorithm>
#include <iostream>
#include <iterator>

bool is_even(int value) {
    return value % 2 == 0;
}

int main() {
    int samples[]{4, 9, 2, 7, 1};
    auto first = std::begin(samples);
    auto last = std::end(samples);

    const auto found = std::find(first, last, 7);
    if (found != last) {
        std::cout << "found: " << *found << '\n';
    }

    int threshold = 5;
    const auto at_least_threshold = [threshold](int value) {
        return value >= threshold;
    };

    std::cout << "at least 5: "
              << std::count_if(first, last, at_least_threshold) << '\n';
    std::cout << "even: " << std::count_if(first, last, is_even) << '\n';

    std::sort(first, last);
    std::cout << "sorted:";
    for (const int value : samples) {
        std::cout << ' ' << value;
    }
    std::cout << '\n';
}
