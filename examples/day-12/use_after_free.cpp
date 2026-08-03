#include <iostream>

int main() {
    int* owner{new int{42}};
    int* alias{owner};

    delete owner;
    owner = nullptr;

    std::cout << *alias << '\n'; // Undefined behavior: alias is dangling.
}
