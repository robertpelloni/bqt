# Project Roadmap

## Phase 1 & 2: Foundation & QML Wiring (Complete)
- [x] Establish Omni-Workspace guidelines and C++ backend boilerplate.
- [x] Wire QML Engine (`QQmlApplicationEngine`), implement `qmlRegisterType` pipeline.
- [x] Intercept core OS events and render virtual cursors via `OmniDeveloperOverlay`.

## Phase 3: OS Routing & Focus Trees (Complete)
- [x] Resolve Dual-Hierarchy crisis (refactor to `QQuickPaintedItem`).
- [x] Map `DeviceId -> Focused Widget` in `OmniInputManager`.
- [x] Route Keyboard/Touch events independently of OS window manager.

## Phase 4: UI Comprehensiveness & The Ultimate Typing Test (Complete)
- [x] Implement robust `OmniButton` and `OmniSlider`.
- [x] Implement `OmniTextField`: Prove independent, concurrent multi-user typing works.
- [x] Implement `OmniDial` for audio applications.
- [x] Scaffold core DSP nodes (`OmniFilter`, `OmniGain`).

## Phase 5: Hybrid Framework Transcendence (JUCE/ImGui/WASM)
- [ ] **JUCE RHI Bridge:** Refine `JuceWidget` to share Vulkan/DirectX textures directly with the Qt RHI.
- [ ] **Audio Graphing:** Implement `OmniAudioGraph` wrapping `juce::AudioProcessorGraph` and connect the QML `OmniFilter`/`OmniGain` nodes into it.
- [ ] **Platform Drivers:** Implement raw Windows/Linux plugins to grab true hardware device signatures instead of mocked strings.

## Phase 6: Final Polish
- [ ] Implement `OmniProgressBar`, `OmniCheckBox`.
- [ ] Combine redundant functionality from linked submodules.
- [ ] 100% test coverage.
