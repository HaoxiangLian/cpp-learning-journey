# 第 17 天练习：关联容器、比较器、哈希与重新散列

[返回第 17 天讲义](../../days/day17.md) · [完成后查看答案](../../solutions/day17/README.md)

每题先写出容器中的键、映射值和 `size()`；涉及无序容器时，再标出哈希值、候选桶以及修改前后访问入口是否有效。不要只写“树更慢”或“哈希更快”。

## 一、概念判断题

判断正误并用一句话说明理由。

1. `std::set<Key>` 的元素就是键；`std::map<Key, T>` 的元素包含不可直接改写的键和对应的映射值。
2. `std::set<int> ids{7, 2, 7};` 的 `size()` 为 3，因为初始化列表中写了三个整数。
3. 对不存在的键执行 `map::operator[]` 只返回一个临时默认值，不会修改 `map`。
4. 有序关联容器中，比较器两个方向都返回 `false` 的两个键一定满足 `operator==`。
5. 两个不同键发生哈希冲突后，只能保留后插入的元素。
6. `unordered_map::find` 是平均常量复杂度，因此最坏情况也保证常量复杂度。

## 二、键状态、复杂度与失效分析

1. 逐句写出 `counts` 的键、对应值和 `size()`：

   ```cpp
   std::map<std::string, int> counts;
   auto missing = counts.find("force");
   int first = counts["force"];
   counts["force"] += 2;
   ```

   说明 `missing`、`first` 分别是什么，以及哪一句第一次修改容器。

2. 对下列比较器和容器，分别求 `comp(12, 19)`、`comp(19, 12)`、`comp(12, 25)`，再写出最终元素与 `size()`：

   ```cpp
   struct TensGroupLess {
       bool operator()(int lhs, int rhs) const {
           return lhs / 10 < rhs / 10;
       }
   };

   std::set<int, TensGroupLess> ids;
   ids.insert(12);
   ids.insert(19);
   ids.insert(25);
   ```

3. `LastDigitHash` 返回 `key % 10`。向 `unordered_map` 依次加入 `12 -> force` 和 `22 -> vision`：画出哈希值与桶关系，说明两个键是否等价、`size()` 是多少、`find(22)` 得到什么，以及为什么这次冲突不等于覆盖。
4. 阅读下列代码，指出 `rehash` 后 `iterator`、`reference` 和 `pointer` 分别能否使用；若随后执行 `labels.erase(12)`，结论又怎样变化：

   ```cpp
   std::unordered_map<int, std::string> labels{{12, "force"}};
   auto iterator = labels.find(12);
   std::string& reference = iterator->second;
   std::string* pointer = &iterator->second;

   labels.rehash(labels.bucket_count() + 1);
   ```

5. 为下列需求选择容器并说明顺序与复杂度依据：按时间戳有序扫描并按范围处理状态；只做设备 ID 去重和高频存在性查询且不需要顺序；保存唯一报警等级并按等级顺序输出。随后检查 [`broken/missing_key_order.cpp`](broken/missing_key_order.cpp) 与 [`broken/stale_unordered_iterator.cpp`](broken/stale_unordered_iterator.cpp)，分别判断错误类别和修复方向。

## 三、小型编程题：为三类键操作选择容器

打开 [`associative_choices.cpp`](associative_choices.cpp)，完成：

- 用 `map<int, string>` 保存 `20 -> idle`、`7 -> ready`、`12 -> busy`，按容器键序输出 ID；
- 向 `set<int>` 加入报警 ID `7、7、12`，输出唯一键数量；
- 用题目给出的 `LastDigitHash` 建立 `12 -> force`、`22 -> vision`，验证二者位于同桶但都能分别查到；
- 保存键 12 对应字符串的指针，再调用 `rehash(bucket_count() + 1)`；丢弃旧迭代器，通过指针输出字符串，并重新 `find(12)` 输出键；
- 不遍历输出整个 `unordered_map`，避免把未指定的迭代顺序写进稳定输出。

稳定输出应为：

```text
ordered ids: 7 12 20
unique alarms: 2
same bucket: true
lookup 22: vision
pointer after rehash: force
refreshed key: 12
```

建议使用 C++17 严格警告编译：

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror \
  exercises/day17/associative_choices.cpp -o /tmp/day17_exercise
/tmp/day17_exercise
```
