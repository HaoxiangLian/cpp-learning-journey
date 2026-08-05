#include <cstddef>
#include <iostream>

bool count_call(int& calls) {
    ++calls;
    return true;
}

int main() {
    int calls{};
    const bool logical_and{false && count_call(calls)};
    std::cout << "short-circuit &&: " << calls << " calls\n";

    calls = 0;
    const bool logical_or{true || count_call(calls)};
    std::cout << "short-circuit ||: " << calls << " calls\n";

    int value{3};
    const int prefix_result{++value};
    std::cout << "prefix result: " << prefix_result << ", object: " << value << '\n';

    const int postfix_result{value++};
    std::cout << "postfix result: " << postfix_result << ", object: " << value << '\n';

    int values[3]{10, 20, 30};
    int total{};
    for (const int element : values) {
        total += element;
    }
    std::cout << "range total: " << total << '\n';

    int* const begin{values};
    int* const end{values + 3};
    const std::ptrdiff_t distance{end - begin};
    std::cout << "one-past distance: " << distance << '\n';

    return logical_and || !logical_or;
}
