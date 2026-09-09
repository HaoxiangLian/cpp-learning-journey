// INTENTIONALLY INVALID: this file must fail during compilation.
// It verifies that a queue owning unique_ptr elements cannot be copied.
#include <memory>
#include <vector>

struct Task {
    virtual ~Task() = default;
    virtual void run() const = 0;
};

struct SensorQueue {
    std::vector<std::unique_ptr<Task>> tasks;
};

int main() {
    SensorQueue first;
    SensorQueue second = first;
}
