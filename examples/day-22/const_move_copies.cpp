#include <iostream>
#include <utility>

class Tracker {
public:
    Tracker() = default;
    Tracker(const Tracker&) { std::cout << "copy\n"; }
    Tracker(Tracker&&) noexcept { std::cout << "move\n"; }
};

int main() {
    const Tracker source;
    Tracker target{std::move(source)};
}
