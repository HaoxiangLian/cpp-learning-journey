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

    // TODO 1：实现移动构造。接管资源后，让 source 变为空批次。

    // TODO 2：实现移动赋值。先处理目标旧资源，再接管 source。

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

    // TODO 3：移动构造 moved，再移动赋值给 assigned，并输出结果。
    std::cout << "完成 TODO 后应输出 moved: 4 9\n";
}
