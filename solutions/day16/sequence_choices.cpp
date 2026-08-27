#include <cstddef>
#include <deque>
#include <iostream>
#include <list>
#include <string>
#include <vector>

int main() {
    std::vector<int> readings{4, 9, 2};
    readings.reserve(6);
    readings.push_back(7);

    std::cout << "readings:";
    for (const int value : readings) {
        std::cout << ' ' << value;
    }
    std::cout << '\n';

    std::deque<std::string> jobs{"normal"};
    jobs.push_front("urgent");
    jobs.push_back("later");
    std::cout << "jobs:";
    for (const auto& job : jobs) {
        std::cout << ' ' << job;
    }
    std::cout << '\n';

    std::list<int> checkpoints{10, 20, 30};
    auto first = checkpoints.begin();
    auto middle = first;
    ++middle;
    checkpoints.erase(middle);

    std::cout << "checkpoints:";
    for (const int checkpoint : checkpoints) {
        std::cout << ' ' << checkpoint;
    }
    std::cout << '\n'
              << "kept first: " << *first << '\n';

    const std::size_t remembered_index = 1;
    readings.reserve(readings.capacity() + 1);
    const auto refreshed = readings.begin() + remembered_index;
    std::cout << "refreshed reading: " << *refreshed << '\n';
}
