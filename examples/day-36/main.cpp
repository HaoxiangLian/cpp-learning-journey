#include <algorithm>
#include <cmath>
#include <iostream>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>
#include <utility>

struct Sample {
    std::string joint;
    double position;
    double temperature;
};

std::optional<Sample> validate(Sample sample) {
    if (!std::isfinite(sample.position) ||
        sample.temperature < -40.0 ||
        sample.temperature > 125.0) {
        return std::nullopt;
    }
    return sample;
}

int main() {
    const std::vector<Sample> input{
        {"shoulder", 1.2, 45.0},
        {"elbow", -0.4, 130.0},
        {"shoulder", 1.4, 47.0},
    };

    std::vector<Sample> valid;
    std::unordered_map<std::string, int> counts;

    for (const Sample& sample : input) {
        if (auto checked = validate(sample)) {
            ++counts[checked->joint];
            valid.push_back(std::move(*checked));
        } else {
            std::cout << "rejected " << sample.joint << "\n";
        }
    }

    std::sort(valid.begin(), valid.end(),
              [](const Sample& left, const Sample& right) {
                  return left.position < right.position;
              });

    std::cout << "valid = " << valid.size() << "\n";
    std::cout << "shoulder count = " << counts.at("shoulder") << "\n";
    std::cout << "positions:";
    for (const Sample& sample : valid) {
        std::cout << ' ' << sample.position;
    }
    std::cout << "\n";
}
