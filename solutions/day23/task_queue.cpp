#include <iostream>
#include <memory>
#include <utility>
#include <vector>

class Task {
public:
    virtual ~Task() = default;
    virtual int run() const = 0;
};

class ValueTask final : public Task {
public:
    explicit ValueTask(int value) : value_{value} {}
    int run() const override { return value_; }

private:
    int value_;
};

class TaskQueue {
public:
    void push(std::unique_ptr<Task> task) {
        tasks_.push_back(std::move(task));
    }

    std::vector<int> run_all() const {
        std::vector<int> results;
        results.reserve(tasks_.size());
        for (const auto& task : tasks_) {
            results.push_back(task->run());
        }
        return results;
    }

private:
    std::vector<std::unique_ptr<Task>> tasks_;
};

int main() {
    TaskQueue queue;
    queue.push(std::make_unique<ValueTask>(4));
    queue.push(std::make_unique<ValueTask>(7));

    std::cout << "results:";
    for (const int result : queue.run_all()) {
        std::cout << ' ' << result;
    }
    std::cout << '\n';
}

