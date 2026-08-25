#include <iostream>

class TaskSession {
public:
    explicit TaskSession(int id)
        : id_{id}, active_{true} {
        std::cout << "session " << id_ << " opened\n";
    }

    ~TaskSession() {
        std::cout << "session " << id_ << " closed\n";
    }

    void finish() {
        active_ = false;
        std::cout << "session " << id_ << " finished\n";
    }

    int id() const {
        return id_;
    }

    bool active() const {
        return active_;
    }

private:
    int id_;
    bool active_;
};

int main() {
    TaskSession session{42};
    std::cout << std::boolalpha << "active: " << session.active() << '\n';
    session.finish();
    std::cout << std::boolalpha << "active: " << session.active() << '\n';
}
