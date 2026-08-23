#include <iostream>
#include <string>
#include <type_traits>
#include <utility>

template <typename T>
T larger(T left, T right) {
    return left < right ? right : left;
}

template <typename T>
class Reading {
    static_assert(std::is_arithmetic_v<T>,
                  "Reading<T> requires an arithmetic type");

public:
    Reading(T value, std::string unit)
        : value_{value}, unit_{std::move(unit)} {}

    T value() const {
        return value_;
    }

    const std::string& unit() const {
        return unit_;
    }

private:
    T value_{};
    std::string unit_;
};

template <typename T>
struct TypeName {
    static const char* get() {
        return "other";
    }
};

template <>
struct TypeName<bool> {
    static const char* get() {
        return "boolean";
    }
};

int main() {
    std::cout << "larger int: " << larger(4, 9) << '\n';
    std::cout << "larger double: " << larger(3.5, 2.0) << '\n';

    Reading<int> count{9, "items"};
    Reading<double> temperature{23.5, "C"};

    std::cout << "count reading: " << count.value() << ' '
              << count.unit() << '\n';
    std::cout << "temperature reading: " << temperature.value() << ' '
              << temperature.unit() << '\n';
    std::cout << "type int: " << TypeName<int>::get() << '\n';
    std::cout << "type bool: " << TypeName<bool>::get() << '\n';
}
