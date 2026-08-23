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
    std::cout << "max count: " << larger(4, 9) << '\n';

    Reading<double> temperature{23.5, "C"};
    std::cout << "temperature: " << temperature.value() << ' '
              << temperature.unit() << '\n';
    std::cout << "bool label: " << TypeName<bool>::get() << '\n';
}
