#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

std::string format_temperature(double celsius) {
    std::ostringstream output;
    output << std::fixed << std::setprecision(1)
           << "temperature: " << celsius << " C";
    return output.str();
}

int main() {
    std::cout << format_temperature(36.5) << '\n';
}
