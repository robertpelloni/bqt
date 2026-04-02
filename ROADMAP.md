# Project Roadmap

## Phase 1: Foundation & Core Boilerplate (Complete)
- [x] Establish Omni-Workspace AI agent guidelines and global documentation (`AGENTS.md`, `UNIVERSAL_LLM_INSTRUCTIONS.md`).
- [x] Implement robust documentation structure (`VISION.md`, `MEMORY.md`, `CHANGELOG.md`, `VERSION.md`).
- [x] Complete codebase analysis of `bobui` vs `qtbase`.
- [x] Establish Submodule Dashboard and tracking scripts.
- [x] Scaffold the missing `OmniUI/omnicore/src` and `OmniUI/omnicore/include` directory structures.
- [x] Autonomously generate the foundational C++ boilerplate for widgets, charts, audio DSP, and networking.

## Phase 2: QML Engine Wiring & Multi-Cursor Base (Complete)
- [x] Implement `QQmlApplicationEngine` lifecycle inside `OmniApplication`.
- [x] Establish `OmniQmlRegistration` to expose backend classes to QML via `qmlRegisterType`.
- [x] Hack the core Qt event loop (`notify`) to intercept and tag input events for multi-user routing.
- [x] Implement the `OmniDeveloperOverlay` virtual cursor rendering loop.

## Phase 3: Robust Widget Logic & QML Facades
- [ ] **Facade Implementation:** Fully implement the QML facades (e.g., `OmniQmlButton`) so they map directly to the underlying `OmniButton` C++ logic, or refactor to pure `QQuickItem` SceneGraph rendering depending on performance metrics.
- [ ] **JUCE RHI Bridge:** Refine `JuceWidget` to share Vulkan/DirectX textures directly with the Qt RHI instead of using software `QPainter` fallbacks.
- [ ] **No Vaporware Audit:** Ensure all examples (`HelloOmni`, `ComplexDemo`) compile cleanly and render correctly.

## Phase 4: Hardware-Level Input Drivers
- [ ] Implement raw platform plugins (`RegisterRawInputDevices` on Windows, `libinput` on Linux) to feed `OmniInputManager` with real, distinct hardware HID signatures.
- [ ] Complete the per-device focus tree mapping (allowing Mouse 1 to type in TextField A while Mouse 2 types in TextField B).

## Phase 5: UI Comprehensiveness & Polishing
- [ ] Wire up all backend configurations to the frontend OmniUI with extreme detail.
- [ ] Add comprehensive tooltips, labels, and manual descriptions within the UI for every single toggle and slider.
- [ ] Combine redundant functionality from linked projects/submodules into singular OmniUI components.
- [ ] Achieve 100% test coverage for all features.
