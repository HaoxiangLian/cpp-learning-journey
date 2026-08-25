# 第 16 天练习：顺序容器、容量与迭代器失效

[返回第 16 天讲义](../../days/day16.md) · [完成后查看答案](../../solutions/day16/README.md)

每题先写出操作前后的元素序列、`size()` 与 `capacity()` 关系，再判断旧迭代器或引用。不要只凭容器名字回答“快”或“稳定”。

## 一、概念判断题

判断正误并用一句话说明理由。

1. `std::vector<int>` 自己管理元素的生命周期和存储，普通业务代码不需要为它的元素手写 `delete[]`。
2. 空 `vector` 执行 `reserve(10)` 后，`size()` 为 10，可以直接写 `values[9]`。
3. `resize(5, 0)` 会把元素数量改为 5；若原来不足 5 个，会在末尾创建值为 0 的元素。
4. `vector::push_back` 是摊销常量时间，因此每一次调用都绝不会移动现有元素。
5. `deque` 支持随机访问，并保证在首尾插入、删除单个元素为常量时间。
6. `list` 在已知迭代器位置插入是常量时间，所以从头寻找第 1000 个位置再插入的整个过程也是常量时间。

## 二、元素状态、复杂度与失效分析

1. 分别写出下列两个对象创建后和各执行一次 `push_back(8)` 后的元素数量与元素序列：

   ```cpp
   std::vector<int> a(3);
   std::vector<int> b{3};
   ```

2. 从空 `vector<int>` 开始，依次执行 `reserve(6)`、`resize(3, 7)`、`push_back(9)`、`resize(2)`。逐步写出元素序列、`size()` 和 `capacity()` 能确定的最强结论，并指出哪一步仅处理空间、哪一步创建或移除元素。
3. 为三种需求选择 `vector`、`deque` 或 `list`，写出关键复杂度和放弃的能力：频繁按下标读取且末尾追加；频繁从首尾进出且仍需随机访问；已经长期持有中间位置并在其附近插删。
4. 阅读下列代码，指出 `reserve` 后哪些旧访问入口失效、元素值是否改变，以及如何安全重新取得值 9 的位置：

   ```cpp
   std::vector<int> values{4, 9, 2};
   auto iterator = values.begin() + 1;
   int& reference = values[1];
   int* pointer = &values[1];

   values.reserve(values.capacity() + 1);
   ```

5. 查看 [`broken/list_subscript.cpp`](broken/list_subscript.cpp) 与 [`broken/stale_vector_iterator.cpp`](broken/stale_vector_iterator.cpp)，分别判断错误发生阶段、错误类别、不能依赖的运行现象及修复方向。

## 三、小型编程题：按操作模式使用顺序容器

打开 [`sequence_choices.cpp`](sequence_choices.cpp)，完成：

- 为 `readings` 预留至少 6 个元素的容量，在末尾加入 `7` 并输出序列；
- 用 `deque` 把任务组织为 `urgent、normal、later` 并输出；
- 保存 `checkpoints` 第一个元素的迭代器，删除中间的 `20`，输出剩余序列和仍有效的第一个元素；
- 保存读数 9 的下标，调用 `reserve(readings.capacity() + 1)` 使 `vector` 必然重新分配；丢弃任何旧迭代器，从新 `begin()` 重新取得并输出 9；
- 在代码旁用注释说明三种容器分别匹配的主要操作。

稳定输出应为：

```text
readings: 4 9 2 7
jobs: urgent normal later
checkpoints: 10 30
kept first: 10
refreshed reading: 9
```

建议使用 C++17 严格警告编译：

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror \
  exercises/day16/sequence_choices.cpp -o /tmp/day16_exercise
/tmp/day16_exercise
```
