#include <iostream>
#include <memory>

struct Job {
    explicit Job(int number) : number{number} {}
    int number;
};

std::unique_ptr<Job> transfer_job(std::unique_ptr<Job> job) {
    ++job->number;
    return job;
}

int observe_job(const std::weak_ptr<Job>& observer) {
    const auto owner = observer.lock();
    return owner ? owner->number : -1;
}

int main() {
    auto first = std::make_unique<Job>(11);
    auto second = transfer_job(std::move(first));
    std::cout << "moved number: " << second->number << '\n';

    auto owner = std::make_shared<Job>(20);
    std::weak_ptr<Job> observer = owner;
    std::cout << "observed number: " << observe_job(observer) << '\n';
    owner.reset();
    std::cout << "after reset: " << observe_job(observer) << '\n';
}
