// INTENTIONALLY INVALID: compile-error exercise. Excluded from CMake targets.
int main() {
    const int mode{1};

    switch (mode) {
    case 1:
        break;
    case 1:
        break;
    default:
        break;
    }
}
