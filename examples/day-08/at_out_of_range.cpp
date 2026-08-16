#include <iostream>
#include <vector>

int main() {
    const std::vector<double> angles{10.0, 20.0, 30.0};
    std::cout << angles.at(3) << '\n';
    return 0;
}

