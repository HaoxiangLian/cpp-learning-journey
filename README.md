# C++ 24 天基础重构与秋招面试突击

这是一套以 C++17 为主线、面向基础薄弱学习者的 24 天课程。课程先建立准确的语言模型，再训练代码辨析与面试表达；机器人、传感器和任务队列只用作轻量示例。

## 当前发布状态

- 课程开始：2026-08-17
- 每日发布：09:30（Asia/Shanghai）
- 已发布：
  - [第 1 天：源代码如何变成可运行程序](days/day01.md)
  - [第 2 天：变量、对象、类型与初始化](days/day02.md)（提前发布）
  - [第 3 天：表达式、转换与控制流](days/day03.md)（提前发布）
  - [第 4 天：函数如何划分责任](days/day04.md)（提前发布）
  - [第 5 天：名字何时可见，对象活多久](days/day05.md)（提前发布）
  - [第 6 天：间接访问与内存模型入门](days/day06.md)（提前发布）
  - [第 7 天：连续数据与手动资源管理](days/day07.md)（提前发布）
  - [第 8 天：类如何维护不变量](days/day08.md)（提前发布）
  - [第 9 天：对象怎样出生和结束](days/day09.md)（提前发布）
  - [第 10 天：复制资源为什么危险](days/day10.md)（提前发布）
  - [第 11 天：转移资源而不是复制](days/day11.md)（提前发布）
  - [第 12 天：继承表达什么关系](days/day12.md)（提前发布）
  - [第 13 天：运行时多态与对象模型](days/day13.md)（提前发布）
  - [第 14 天：一份代码怎样适配多种类型](days/day14.md)（提前发布）
- 唯一进度源：[progress.md](progress.md)
- 机器可读发布状态：[release/state.json](release/state.json)

## 从这里开始

1. 阅读 [24 天全局路线图](ROADMAP_24_DAYS.md)，只了解知识顺序，不提前背结论。
2. 开始当天讲义前，先回答“前置知识检查”；“攻坚目标”的第 n 项与“机制 n”对应，按编号阅读，并沿着“代码现象 → 普通话结论 → 就地定义 → 代码映射 → 进阶边界 → 反例修复”复述推理，不单独背术语卡。
3. 独立运行当天讲义链接的最小示例，再完成对应 `exercises/dayXX/` 练习。
4. 提交自己的答案后，再查看对应 `solutions/dayXX/` 答案与解析。
5. 从[按天术语索引](docs/glossary.md)进入当天术语文件复述定义，再用[面试题索引](interview/question-bank.md)做连续追问；完成口述后，到[按天面试答案索引](interview/answers/README.md)逐题核对主回答和追问答案。
6. 在 [progress.md](progress.md)记录当日完成情况，并在第 2、7、14 天回顾错题。

每天建议投入 90—120 分钟：前置检查 5 分钟，讲义与示例 55—70 分钟，练习 25—35 分钟，口述与复盘 10 分钟。未达到当天“完成标准”时，不跳过前置知识。

遇到“标准语义、实现模型、ABI、寄存器、优化器”等抽象词时，先确认自己能用当前代码回答三个问题：**有哪几个对象、哪个对象发生变化、它什么时候不能再用**。这些抽象词只用于补充精确边界；如果还不能预测示例，先停在代码状态表，不需要靠背诵底层名词继续往下赶。

## 构建与验证

需要 CMake 3.16+，以及支持 C++17 的 GCC 或 Clang。

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
ctest --test-dir build --output-on-failure
python3 scripts/validate_repository.py
```

在 GCC/Clang 上启用 Sanitizer：

```bash
cmake -S . -B build-sanitize \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCPP_LEARNING_ENABLE_SANITIZERS=ON
cmake --build build-sanitize --parallel
ctest --test-dir build-sanitize --output-on-failure
```

`exercises/**/broken/` 中的代码是显著标注的故意错误示例，不进入正常构建目标。

## 仓库结构

| 路径 | 作用 |
|---|---|
| `ROADMAP_24_DAYS.md` | 唯一的 24 天知识依赖大纲 |
| `docs/glossary.md` | 24 天术语导航，不保存正式定义 |
| `docs/glossary/dayXX.md` | 第 XX 天首次出现术语的唯一规范定义 |
| `docs/lecture-writing-guide.md` | 讲义叙事、首次术语解释与资料使用规范 |
| `days/dayXX.md` | 每日讲义 |
| `examples/dayXX/` | 可独立构建的正确示例 |
| `exercises/dayXX/` | 不含答案的当日练习 |
| `solutions/dayXX/` | 答案与解析 |
| `interview/question-bank.md` | 面试题索引与对应学习日 |
| `interview/answers/dayXX.md` | 按学习日拆分的主问题与连续追问参考答案 |
| `interview/answers/README.md` | 24 天面试答案导航 |
| `release/` | 幂等发布状态和清单 |
| `.github/workflows/ci.yml` | GCC/Clang、链接与仓库结构验证 |

## 课程边界

- 正文以 C++17 为准；C++11/14 来源会标注，少量 C++20 内容会明确写为补充。
- 标准保证与常见实现分开说明。比如虚函数表是常见实现技术，不是标准规定的数据结构。
- “未定义行为”表示标准不约束结果，不等同于“一定崩溃”或“一定段错误”。
- 算法练习只服务于 C++ 容器、迭代器和资源管理，不扩张为完整算法训练营。

旧版未完成课程已由回滚分支 `archive/pre-24day-rebuild-2026-08-16` 保留；本路线图取代旧的 7 天及其他中间版本计划。
