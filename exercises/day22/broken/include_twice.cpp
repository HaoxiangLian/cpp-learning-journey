// INTENTIONALLY INVALID: including the unguarded definition twice causes redefinition.

#include "unguarded.hpp"
#include "unguarded.hpp"

int main() {
    Calibration calibration{1.0};
    return calibration.scale == 1.0 ? 0 : 1;
}
