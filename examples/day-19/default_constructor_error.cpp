class Sensor {
public:
    explicit Sensor(int channel)
        : channel_{channel} {}

private:
    int channel_;
};

int main() {
    Sensor sensor;
}
