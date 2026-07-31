#include <iostream>
#include <string>
#include <vector>

namespace robotics {

using JointId = int;

enum class Mode {
    offline,
    ready,
    running,
    fault
};

struct JointState {
    JointId id{};
    std::string name{};
    double angle_deg{};
    bool enabled{};
};

std::string mode_name(Mode mode) {
    switch (mode) {
        case Mode::offline:
            return "offline";
        case Mode::ready:
            return "ready";
        case Mode::running:
            return "running";
        case Mode::fault:
            return "fault";
    }
    return "unknown";
}

std::string result_text(bool succeeded) {
    if (succeeded) {
        return "accepted";
    }
    return "rejected";
}

void print_joint(const JointState& joint) {
    std::cout << "  [" << joint.id << "] " << joint.name
              << ": angle=" << joint.angle_deg
              << " deg, enabled=";

    if (joint.enabled) {
        std::cout << "yes\n";
    } else {
        std::cout << "no\n";
    }
}

class RobotMonitor {
public:
    void set_name(const std::string& name) {
        robot_name_ = name;
    }

    void set_mode(Mode mode) {
        mode_ = mode;
    }

    void add_joint(const JointState& joint) {
        joints_.push_back(joint);
    }

    bool update_angle(JointId id, double angle_deg) {
        if (angle_deg < -180.0 || angle_deg > 180.0) {
            return false;
        }

        for (JointState& joint : joints_) {
            if (joint.id == id) {
                joint.angle_deg = angle_deg;
                return true;
            }
        }
        return false;
    }

    int enabled_count() const {
        int count{};
        for (const JointState& joint : joints_) {
            if (joint.enabled) {
                ++count;
            }
        }
        return count;
    }

    void print_report() const {
        std::cout << "Robot: " << robot_name_ << '\n';
        std::cout << "Mode: " << mode_name(mode_) << '\n';
        std::cout << "Enabled joints: " << enabled_count()
                  << " / " << joints_.size() << '\n';

        for (const JointState& joint : joints_) {
            print_joint(joint);
        }
    }

private:
    std::string robot_name_{"unnamed"};
    Mode mode_{Mode::offline};
    std::vector<JointState> joints_{};
};

}  // namespace robotics

int main() {
    robotics::RobotMonitor monitor{};
    robotics::JointState base{1, "base", 0.0, true};
    robotics::JointState shoulder{2, "shoulder", 35.0, true};
    robotics::JointState tool{3, "tool", -10.0, false};

    monitor.set_name("Cardiac-Arm");
    monitor.add_joint(base);
    monitor.add_joint(shoulder);
    monitor.add_joint(tool);
    monitor.set_mode(robotics::Mode::running);

    const bool valid_update{monitor.update_angle(2, 47.5)};
    const bool missing_joint{monitor.update_angle(9, 10.0)};
    const bool invalid_angle{monitor.update_angle(1, 220.0)};

    std::cout << "Update joint 2: "
              << robotics::result_text(valid_update) << '\n';
    std::cout << "Update joint 9: "
              << robotics::result_text(missing_joint) << '\n';
    std::cout << "Update joint 1 to 220 deg: "
              << robotics::result_text(invalid_angle) << "\n\n";

    monitor.print_report();
    return 0;
}
