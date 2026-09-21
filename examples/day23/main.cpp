#include <iostream>
#include <memory>
#include <utility>
#include <vector>

class Task {
public:
    virtual ~Task() = default;
    virtual int run() const = 0;
};

class Add final : public Task {
public:
    explicit Add(int value) : value_{value} {}
    int run() const override { return value_ + 1; }

private:
    int value_;
};

int main() {
    auto source = std::make_unique<Add>(4);
    std::vector<std::unique_ptr<Task>> tasks;
    tasks.push_back(std::move(source));
    tasks.push_back(std::make_unique<Add>(7));

    std::cout << std::boolalpha << "source empty: " << (source == nullptr) << '\n';
    std::cout << "results:";
    for (const auto& task : tasks) {
        std::cout << ' ' << task->run();
    }
    std::cout << '\n';

    const auto count = tasks.size();
    tasks.clear();
    std::cout << "tasks destroyed: " << count << '\n';
}

