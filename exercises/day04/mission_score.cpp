#include <iostream>

int advance_tasks(int tasks) {
    // TODO: 修改独立形参并返回新值。
    return tasks;
}

int mission_score(int completed_tasks, int points_per_task = 10) {
    // TODO: 返回任务数与每项分值的乘积。
    return completed_tasks + points_per_task;
}

void print_metric(int value) {
    // TODO: 按要求输出 int 重载的标签和值。
    std::cout << value << '\n';
}

void print_metric(double value) {
    // TODO: 按要求输出 double 重载的标签和值。
    std::cout << value << '\n';
}

int sum_to(int n) {
    if (n <= 0) {
        return 0;
    }
    // TODO: 让下一次调用向终止条件推进。
    return n;
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
