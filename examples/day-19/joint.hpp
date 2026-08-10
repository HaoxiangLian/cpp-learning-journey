#ifndef DAY19_JOINT_HPP
#define DAY19_JOINT_HPP

#include <string>

class Joint {
public:
    explicit Joint(const std::string& name);
    Joint(const std::string& name, double lower_limit, double upper_limit);

    Joint& set_position(double position);

    const std::string& name() const;
    double position() const;
    double lower_limit() const;
    double upper_limit() const;

private:
    std::string name_;
    double lower_limit_{-180.0};
    double upper_limit_{180.0};
    double position_{0.0};
};

#endif
