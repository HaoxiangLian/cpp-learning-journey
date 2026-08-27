// INTENTIONALLY INVALID: std::sort requires random-access iterators.
#include <algorithm>
#include <iterator>
#include <sstream>

int main() {
    std::istringstream input{"8 3 5"};
    std::istream_iterator<int> first{input};
    std::istream_iterator<int> last{};
    std::sort(first, last);
}
