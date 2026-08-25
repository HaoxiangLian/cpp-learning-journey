#include <cstddef>
#include <deque>
#include <iostream>
#include <iterator>
#include <list>
#include <string>
#include <vector>

int main() {
    std::vector<int> readings{4, 9, 2};
    readings.push_back(7);

    std::cout << "vector:";
    for (const int value : readings) {
        std::cout << ' ' << value;
    }
    std::cout << '\n';

    const std::size_t size_before = readings.size();
    const std::size_t requested_capacity = readings.capacity() + 2;
    readings.reserve(requested_capacity);
    std::cout << std::boolalpha
              << "reserve keeps size: "
              << (readings.size() == size_before) << '\n'
              << "capacity enough: "
              << (readings.capacity() >= requested_capacity) << '\n';

    readings.resize(5, 0);
    std::cout << "resized size: " << readings.size() << '\n'
              << "resized last: " << readings.back() << '\n';

    const std::size_t remembered_index = 1;
    readings.reserve(readings.capacity() + 1);
    const auto refreshed = readings.begin() + remembered_index;
    std::cout << "refreshed value: " << *refreshed << '\n';

    std::deque<std::string> jobs{"normal"};
    jobs.push_front("urgent");
    jobs.push_back("later");
    std::cout << "deque:";
    for (const auto& job : jobs) {
        std::cout << ' ' << job;
    }
    std::cout << '\n';

    std::list<int> checkpoints{10, 20, 30};
    auto kept = checkpoints.begin();
    auto middle = kept;
    ++middle;
    checkpoints.erase(middle);

    std::cout << "list after erase:";
    for (const int checkpoint : checkpoints) {
        std::cout << ' ' << checkpoint;
    }
    std::cout << '\n'
              << "kept list value: " << *kept << '\n';
}
