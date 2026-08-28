#include <cstddef>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <unordered_map>

struct LastDigitHash {
    std::size_t operator()(int key) const {
        return static_cast<std::size_t>(key % 10);
    }
};

int main() {
    std::map<int, std::string> ordered_status{
        {20, "idle"},
        {7, "ready"},
        {12, "busy"},
    };
    std::cout << "ordered ids:";
    for (const auto& entry : ordered_status) {
        std::cout << ' ' << entry.first;
    }
    std::cout << '\n';

    std::set<int> alarm_ids{7, 7, 12};
    std::cout << "unique alarms: " << alarm_ids.size() << '\n';

    std::unordered_map<int, std::string, LastDigitHash> labels{
        {12, "force"},
        {22, "vision"},
    };
    std::cout << std::boolalpha
              << "same bucket: " << (labels.bucket(12) == labels.bucket(22)) << '\n'
              << "lookup 22: " << labels.find(22)->second << '\n';

    auto old = labels.find(12);
    const std::string* kept = &old->second;
    labels.rehash(labels.bucket_count() + 1);

    // old 已失效；重新散列不使指向现存元素的指针 kept 失效。
    const auto refreshed = labels.find(12);
    std::cout << "pointer after rehash: " << *kept << '\n'
              << "refreshed key: " << refreshed->first << '\n';
}
