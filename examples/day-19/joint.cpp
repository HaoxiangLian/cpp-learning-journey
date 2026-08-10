#include "joint.hpp"

#include <cassert>

Joint::Joint(const std::string& name)
    : Joint{name, -180.0, 180.0} {}

Joint::Joint(const std::string& name, double lower_limit, double upper_limit)
    : name_{name},
      lower_limit_{lower_limit},
      upper_limit_{upper_limit},
      position_{lower_limit} {
    assert(lower_limit_ <= upper_limit_);
}

Joint& Joint::set_position(double position) {
    assert(position >= lower_limit_ && position <= upper_limit_);
    this->position_ = position;
    return *this;
}

const std::string& Joint::name() const {
    return name_;
}

double Joint::position() const {
    return position_;
}

double Joint::lower_limit() const {
    return lower_limit_;
}

double Joint::upper_limit() const {
    return upper_limit_;
}
