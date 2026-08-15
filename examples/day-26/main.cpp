#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <utility>

class Device {
public:
    virtual ~Device() = default;
    virtual std::string name() const = 0;
    virtual void update() = 0;
};

class Motor final : public Device {
public:
    explicit Motor(std::string name) : name_{std::move(name)} {}
    std::string name() const override { return name_; }
    void update() override { ++cycles_; }
private:
    std::string name_;
    int cycles_{};
};

class DeviceManager {
public:
    void add(std::unique_ptr<Device> device) {
        devices_.push_back(std::move(device));
    }
    void update_all() {
        for (auto& device : devices_) device->update();
    }
private:
    std::vector<std::unique_ptr<Device>> devices_;
};

int main() {
    DeviceManager manager;
    manager.add(std::make_unique<Motor>("joint-1"));
    manager.update_all();
    std::cout << "updated\n";
}
