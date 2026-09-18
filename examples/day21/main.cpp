#include <atomic>
#include <iostream>
#include <mutex>
#include <thread>

int main() {
    int prepared{0};
    std::thread prepare{[&prepared] { prepared = 21; }};
    prepare.join();
    std::cout << "after join: " << prepared << '\n';

    int total{0};
    std::mutex total_mutex;
    auto add = [&total, &total_mutex] {
        for (int i{0}; i < 1000; ++i) {
            std::lock_guard<std::mutex> lock{total_mutex};
            ++total;
        }
    };
    std::thread first{add};
    std::thread second{add};
    first.join();
    second.join();
    std::cout << "protected total: " << total << '\n';

    std::mutex left;
    std::mutex right;
    int transfers{0};
    auto transfer = [&] {
        std::scoped_lock lock{left, right};
        ++transfers;
    };
    std::thread third{transfer};
    std::thread fourth{transfer};
    third.join();
    fourth.join();
    std::cout << "safe transfers: " << transfers << '\n';

    int payload{0};
    int observed{0};
    std::atomic<bool> ready{false};
    std::thread producer{[&] {
        payload = 42;
        ready.store(true, std::memory_order_release);
    }};
    std::thread consumer{[&] {
        while (!ready.load(std::memory_order_acquire)) {
        }
        observed = payload;
    }};
    producer.join();
    consumer.join();
    std::cout << "observed payload: " << observed << '\n';
}
