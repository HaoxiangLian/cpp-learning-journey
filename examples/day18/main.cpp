#include <iostream>
#include <memory>

class Session {
public:
    explicit Session(int id) : id_{id} {}

    ~Session() {
        std::cout << "session " << id_ << " destroyed\n";
    }

    int id() const {
        return id_;
    }

private:
    int id_;
};

int main() {
    std::cout << std::boolalpha;

    auto source = std::make_unique<Session>(7);
    auto destination = std::move(source);
    std::cout << "unique id: " << destination->id() << '\n';
    std::cout << "source empty: " << (source == nullptr) << '\n';

    std::weak_ptr<Session> observer;
    {
        auto first = std::make_shared<Session>(9);
        auto second = first;
        observer = first;

        std::cout << "shared count: " << first.use_count() << '\n';
        first.reset();
        std::cout << "after reset count: " << second.use_count() << '\n';
        std::cout << "weak alive: " << static_cast<bool>(observer.lock()) << '\n';
    }

    std::cout << "weak expired: " << observer.expired() << '\n';
}
