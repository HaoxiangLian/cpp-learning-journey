#include <iostream>

int namespace_counter{100};

void automatic_demo() {
    int counter{};
    ++counter;
    std::cout << "automatic counter: " << counter << '\n';
}

int next_static_value() {
    static int counter{};
    ++counter;
    return counter;
}

void recursive_trace(int depth) {
    int marker{depth};
    std::cout << "enter: " << marker << '\n';

    if (depth > 0) {
        recursive_trace(depth - 1);
    }

    std::cout << "leave: " << marker << '\n';
}

int main() {
    std::cout << "namespace counter: " << namespace_counter << '\n';
    ++namespace_counter;
    std::cout << "namespace counter: " << namespace_counter << '\n';

    automatic_demo();
    automatic_demo();

    std::cout << "static counter: " << next_static_value() << '\n';
    std::cout << "static counter: " << next_static_value() << '\n';

    recursive_trace(2);
    return 0;
}
