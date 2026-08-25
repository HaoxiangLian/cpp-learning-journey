// INTENTIONALLY INVALID: a standard C++17 array cannot have a negative bound.

int main() {
    int values[-1]{};
    return values[0];
}
