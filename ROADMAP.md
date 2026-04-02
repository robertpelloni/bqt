# Project Roadmap

## Phase 1: Foundation & Core Boilerplate (Complete)
- [x] Establish Omni-Workspace guidelines (`AGENTS.md`, `UNIVERSAL_LLM_INSTRUCTIONS.md`).
- [x] Scaffold `OmniUI/omnicore/src` directory structures and generate C++ boilerplate.

## Phase 2: QML Engine Wiring & Multi-Cursor Base (Complete)
- [x] Implement `QQmlApplicationEngine` lifecycle inside `OmniApplication`.
- [x] Establish `OmniQmlRegistration` to expose backend classes to QML.
- [x] Hack core Qt event loop (`notify`) to intercept mouse coordinates.
- [x] Implement the `OmniDeveloperOverlay` virtual cursor rendering loop.

## Phase 3: Robust Widget Logic & OS Routing Expansion (Complete)
- [x] **Solve Dual-Hierarchy Crisis:** Refactor C++ widgets from `QWidget` to `QQuickPaintedItem`.
- [x] **Widget Implementation:** Implement `OmniButton` and `OmniSlider` with properties and native drawing.
- [x] **Focus Tree Segregation:** Expand `OmniInputManager` to track `DeviceId -> Focused Widget`.
- [x] **Keyboard/Touch Routing:** Expand `notify` to route `QKeyEvent`s and `QTouchEvent`s securely into the multi-cursor focus map.

## Phase 4: Hybrid Framework Transcendence (JUCE/ImGui/WASM)
- [ ] **JUCE RHI Bridge:** Refine `JuceWidget` to share Vulkan/DirectX textures directly with the Qt RHI.
- [ ] **Audio Graphing:** Implement `OmniAudioGraph` and `OmniAudioProcessor` wrapping `juce::AudioProcessorGraph`.
- [ ] **Platform Drivers:** Implement raw Windows/Linux plugins to grab true hardware device signatures.

## Phase 5: UI Comprehensiveness & Expansion
- [ ] Implement remaining core widgets (`OmniTextField`, `OmniDial`, `OmniProgressBar`).
- [ ] Combine redundant functionality from linked submodules.
- [ ] 100% test coverage.
