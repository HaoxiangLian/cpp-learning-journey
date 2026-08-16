#include <iostream>

int main() {
    constexpr int counts_per_revolution{4096};
    constexpr double degrees_per_revolution{360.0};
    const int counts{1024};

    double wrong_turns{};
    wrong_turns = counts / counts_per_revolution;

    const double turns{
        static_cast<double>(counts) / counts_per_revolution};
    const double angle{turns * degrees_per_revolution};

    std::cout << "wrong turns: " << wrong_turns << '\n';
    std::cout << "turns: " << turns << '\n';
    std::cout << "angle: " << angle << '\n';
    return 0;
}

