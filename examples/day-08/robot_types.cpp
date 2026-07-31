#include <iostream>
#include <string>
#include <vector>

namespace robotics {

using JointId = int;

enum class Mode {
    idle,
    calibrating,
    running,
    fault
};

struct JointState {
    JointId id{};
    double angle{};
    bool enabled{};
};

class Robot {
public:
    void set_name(const std::string& name) {
        name_ = name;
    }

    void set_mode(Mode mode) {
        mode_ = mode;
    }

    void add_joint(const JointState& joint) {
        joints_.push_back(joint);
    }

    const std::string& name() const {
        return name_;
    }

    Mode mode() const {
        return mode_;
    }

    const std::vector<JointState>& joints() const {
        return joints_;
    }

private:
    std::string name_{"Unnamed"};
    Mode mode_{Mode::idle};
    std::vector<JointState> joints_{};
};

std::string mode_name(Mode mode) {
    switch (mode) {
    case Mode::idle:
        return "idle";
    case Mode::calibrating:
        return "calibrating";
    case Mode::running:
        return "running";
    case Mode::fault:
        return "fault";
    }

    return "unknown";
}

void print_joint(const JointState& joint) {
    std::cout << "Joint " << joint.id
              << " angle=" << joint.angle
              << " enabled=" << std::boolalpha << joint.enabled << '\n';
}

} // namespace robotics

int main() {
    robotics::Robot robot{};
    robot.set_name("Arm-A");
    robot.set_mode(robotics::Mode::running);

    robotics::JointState shoulder{1, 15.5, true};
    robotics::JointState elbow{2, -20.0, false};

    robot.add_joint(shoulder);
    robot.add_joint(elbow);

    std::cout << "Robot: " << robot.name() << '\n';
    std::cout << "Mode: " << robotics::mode_name(robot.mode()) << '\n';
    std::cout << "Joint count: " << robot.joints().size() << '\n';

    for (const robotics::JointState& joint : robot.joints()) {
        robotics::print_joint(joint);
    }

    robotics::JointState copied{shoulder};
    copied.angle = 90.0;

    std::cout << "Copied angle: " << copied.angle << '\n';
    std::cout << "Original angle: " << shoulder.angle << '\n';

    return 0;
}
