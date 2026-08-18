// INTENTIONALLY INVALID: this file must fail during compilation.

class JointAngle {
private:
    double degrees_{0.0};
};

int main() {
    JointAngle joint;
    joint.degrees_ = 500.0;  // error: private member
}
