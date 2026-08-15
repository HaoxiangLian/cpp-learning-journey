#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>

std::optional<double> parse_force(const std::string& text) {
    try {
        std::size_t used = 0;
        const double value = std::stod(text, &used);
        if (used != text.size() || value < 0.0) {
            return std::nullopt;
        }
        return value;
    } catch (const std::invalid_argument&) {
        return std::nullopt;
    } catch (const std::out_of_range&) {
        return std::nullopt;
    }
}

int main() {
    for (const std::string text : {"12.5", "bad", "-2.0"}) {
        const auto force = parse_force(text);
        if (force) {
            std::cout << text << " -> " << *force << "\n";
        } else {
            std::cout << text << " -> invalid\n";
        }
    }
}
