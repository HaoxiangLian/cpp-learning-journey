#include <iostream>

int advance_tasks(int tasks) {
    ++tasks;
    return tasks;
}

int mission_score(int completed_tasks, int points_per_task = 10) {
    return completed_tasks * points_per_task;
}

void print_metric(int value) {
    std::cout << "metric(int): " << value << '\n';
}

void print_metric(double value) {
    std::cout << "metric(double): " << value << '\n';
}

int sum_to(int n) {
    if (n <= 0) {
        return 0;
    }
    return n + sum_to(n - 1);
}

int main() {
    int original_tasks{3};
    int advanced_tasks{advance_tasks(original_tasks)};
    int score{mission_score(original_tasks)};

    std::cout << "original tasks: " << original_tasks << '\n';
    std::cout << "advanced tasks: " << advanced_tasks << '\n';
    std::cout << "score: " << score << '\n';
    print_metric(score);
    print_metric(30.5);
    std::cout << "recursive sum: " << sum_to(3) << '\n';
}
