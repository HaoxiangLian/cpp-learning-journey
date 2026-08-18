#include <algorithm>
#include <cstddef>
#include <iostream>

class OwnedSamples {
public:
    explicit OwnedSamples(std::size_t count)
        : count_{count}, data_{count_ > 0 ? new int[count_]{} : nullptr} {}

    ~OwnedSamples() {
        delete[] data_;
    }

    OwnedSamples(const OwnedSamples& other)
        : count_{other.count_},
          data_{count_ > 0 ? new int[count_]{} : nullptr} {
        if (count_ > 0) {
            std::copy(other.data_, other.data_ + count_, data_);
        }
    }

    OwnedSamples& operator=(const OwnedSamples& other) {
        if (this == &other) {
            return *this;
        }

        int* replacement{
            other.count_ > 0 ? new int[other.count_]{} : nullptr
        };
        if (other.count_ > 0) {
            std::copy(other.data_, other.data_ + other.count_, replacement);
        }

        delete[] data_;
        data_ = replacement;
        count_ = other.count_;
        return *this;
    }

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
    OwnedSamples original{2};
    original.set(0, 5);
    original.set(1, 10);

    OwnedSamples copied{original};
    copied.set(0, 50);

    OwnedSamples assigned{1};
    assigned = original;
    assigned.set(1, 100);
    assigned = assigned;

    std::cout << "original: " << original.value(0) << ' ' << original.value(1) << '\n';
    std::cout << "copied: " << copied.value(0) << ' ' << copied.value(1) << '\n';
    std::cout << "assigned: " << assigned.value(0) << ' ' << assigned.value(1) << '\n';
}
