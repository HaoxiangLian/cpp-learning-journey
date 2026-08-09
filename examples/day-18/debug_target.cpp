#include <iostream>
#include <vector>

int weighted_sum(const std::vector<int>& values, int weight) {
    int total{0};
    for (const int value : values) {
        total += value * weight;
    }
    return total;
}

int calculate_command() {
    const std::vector<int> joint_errors{2, 4, 7};
    return weighted_sum(joint_errors, 2);
}

int main() {
    const int command{calculate_command()};
    std::cout << "weighted sum: " << command << '\n';
}
