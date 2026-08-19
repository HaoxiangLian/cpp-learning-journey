#include <cstddef>
#include <iostream>
#include <utility>

class MoveOnlyBuffer {
public:
    explicit MoveOnlyBuffer(std::size_t size)
        : size_{size}, data_{size_ > 0 ? new int[size_]{} : nullptr} {}

    ~MoveOnlyBuffer() {
        delete[] data_;
    }

    MoveOnlyBuffer(const MoveOnlyBuffer&) = delete;
    MoveOnlyBuffer& operator=(const MoveOnlyBuffer&) = delete;

    MoveOnlyBuffer(MoveOnlyBuffer&& other)
        : size_{other.size_}, data_{other.data_} {
        other.size_ = 0;
        other.data_ = nullptr;
    }

    MoveOnlyBuffer& operator=(MoveOnlyBuffer&& other) {
        if (this == &other) {
            return *this;
        }

        delete[] data_;
        size_ = other.size_;
        data_ = other.data_;
        other.size_ = 0;
        other.data_ = nullptr;
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

    bool empty() const {
        return size_ == 0;
    }

private:
    std::size_t size_{};
    int* data_{};
};

int main() {
    std::cout << std::boolalpha;

    MoveOnlyBuffer source{3};
    source.set(1, 20);
    MoveOnlyBuffer target{std::move(source)};

    std::cout << "target[1]: " << target.value(1) << '\n';
    std::cout << "source empty: " << source.empty() << '\n';

    MoveOnlyBuffer assigned{1};
    assigned = std::move(target);

    std::cout << "assigned[1]: " << assigned.value(1) << '\n';
    std::cout << "target empty: " << target.empty() << '\n';
}
