#include <algorithm>
#include <cstddef>
#include <iostream>

class SampleBuffer {
public:
    explicit SampleBuffer(std::size_t size)
        : size_{size}, data_{size_ > 0 ? new int[size_]{} : nullptr} {}

    ~SampleBuffer() {
        delete[] data_;
    }

    SampleBuffer(const SampleBuffer& other)
        : size_{other.size_},
          data_{size_ > 0 ? new int[size_]{} : nullptr} {
        if (size_ > 0) {
            std::copy(other.data_, other.data_ + size_, data_);
        }
    }

    SampleBuffer& operator=(const SampleBuffer& other) {
        if (this == &other) {
            return *this;
        }

        int* replacement{
            other.size_ > 0 ? new int[other.size_]{} : nullptr
        };
        if (other.size_ > 0) {
            std::copy(other.data_, other.data_ + other.size_, replacement);
        }

        delete[] data_;
        data_ = replacement;
        size_ = other.size_;
        return *this;
    }

    bool set(std::size_t index, int value) {
        if (index >= size_) {
            return false;
        }
        data_[index] = value;
        return true;
    }

    int value(std::size_t index) const {
        return index < size_ ? data_[index] : 0;
    }

private:
    std::size_t size_;
    int* data_;
};

int main() {
    SampleBuffer original{3};
    original.set(0, 10);
    original.set(1, 20);

    SampleBuffer copied{original};
    copied.set(0, 99);

    SampleBuffer assigned{1};
    assigned = original;
    assigned.set(1, 77);
    assigned = assigned;

    std::cout << "original[0]: " << original.value(0) << '\n';
    std::cout << "copied[0]: " << copied.value(0) << '\n';
    std::cout << "original[1]: " << original.value(1) << '\n';
    std::cout << "assigned[1]: " << assigned.value(1) << '\n';
}
