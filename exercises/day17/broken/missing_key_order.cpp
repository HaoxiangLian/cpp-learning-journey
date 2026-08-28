// INTENTIONALLY INVALID: SensorKey has no ordering relation for std::map.
#include <map>

struct SensorKey {
    int id;
};

int main() {
    std::map<SensorKey, int> readings;
    readings[{7}] = 42;
    return readings.begin()->second;
}
