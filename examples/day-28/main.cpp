#include <iostream>
#include <vector>

int main() {
    std::vector<double> samples;
    samples.reserve(4);
    samples.push_back(1.0);
    samples.push_back(2.0);

    auto first = samples.begin();
    std::cout << *first << '\n';

    samples.push_back(3.0);
    std::cout << *first << '\n'; // reserve 足够时本例仍有效
}
