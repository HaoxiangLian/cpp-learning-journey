#include "reading.hpp"

bool is_valid(const Reading& reading) {
    return reading.sensor_id > 0 && reading.value >= 0.0;
}
