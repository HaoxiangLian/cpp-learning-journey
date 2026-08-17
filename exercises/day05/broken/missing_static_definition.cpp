// INTENTIONALLY INVALID: expected to compile, then fail during linking.

struct Counter {
    static int value;
};

int main() {
    return Counter::value;
}
