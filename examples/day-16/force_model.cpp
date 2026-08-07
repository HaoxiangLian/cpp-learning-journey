#include "force_model.hpp"

namespace {
constexpr double gain{2.0};
constexpr double calibration_bias{0.25};
}

double compute_force(double strain) {
    return strain * gain + calibration_bias;
}
