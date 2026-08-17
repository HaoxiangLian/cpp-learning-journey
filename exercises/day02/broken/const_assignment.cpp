// INTENTIONALLY INVALID: compile-error exercise. Excluded from CMake targets.
int main() {
    const int sensor_id{12};
    sensor_id = 13;
    return sensor_id;
}
