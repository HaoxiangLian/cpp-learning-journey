int main() {
    const int fixed{10};
    int& invalid_path{const_cast<int&>(fixed)};
    invalid_path = 20;
    return fixed;
}
