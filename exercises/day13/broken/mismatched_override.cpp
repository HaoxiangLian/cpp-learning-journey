// INTENTIONALLY INVALID: this file must fail during compilation.
class Sensor {
public:
    virtual int read() const {
        return 0;
    }
};

class Lidar : public Sensor {
public:
    int read() override {  // 编译错误：缺少与基类一致的 const
        return 64;
    }
};

int main() {
    Lidar lidar;
    return lidar.read();
}
