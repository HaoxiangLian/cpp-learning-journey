int main() {
    int* owner{new int{42}};
    delete owner;
    delete owner; // Undefined behavior: the storage was already released.
}
