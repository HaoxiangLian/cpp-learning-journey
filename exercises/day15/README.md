# 第 15 天练习：迭代器、算法与 Lambda

[返回第 15 天讲义](../../days/day15.md) · [完成后查看答案](../../solutions/day15/README.md)

每题先画最小状态：`first` 当前在哪里、`last` 是否代表元素、算法返回哪里、Lambda 保存的是副本还是引用。只有状态确定后，再写术语和结论。

## 一、概念判断题

判断正误并用一句话说明理由。

1. 迭代器一定是裸指针，因此每种迭代器都支持 `it + 5`。
2. 半开区间 `[first, last)` 包含 `first` 表示的元素，但不包含 `last` 表示的结束位置。
3. `std::find(first, last, target)` 没找到目标时，返回值仍可安全解引用，只是得到一个默认值。
4. 谓词必须由 Lambda 编写，普通函数不能作为 `std::count_if` 的最后一个实参。
5. `[threshold]` 按值捕获后，外层 `threshold` 的后续变化会自动同步到 Lambda 保存的副本。
6. 只要能把两个对象写在 `std::sort(first, last)` 中，它们就一定满足排序算法的迭代器要求。

## 二、位置、算法、捕获与能力分析

1. 阅读下面代码，依次写出 `current` 创建后、一次递增后、三次递增后所表示的位置；指出每个时点能否执行 `*current`：

   ```cpp
   int values[]{4, 9, 2};
   auto current = std::begin(values);
   auto last = std::end(values);

   ++current;
   ++current;
   ++current;
   ```

2. 对下面两次查找分别写出 `found` 的位置、条件分支是否进入，以及数组是否被修改：

   ```cpp
   int values[]{4, 9, 2};
   auto first = std::begin(values);
   auto last = std::end(values);

   auto found_9 = std::find(first, last, 9);
   auto found_7 = std::find(first, last, 7);
   ```

3. 查看 [`broken/missing_lambda_capture.cpp`](broken/missing_lambda_capture.cpp)，指出哪个外层名字在 Lambda 函数体中被使用、捕获列表当前承诺了什么，以及编译器为什么必须拒绝这段代码。分别给出按值与按引用的修复方向，并说明选择依据。
4. 不运行代码，填写两个结果并说明 Lambda 对象各自保存什么：

   ```cpp
   int threshold = 5;
   auto by_value = [threshold] (int value) {
       return value >= threshold;
   };
   auto by_reference = [&threshold] (int value) {
       return value >= threshold;
   };

   threshold = 8;
   bool a = by_value(7);
   bool b = by_reference(7);
   ```

5. 查看 [`broken/sort_input_iterator.cpp`](broken/sort_input_iterator.cpp)，列出 `std::find` 遍历时需要的最小动作、`std::sort` 额外需要的迭代器能力，并解释为什么数组的 `int*` 可满足后者而输入迭代器不能。

## 三、小型编程题：用算法处理一组读数

打开 [`algorithm_samples.cpp`](algorithm_samples.cpp)，完成以下 TODO：

- 定义 `is_even(int)`，让它能充当一元谓词；
- 用 `std::begin`、`std::end` 取得数组的半开区间；
- 用 `std::find` 查找 `7`，只在返回位置不等于 `last` 时解引用并输出；
- 创建阈值 `5`，按值捕获它，用 `std::count_if` 统计不小于阈值的元素；
- 把普通函数 `is_even` 作为谓词，统计偶数；
- 用 `std::sort` 排序并输出全部元素。

完成后的稳定输出应为：

```text
found: 7
at least 5: 2
even: 2
sorted: 1 2 4 7 9
```

建议使用 C++17 严格警告编译：

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror \
  exercises/day15/algorithm_samples.cpp -o /tmp/day15_exercise
/tmp/day15_exercise
```

完成后再查看 [`solutions/day15/README.md`](../../solutions/day15/README.md)，比较的重点不是代码行数，而是每次解引用前是否满足位置前提、每个算法和谓词是否承担了清楚的单一责任。
