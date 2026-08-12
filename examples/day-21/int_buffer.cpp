#include "int_buffer.hpp"

#include <algorithm>
#include <cassert>
#include <utility>

IntBuffer::IntBuffer(std::size_t size)
    : size_{size}, data_{size == 0 ? nullptr : new int[size]{}} {}

IntBuffer::~IntBuffer() {
    delete[] data_;
}

IntBuffer::IntBuffer(const IntBuffer& other)
    : size_{other.size_},
      data_{other.size_ == 0 ? nullptr : new int[other.size_]} {
    if (size_ != 0) {
        std::copy(other.data_, other.data_ + other.size_, data_);
    }
}

IntBuffer& IntBuffer::operator=(const IntBuffer& other) {
    if (this == &other) {
        return *this;
    }

    IntBuffer temporary{other};
    swap(temporary);
    return *this;
}

std::size_t IntBuffer::size() const {
    return size_;
}

int& IntBuffer::operator[](std::size_t index) {
    assert(index < size_);
    return data_[index];
}

const int& IntBuffer::operator[](std::size_t index) const {
    assert(index < size_);
    return data_[index];
}

void IntBuffer::swap(IntBuffer& other) noexcept {
    using std::swap;
    swap(size_, other.size_);
    swap(data_, other.data_);
}
