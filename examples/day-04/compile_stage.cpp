int main() {
    int target_position{10};
    int current_position{4};
    int error{target_position - current_position};
    int command{error * 2};
    return command;
}
