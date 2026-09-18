#include <iostream>
#include <utility>

struct Message {};

void consume(const Message&) { std::cout << "read message\n"; }
void consume(Message&&) { std::cout << "move message\n"; }

template <class T>
void relay(T&& message) {
    // TODO: 保留调用者传入表达式原来的左值/右值性质。
    consume(message);
}

int main() {
    Message message;
    relay(message);
    relay(Message{});
}
