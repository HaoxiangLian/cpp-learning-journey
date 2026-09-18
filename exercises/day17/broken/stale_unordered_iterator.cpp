// INTENTIONALLY INVALID: using an iterator invalidated by rehash is UB.
#include <unordered_map>

int main() {
    std::unordered_map<int, int> values{{7, 42}};
    auto old = values.find(7);
    values.rehash(values.bucket_count() + 1);
    return old->second;
}
