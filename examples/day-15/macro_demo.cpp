#include <iostream>

#define BAD_SQUARE(x) x * x
#define PAREN_SQUARE(x) ((x) * (x))

int main() {
    const int bad_result{BAD_SQUARE(1 + 2)};
    const int good_result{PAREN_SQUARE(1 + 2)};

    std::cout << "bad square: " << bad_result << '\n';
    std::cout << "parenthesized square: " << good_result << '\n';

#if defined(ENABLE_TRACE)
    std::cout << "trace enabled\n";
#endif
}
