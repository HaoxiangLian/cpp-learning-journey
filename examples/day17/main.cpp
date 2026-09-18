#include <cstddef>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <unordered_map>

struct TensGroupLess {
    bool operator()(int lhs, int rhs) const {
        return lhs / 10 < rhs / 10;
    }
};

struct LastDigitHash {
    std::size_t operator()(int key) const {
        return static_cast<std::size_t>(key % 10);
    }
};

int main() {
    std::map<std::string, int> limits{{"force", 8}, {"vision", 5}};
    std::set<int> active_ids{9, 2, 9};

    const auto force = limits.find("force");
    std::cout << "map lookup: " << force->second << '\n'
              << "set size: " << active_ids.size() << '\n'
              << "ordered ids:";
    for (const int id : active_ids) {
        std::cout << ' ' << id;
    }
    std::cout << '\n';

    const std::size_t size_before = limits.size();
    const int inserted_value = limits["temperature"];
    std::cout << std::boolalpha
              << "subscript inserted: " << (limits.size() == size_before + 1) << '\n'
              << "inserted value: " << inserted_value << '\n';

    std::set<int, TensGroupLess> grouped_ids;
    grouped_ids.insert(12);
    grouped_ids.insert(19);
    grouped_ids.insert(25);
    std::cout << "comparator-equivalent size: " << grouped_ids.size() << '\n';

    std::unordered_map<int, std::string, LastDigitHash> labels{
        {12, "force"},
        {22, "vision"},
    };
    std::cout << "same bucket: " << (labels.bucket(12) == labels.bucket(22)) << '\n'
              << "collision keeps both: " << labels.find(12)->second << ' '
              << labels.find(22)->second << '\n';

    auto old = labels.find(12);
    const std::string* kept = &old->second;
    const std::size_t requested_buckets = labels.bucket_count() + 1;
    labels.rehash(requested_buckets);

    // old 已失效，不再读取、比较或移动；指向现存元素的 kept 仍有效。
    std::cout << "pointer after rehash: " << *kept << '\n'
              << "bucket count enough: "
              << (labels.bucket_count() >= requested_buckets) << '\n';
}
