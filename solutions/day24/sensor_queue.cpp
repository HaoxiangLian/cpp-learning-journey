#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

struct Result {
    std::string name;
    int value;
};

class Task {
public:
    virtual ~Task() = default;
    virtual Result run() const = 0;
};

class AverageTask final : public Task {
public:
    AverageTask(std::string sensor, int first, int second)
        : sensor_{std::move(sensor)}, first_{first}, second_{second} {}

    Result run() const override {
        return {sensor_ + " average", (first_ + second_) / 2};
    }

private:
    std::string sensor_;
    int first_;
    int second_;
};

class ThresholdTask final : public Task {
public:
    ThresholdTask(std::string sensor, int reading, int limit)
        : sensor_{std::move(sensor)}, reading_{reading}, limit_{limit} {}

    Result run() const override {
        return {sensor_ + " alert", reading_ > limit_ ? 1 : 0};
    }

private:
    std::string sensor_;
    int reading_;
    int limit_;
};

class SensorQueue {
public:
    void push(std::unique_ptr<Task> task) {
        tasks_.push_back(std::move(task));
    }

    std::vector<Result> run_all() const {
        std::vector<Result> results;
        results.reserve(tasks_.size());
        for (const auto& task : tasks_) {
            results.push_back(task->run());
        }
        return results;
    }

    std::size_t size() const noexcept {
        return tasks_.size();
    }

private:
    std::vector<std::unique_ptr<Task>> tasks_;
};

int main() {
    SensorQueue queue;
    queue.push(std::make_unique<AverageTask>("room", 20, 24));
    queue.push(std::make_unique<ThresholdTask>("motor", 91, 80));

    for (const Result& result : queue.run_all()) {
        std::cout << result.name << ": " << result.value << '\n';
    }
    std::cout << "processed: " << queue.size() << '\n';
}
