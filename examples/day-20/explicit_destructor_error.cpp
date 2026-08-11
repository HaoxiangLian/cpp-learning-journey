#include <cstddef>

class Buffer {
public:
    explicit Buffer(std::size_t size)
        : data_{new int[size]{}} {}

    ~Buffer() {
        delete[] data_;
    }

private:
    int* data_;
};

int main() {
    Buffer buffer{4};
    buffer.~Buffer();
}
