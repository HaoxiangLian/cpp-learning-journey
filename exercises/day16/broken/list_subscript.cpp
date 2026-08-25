// INTENTIONALLY INVALID: std::list does not provide random-access subscripting.
#include <list>

int main() {
    std::list<int> values{4, 9, 2};
    return values[1];
}
