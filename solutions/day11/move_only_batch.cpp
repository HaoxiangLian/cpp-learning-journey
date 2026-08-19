#include <cstddef>
#include <iostream>
#include <utility>

class MoveOnlyBatch {
public:
    explicit MoveOnlyBatch(std::size_t count)
        : count_{count}, data_{count_ > 0 ? new int[count_]{} : nullptr} {}

    ~MoveOnlyBatch() {
        delete[] data_;
    }

    MoveOnlyBatch(const MoveOnlyBatch&) = delete;
    MoveOnlyBatch& operator=(const MoveOnlyBatch&) = delete;

    MoveOnlyBatch(MoveOnlyBatch&& source)
        : count_{source.count_}, data_{source.data_} {
        source.count_ = 0;
        source.data_ = nullptr;
    }

    MoveOnlyBatch& operator=(MoveOnlyBatch&& source) {
        if (this == &source) {
            return *this;
        }

        delete[] data_;
        count_ = source.count_;
        data_ = source.data_;
        source.count_ = 0;
        source.data_ = nullptr;
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

    bool empty() const {
        return count_ == 0;
    }

private:
    std::size_t count_{};
    int* data_{};
};

int main() {
    MoveOnlyBatch source{2};
    source.set(0, 4);
    source.set(1, 9);

    MoveOnlyBatch moved{std::move(source)};
    MoveOnlyBatch assigned{1};
    assigned = std::move(moved);

    std::cout << "moved: " << assigned.value(0) << ' '
              << assigned.value(1) << '\n';
    std::cout << std::boolalpha
              << "sources empty: " << source.empty() << ' '
              << moved.empty() << '\n';
}
