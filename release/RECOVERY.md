# 每日发布恢复说明

## 正常路径

定时任务每次先读 `release/state.json`，只处理 `next_day`，在独立 `release/dayXX-YYYY-MM-DD` 分支和 Pull Request 中提交。GitHub Actions 的结构检查、GCC、Clang、ASan/UBSan 全部通过后才允许合并。

## 失败时

1. 不修改默认分支上的 `published_days` 和 `next_day`。
2. 保留失败分支和 Pull Request，读取 Actions 日志并只修复当天文件。
3. 重新运行 PR 检查；不要另建同一天的重复 PR。
4. 检查通过后合并，再确认默认分支状态只前进一天。

## 手动触发

- 可在 GitHub Actions 的 `course-ci` 页面使用 `workflow_dispatch` 重跑全仓库验证。
- 可在 ChatGPT 的定时任务页面手动运行每日发布任务；任务仍以仓库状态为准，因此重复运行不会重复发布。
- 若某个计划时点失败，先恢复同一天，再让下一次计划运行继续读取 `next_day`。

## 完成与停止

第 24 天成功后，状态必须为 `completed`，`next_day` 必须为 `null`。当前日程只包含余下 23 次计划运行，因此不会计划第 25 天。
