#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

int main() {
    std::queue<int> samples;
    std::mutex mutex;
    std::condition_variable ready;
    bool done = false;

    std::thread consumer([&] {
        while (true) {
            std::unique_lock<std::mutex> lock(mutex);
            ready.wait(lock, [&] { return !samples.empty() || done; });

            if (samples.empty() && done) {
                break;
            }

            const int value = samples.front();
            samples.pop();
            lock.unlock();
            std::cout << "processed " << value << "\n";
        }
    });

    for (int value : {10, 20, 30}) {
        {
            std::lock_guard<std::mutex> lock(mutex);
            samples.push(value);
        }
        ready.notify_one();
    }

    {
        std::lock_guard<std::mutex> lock(mutex);
        done = true;
    }
    ready.notify_one();
    consumer.join();
}
