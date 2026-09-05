#include <iostream>
#include <utility>

struct Message {};

void consume(const Message&) { std::cout << "read message\n"; }
void consume(Message&&) { std::cout << "move message\n"; }

template <class T>
void relay(T&& message) {
    consume(std::forward<T>(message));
}

int main() {
    Message message;
    const Message fixed;
    relay(message);
    relay(Message{});
    relay(fixed);
}
