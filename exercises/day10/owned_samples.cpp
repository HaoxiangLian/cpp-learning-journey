#include <cstddef>
#include <iostream>

class OwnedSamples {
public:
    explicit OwnedSamples(std::size_t count)
        : count_{count}, data_{count_ > 0 ? new int[count_]{} : nullptr} {}

    ~OwnedSamples() {
        delete[] data_;
    }

    // TODO: 用深拷贝构造和拷贝赋值替换这两个已删除声明。
    OwnedSamples(const OwnedSamples&) = delete;
    OwnedSamples& operator=(const OwnedSamples&) = delete;

    bool set(std::size_t index, int value) {
        if (index >= count_) {
            return false;
        }
        data_[index] = value;
        return true;
    }

    int value(std::size_t index) const {
        return index < count_ ? data_[index] : 0;
    }

private:
    std::size_t count_;
    int* data_;
};

int main() {
    OwnedSamples samples{2};
    samples.set(0, 5);
    std::cout << "starter value: " << samples.value(0) << '\n';
    std::cout << "copy operations are disabled until implemented\n";
}
