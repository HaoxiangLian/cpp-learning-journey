#include <iostream>
#include <utility>

struct Packet {};

void route(const Packet&) { std::cout << "lvalue route\n"; }
void route(Packet&&) { std::cout << "rvalue route\n"; }

template <class T>
void relay(T&& packet) {
    route(std::forward<T>(packet));
}

int main() {
    const int limit{7};
    auto copied = limit;
    copied = 8;
    const auto& view = limit;
    std::cout << "copied: " << copied << ", limit: " << limit << '\n';
    std::cout << "view: " << view << '\n';

    int value{10};
    decltype((value)) alias = value;
    alias = 11;
    std::cout << "value through decltype ref: " << value << '\n';

    Packet packet;
    relay(packet);
    relay(Packet{});
}
