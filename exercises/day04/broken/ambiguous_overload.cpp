// INTENTIONALLY INVALID: compile-error exercise. Excluded from CMake targets.
void calibrate(int, double) {
}

void calibrate(double, int) {
}

int main() {
    calibrate(1, 1);
}
