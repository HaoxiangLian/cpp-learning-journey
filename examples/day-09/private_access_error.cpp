#include <string>

enum class Mode {
    offline,
    running
};

class RobotMonitor {
public:
    void set_mode(Mode mode) {
        mode_ = mode;
    }

private:
    std::string name_{"unnamed"};
    Mode mode_{Mode::offline};
};

int main() {
    RobotMonitor monitor{};
    monitor.mode_ = Mode::running;
    return 0;
}
