// INTENTIONALLY INVALID: std::mutex is not copy-constructible.

#include <mutex>

int main() {
    std::mutex first;
    std::mutex second{first};
    second.lock();
    second.unlock();
}
