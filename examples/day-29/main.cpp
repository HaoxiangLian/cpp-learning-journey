#include <iostream>
#include <map>
#include <string>
#include <unordered_map>

int main() {
    std::map<std::string, double> force_limits{
        {"arm", 12.5},
        {"gripper", 6.0},
        {"wrist", 4.5},
    };

    const auto wrist = force_limits.find("wrist");
    if (wrist != force_limits.end()) {
        std::cout << "wrist limit = " << wrist->second << "\n";
    }

    std::unordered_map<std::string, int> error_counts;
    auto [it, inserted] = error_counts.try_emplace("arm", 0);
    ++it->second;

    std::cout << "arm errors = " << error_counts.at("arm") << "\n";
    std::cout << "ordered first key = " << force_limits.begin()->first << "\n";
}
