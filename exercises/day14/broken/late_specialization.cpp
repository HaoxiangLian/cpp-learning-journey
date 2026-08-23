// INTENTIONALLY INVALID: the explicit specialization appears after instantiation.
template <typename T>
struct TypeCode {
    static constexpr int value = 0;
};

constexpr int before_specialization = TypeCode<bool>::value;

template <>
struct TypeCode<bool> {
    static constexpr int value = 1;
};

int main() {
    return before_specialization;
}
