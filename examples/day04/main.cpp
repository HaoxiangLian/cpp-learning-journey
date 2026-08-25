#include <iostream>

int next_count(int count) {
    ++count;
    return count;
}

double cap_reading(double reading, double upper = 80.0) {
    if (reading > upper) {
        return upper;
    }
    return reading;
}

void report(int value) {
    std::cout << "count: " << value << '\n';
}

void report(double value) {
    std::cout << "reading: " << value << '\n';
}

int sum_to(int n) {
    if (n <= 0) {
        return 0;
    }
    return n + sum_to(n - 1);
}

int main() {
    int original_count{3};
    int advanced_count{next_count(original_count)};

    std::cout << "original count: " << original_count << '\n';
    std::cout << "advanced count: " << advanced_count << '\n';
    std::cout << "capped: " << cap_reading(92.0) << '\n';
    report(4);
    report(4.5);
    std::cout << "sum to 4: " << sum_to(4) << '\n';
}
