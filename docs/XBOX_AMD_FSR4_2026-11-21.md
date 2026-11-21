# Xbox + AMD FSR 4 串流路径（2026-11-21）

## 目标

让 Xbox home streaming 经过 Veyra 现有原生 D3D12 增强图，在 RX 9000 上优先使用 FidelityFX 光流、FSR 4.x ML 超分与 FSR 4 ML 2X 帧生成；不可用时保留明确、可审计的 FSR 3.1 回退。串流输入只有最终视频像素，因此 motion/depth 仍是 Veyra 估算 guidance，不宣称等同游戏原生接入。

## 本次修改

- 将 `veyra_xbox_model` 从 `VEYRA_ENABLE_MOONLIGHT` 条件中拆出。Xbox 协议、视频源和 QML 模型现在由 `VEYRA_ENABLE_XBOX` 独立控制。
- 完整 QML 构建入口同时开启 Remote Play、Moonlight 与 Xbox，满足发行打包器已有的四项功能开关检查。
- FSR 超分初始化不再把 provider 列表首项当作实际选择。RX 9000 请求 4.x provider；未枚举到 4.x 时显式选择 3.1，创建后再次查询并核对真实 provider ID/名称。
- 保留现有行为：AMD 适配器默认 FidelityFX optical flow；RX 9000 默认 FSR 4 ML frame generation；FSR FG 固定 2X，4.x 不可用时由引擎恢复到 FSR 3.1。

## 本次检查

- `git diff --check`：通过。
- Xbox/CMake/FSR provider 静态契约检查：通过。
- `python3 -m py_compile scripts/package-qml-release.py scripts/package-windows-installer.py`：通过。
- `python3 scripts/acceptance/test-qml-release-package.py`：10/10 通过；测试生成的临时目录已删除。
- GitHub Windows 2022 / MSVC 聚焦 CI：生产 `Account`、`StreamApi`、`WebRtcSession` 与 HTTPS 实现编译、链接通过。
- WebRTC 本地 loopback 与 FSR provider 选择测试：通过；RX 9000 请求 4.x，未找到或创建结果不符时回退 3.1。
- 产品构建合同检查：通过；完整 QML 入口同时开启 Remote Play、Moonlight 与 Xbox。
- 同一提交的 Linux OBS filter checks：通过。

Windows 首轮检查暴露 `libdatachannel` 只启用默认 WebSocket、未编入媒体处理器。聚焦 CI 清单现显式使用 `libdatachannel[srtp]`；该端口以 `srtp` feature 关闭 `NO_MEDIA`，修复后 MSVC 与测试全部通过。失败记录保留为 GitHub Actions run `44893338200`，成功记录为 run `44894075301`。

## 未验证边界

本轮已经完成 Windows/MSVC 下的聚焦 Xbox 核心编译与单元/合同测试，但没有构建完整 Qt GUI，也没有在 RX 9070 XT、AMD 驱动和真实 Xbox 视频流上做端到端验收。因此不能把 CI 结果表述成“FSR 4 实机已启用”。下一步应在目标机器新建隔离构建目录，确认日志依次出现 Xbox active、FidelityFX optical flow、`fsr-sr provider=4.*` 与 `fsr-fg provider=4.0.*`；若驱动只枚举 3.1，日志必须明确记录回退。
