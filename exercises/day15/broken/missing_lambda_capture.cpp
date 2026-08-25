// INTENTIONALLY INVALID: the lambda uses threshold without capturing it.
#include <algorithm>
#include <iterator>

int main() {
    int values[]{2, 7, 4};
    int threshold = 5;
    const auto count = std::count_if(
        std::begin(values), std::end(values),
        [](int value) { return value >= threshold; });
    return count == 1 ? 0 : 1;
}
