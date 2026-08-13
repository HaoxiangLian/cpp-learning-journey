#include "int_buffer.hpp"

#include <iostream>
#include <utility>
#include <vector>

IntBuffer make_buffer() {
    IntBuffer result{3};
    result[0] = 10;
    result[1] = 20;
    result[2] = 30;
    return result;
}

int main() {
    IntBuffer source{make_buffer()};
    IntBuffer destination{std::move(source)};

    IntBuffer assigned{1};
    assigned = std::move(destination);

    std::cout << "source size: " << source.size() << '\n';
    std::cout << "destination size: " << destination.size() << '\n';
    std::cout << "assigned: " << assigned[0] << ", " << assigned[2] << '\n';

    source = IntBuffer{2};
    std::cout << "reused source size: " << source.size() << '\n';

    std::vector<IntBuffer> buffers;
    buffers.reserve(1);
    buffers.push_back(IntBuffer{1});
    buffers.push_back(IntBuffer{2});
    std::cout << "vector size: " << buffers.size() << '\n';
}
