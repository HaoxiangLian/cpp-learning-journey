#ifndef DAY20_INT_BUFFER_HPP
#define DAY20_INT_BUFFER_HPP

#include <cstddef>

class IntBuffer {
public:
    explicit IntBuffer(std::size_t size);
    ~IntBuffer();

    IntBuffer(const IntBuffer&) = delete;
    IntBuffer& operator=(const IntBuffer&) = delete;

    std::size_t size() const;
    int& at(std::size_t index);
    const int& at(std::size_t index) const;

private:
    int* data_;
    std::size_t size_;
};

#endif
