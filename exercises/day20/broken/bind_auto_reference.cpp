// INTENTIONALLY INVALID: a non-const lvalue reference cannot bind to a temporary.

int main() {
    auto& reference = 42;
    return reference;
}
