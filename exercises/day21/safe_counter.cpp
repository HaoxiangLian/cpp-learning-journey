#include <iostream>
#include <thread>

int main() {
    int count{0};

    auto increment = [&count] {
        for (int i{0}; i < 2000; ++i) {
            // TODO: 让两个线程对 count 的“读取、加一、写回”不能交错。
            ++count;
        }
    };

    std::thread first{increment};
    std::thread second{increment};
    first.join();
    second.join();

    std::cout << "count: " << count << '\n';
}
