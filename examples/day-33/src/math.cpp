#include "robot/math.hpp"

#include <stdexcept>

namespace robot {

double clamp_position(double value, double low, double high) {
    if (low > high) {
        throw std::invalid_argument("low must not exceed high");
    }
    if (value < low) {
        return low;
    }
    if (value > high) {
        return high;
    }
    return value;
}

}  // namespace robot
