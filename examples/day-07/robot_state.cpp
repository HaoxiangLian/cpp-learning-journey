#include <iostream>

namespace robot {

using JointId = int;

enum class MotionState {
    idle,
    moving,
    fault
};

struct JointState {
    JointId id;
    double angle;
    MotionState motion;
};

void print_state(const JointState& joint) {
    std::cout << "joint " << joint.id << ", angle " << joint.angle;
    if (joint.motion == MotionState::fault) {
        std::cout << ", FAULT\n";
    } else if (joint.motion == MotionState::moving) {
        std::cout << ", MOVING\n";
    } else {
        std::cout << ", IDLE\n";
    }
}

}  // namespace robot

int main() {
    robot::JointState joint{2, 45.0, robot::MotionState::moving};
    robot::print_state(joint);
    return 0;
}

