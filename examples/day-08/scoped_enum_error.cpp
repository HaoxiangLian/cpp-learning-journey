enum class Mode {
    idle,
    running
};

int main() {
    Mode mode{Mode::running};
    return mode == 1;
}
