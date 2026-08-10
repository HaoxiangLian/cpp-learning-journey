class Motor {
public:
    explicit Motor(int speed)
        : speed_{speed} {}

private:
    int speed_;
};

int main() {
    Motor motor{100};
    motor.speed_ = 200;
}
