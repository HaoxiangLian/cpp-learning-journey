// INTENTIONALLY INVALID: do not run; new[] paired with delete is undefined behavior.

int main() {
    int* values{new int[3]{1, 2, 3}};
    delete values;
}
