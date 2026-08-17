#include <iostream>

static int file_limit{80};

int next_ticket() {
    static int current{100};
    return ++current;
}

struct Registry {
    inline static int registered{0};
};

int main() {
    int reading{20};
    {
        int reading{35};
        std::cout << "inner reading: " << reading << '\n';
    }

    std::cout << "outer reading: " << reading << '\n';
    std::cout << "ticket: " << next_ticket() << '\n';
    std::cout << "ticket: " << next_ticket() << '\n';

    ++Registry::registered;
    ++Registry::registered;
    std::cout << "registered: " << Registry::registered << '\n';
    std::cout << "file limit: " << file_limit << '\n';
}
