// INTENTIONALLY INVALID: Label does not support the operation required by larger.
struct Label {
    int id;
};

template <typename T>
T larger(T left, T right) {
    return left < right ? right : left;
}

int main() {
    const auto value = larger(Label{1}, Label{2});
    (void)value;
}
