#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

int main() {
    const std::string path = "build/day-31/samples.csv";

    {
        std::ofstream output(path);
        if (!output) {
            std::cerr << "cannot create file\n";
            return 1;
        }
        output << "joint,position\n";
        output << "shoulder,1.25\n";
        output << "elbow,-0.50\n";
    }

    std::ifstream input(path);
    if (!input) {
        std::cerr << "cannot open file\n";
        return 1;
    }

    std::string line;
    std::getline(input, line);  // skip header
    while (std::getline(input, line)) {
        std::istringstream row(line);
        std::string joint;
        std::string position_text;
        if (!std::getline(row, joint, ',') ||
            !std::getline(row, position_text)) {
            std::cerr << "bad row: " << line << "\n";
            continue;
        }
        std::cout << joint << " -> " << std::stod(position_text) << "\n";
    }
}
