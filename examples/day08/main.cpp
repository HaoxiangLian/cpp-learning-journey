#include <iostream>

class JointAngle {
public:
    bool set(double candidate) {
        if (candidate < -180.0 || candidate > 180.0) {
            return false;
        }
        this->degrees_ = candidate;
        return true;
    }

    double degrees() const {
        return this->degrees_;
    }

    bool within_limit() const {
        return degrees_ >= -180.0 && degrees_ <= 180.0;
    }

private:
    double degrees_{0.0};
};

int main() {
    JointAngle joint;
    const bool accepted{joint.set(45.0)};
    const bool rejected{!joint.set(500.0)};

    std::cout << std::boolalpha;
    std::cout << "accepted: " << accepted << '\n';
    std::cout << "rejected invalid: " << rejected << '\n';
    std::cout << "angle: " << joint.degrees() << '\n';
    std::cout << "invariant holds: " << joint.within_limit() << '\n';
}
