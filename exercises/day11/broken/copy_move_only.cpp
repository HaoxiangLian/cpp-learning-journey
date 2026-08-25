// INTENTIONALLY INVALID: this file must fail during compilation.
#include <cstddef>

class MoveOnlyBatch {
public:
    explicit MoveOnlyBatch(std::size_t count)
        : count_{count}, data_{count_ > 0 ? new int[count_]{} : nullptr} {}

    ~MoveOnlyBatch() {
        delete[] data_;
    }

    MoveOnlyBatch(const MoveOnlyBatch&) = delete;
    MoveOnlyBatch& operator=(const MoveOnlyBatch&) = delete;

private:
    std::size_t count_{};
    int* data_{};
};

int main() {
    MoveOnlyBatch source{3};
    MoveOnlyBatch copy{source};  // 编译错误：拷贝构造已删除
}
