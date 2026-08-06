#include "robot_status.hpp"

#include <string>

std::string status_text(int joint_id, bool ready) {
    return "joint " + std::to_string(joint_id) +
           (ready ? ": ready" : ": not ready");
}
