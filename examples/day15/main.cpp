#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>

bool is_even(int value) {
    return value % 2 == 0;
}

int main() {
    int samples[]{4, 9, 2, 7, 1};
    auto first = std::begin(samples);
    auto last = std::end(samples);

    std::cout << "first: " << *first << '\n';

    const auto found = std::find(first, last, 7);
    std::cout << std::boolalpha
              << "found 7: " << (found != last) << '\n';

    const int threshold = 5;
    const auto at_least_threshold = [threshold](int value) {
        return value >= threshold;
    };

    std::cout << "at least 5: "
              << std::count_if(first, last, at_least_threshold) << '\n';
    std::cout << "even: "
              << std::count_if(first, last, is_even) << '\n';

    std::istringstream input{"8 3 5"};
    std::istream_iterator<int> input_first{input};
    std::istream_iterator<int> input_last{};
    const auto stream_found = std::find(input_first, input_last, 3);
    std::cout << "stream found 3: " << (stream_found != input_last) << '\n';

    std::sort(first, last);
    std::cout << "sorted:";
    for (const int value : samples) {
        std::cout << ' ' << value;
    }
    std::cout << '\n';
}
