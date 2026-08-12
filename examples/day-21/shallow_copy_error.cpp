#include <cstddef>

class BadBuffer {
public:
    explicit BadBuffer(std::size_t size)
        : data_{new int[size]{}} {}

    ~BadBuffer() {
        delete[] data_;
    }

private:
    int* data_;
};

int main() {
    BadBuffer first{4};
    BadBuffer second{first};
}
