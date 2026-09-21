#include <iostream>
#include <mutex>
#include <thread>

int main() {
    int count{0};
    std::mutex count_mutex;

    auto increment = [&count, &count_mutex] {
        for (int i{0}; i < 2000; ++i) {
            std::lock_guard<std::mutex> lock{count_mutex};
            ++count;
        }
    };

    std::thread first{increment};
    std::thread second{increment};
    first.join();
    second.join();

    std::cout << "count: " << count << '\n';
}
