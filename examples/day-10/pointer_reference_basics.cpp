#include <iostream>

void increment_by_pointer(int* value) {
    if (value == nullptr) {
        return;
    }
    ++(*value);
}

void increment_by_reference(int& value) {
    ++value;
}

void set_first(int values[], int new_value) {
    values[0] = new_value;
}

void print_values(const int values[], int count) {
    for (int index{}; index < count; ++index) {
        std::cout << values[index];
        if (index + 1 < count) {
            std::cout << ' ';
        }
    }
    std::cout << '\n';
}

int main() {
    int joint_id{10};
    int* joint_pointer{&joint_id};
    int& joint_reference{joint_id};

    std::cout << "initial: " << joint_id << '\n';
    std::cout << "pointer and object address match: ";
    if (joint_pointer == &joint_id) {
        std::cout << "yes\n";
    } else {
        std::cout << "no\n";
    }

    *joint_pointer = 20;
    std::cout << "after pointer write: " << joint_id << '\n';

    joint_reference = 30;
    std::cout << "after reference write: " << joint_id << '\n';

    increment_by_pointer(joint_pointer);
    std::cout << "after pointer increment: " << joint_id << '\n';

    increment_by_reference(joint_reference);
    std::cout << "after reference increment: " << joint_id << '\n';

    increment_by_pointer(nullptr);
    std::cout << "null call leaves value: " << joint_id << '\n';

    int samples[3]{4, 8, 12};
    int* first{samples};
    std::cout << "array values through pointer: "
              << *first << ' '
              << *(first + 1) << ' '
              << *(first + 2) << '\n';

    set_first(samples, 40);
    std::cout << "after array-parameter update: ";
    print_values(samples, 3);
    return 0;
}
