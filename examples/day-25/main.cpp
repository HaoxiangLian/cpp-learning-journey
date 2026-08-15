#include <iostream>

struct Vector2 {
    double x{};
    double y{};
};

Vector2 operator+(const Vector2& left, const Vector2& right) {
    return {left.x + right.x, left.y + right.y};
}

std::ostream& operator<<(std::ostream& out, const Vector2& value) {
    return out << '(' << value.x << ", " << value.y << ')';
}

int main() {
    Vector2 a{1.0, 2.0};
    Vector2 b{3.0, 4.0};
    std::cout << "sum = " << (a + b) << '\n';
}
