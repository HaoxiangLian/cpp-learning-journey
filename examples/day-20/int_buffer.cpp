#include "int_buffer.hpp"

#include <cassert>

IntBuffer::IntBuffer(std::size_t size)
    : data_{new int[size]{}}, size_{size} {}

IntBuffer::~IntBuffer() {
    delete[] data_;
}

std::size_t IntBuffer::size() const {
    return size_;
}

int& IntBuffer::at(std::size_t index) {
    assert(index < size_);
    return data_[index];
}

const int& IntBuffer::at(std::size_t index) const {
    assert(index < size_);
    return data_[index];
}
