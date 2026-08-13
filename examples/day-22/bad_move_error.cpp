#include <cstddef>
#include <utility>

class BadBuffer {
public:
    explicit BadBuffer(std::size_t size) : data_{new int[size]{}} {}
    ~BadBuffer() { delete[] data_; }

    BadBuffer(const BadBuffer&) = delete;
    BadBuffer& operator=(const BadBuffer&) = delete;

    BadBuffer(BadBuffer&& other) noexcept : data_{other.data_} {}

private:
    int* data_;
};

int main() {
    BadBuffer first{4};
    BadBuffer second{std::move(first)};
}
