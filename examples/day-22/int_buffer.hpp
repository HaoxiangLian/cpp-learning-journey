#ifndef DAY22_INT_BUFFER_HPP
#define DAY22_INT_BUFFER_HPP

#include <cstddef>

class IntBuffer {
public:
    explicit IntBuffer(std::size_t size);
    ~IntBuffer();

    IntBuffer(const IntBuffer& other);
    IntBuffer& operator=(const IntBuffer& other);
    IntBuffer(IntBuffer&& other) noexcept;
    IntBuffer& operator=(IntBuffer&& other) noexcept;

    std::size_t size() const;
    int& operator[](std::size_t index);
    const int& operator[](std::size_t index) const;

    void swap(IntBuffer& other) noexcept;

private:
    std::size_t size_{0};
    int* data_{nullptr};
};

#endif
