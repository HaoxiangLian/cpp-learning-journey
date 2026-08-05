int main() {
    int values[3]{10, 20, 30};
    volatile int observed{values[3]};
    return observed;
}
