#include <iostream>

class TaskSession {
public:
    explicit TaskSession(int id)
        : id_{id}, active_{false} {
        // TODO: 将会话设为活动，并输出 opened 信息。
    }

    ~TaskSession() {
        // TODO: 输出 closed 信息。
    }

    void finish() {
        // TODO: 结束会话并输出 finished 信息。
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
