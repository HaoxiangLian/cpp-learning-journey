#include <iostream>
#include <string>

class Trace {
public:
    explicit Trace(const char* label) : label_{label} {
        std::cout << "construct " << label_ << '\n';
    }

    ~Trace() {
        std::cout << "destroy " << label_ << '\n';
    }

private:
    std::string label_;
};

class RobotTask {
public:
    explicit RobotTask(int id)
        : sensor_{"sensor"}, planner_{"planner"}, id_{id} {
        std::cout << "task " << id_ << " ready\n";
    }

    ~RobotTask() {
        std::cout << "task " << id_ << " done\n";
    }

    void run() const {
        std::cout << "task " << id_ << " running\n";
    }

private:
    Trace sensor_;
    Trace planner_;
    int id_;
};

int main() {
    RobotTask task{7};
    task.run();
}
