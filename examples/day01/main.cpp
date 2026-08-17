#include "status.hpp"

#include <iostream>

int main() {
    std::cout << "sensor status: " << status_code() << '\n';
    return 0;
}
