#include <iostream>
#include <limits>

int main(int argc, char**) {
    const int maximum{std::numeric_limits<int>::max()};
    const int input{argc == 1 ? maximum : 0};
    const int overflowed{input + 1};
    std::cout << overflowed << '\n';
}
