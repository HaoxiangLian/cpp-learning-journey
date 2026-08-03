int main() {
    int* values{new int[3]{1, 2, 3}};
    delete values; // Undefined behavior: must use delete[].
}
