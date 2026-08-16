enum class MotionState {
    idle,
    moving,
    fault
};

int main() {
    int code{MotionState::fault};
    return code;
}

