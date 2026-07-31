#include <iostream>
#include <string>
#include <vector>

struct JointState {
    int id{};
    std::string name{};
    double angle_deg{};
};

bool update_angle_wrong(std::vector<JointState>& joints,
                        int id,
                        double angle_deg) {
    for (JointState joint : joints) {
        if (joint.id == id) {
            joint.angle_deg = angle_deg;
            return true;
        }
    }
    return false;
}

int main() {
    std::vector<JointState> joints{{1, "shoulder", 35.0}};

    const bool reported_success{update_angle_wrong(joints, 1, 80.0)};

    std::cout << "reported success: ";
    if (reported_success) {
        std::cout << "yes\n";
    } else {
        std::cout << "no\n";
    }
    std::cout << "stored angle: " << joints[0].angle_deg << '\n';
    return 0;
}
