#include "force_model.hpp"

#include <iostream>

int main() {
    const double strain{1.5};
    std::cout << "force: " << compute_force(strain) << '\n';
}
