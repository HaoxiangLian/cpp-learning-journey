// INTENTIONALLY INVALID: compiles on many toolchains but must never be run.

#include <cstddef>

class BrokenBuffer {
public:
    explicit BrokenBuffer(std::size_t count)
        : data_{new int[count]{}} {}

    ~BrokenBuffer() {
        delete[] data_;
    }

private:
    int* data_;
};

int main() {
    BrokenBuffer first{3};
    BrokenBuffer second{first};  // shallow pointer copy; both destructors call delete[]
}
