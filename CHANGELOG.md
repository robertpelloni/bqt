## [1.0.8] - 2026-04-02
### Added
- Put the Multi-Cursor typing architecture to the ultimate test: Implemented `OmniTextField` natively as a `QQuickPaintedItem`. It perfectly hooks into the `OmniInputManager`'s independent focus tree and directly intercepts routed `QKeyEvent`s, allowing concurrent multi-user typing.
- Implemented `OmniDial` with complex polar/mouse delta calculations, bridging high-end native audio dial logic into the QML engine.
- Established the core DSP foundation: Implemented `OmniFilter` and `OmniGain` as native QML-exposed audio processors, paving the way for the JUCE AudioGraph wrapping.
- Replaced the previous dummy mocks for TextField, Dial, Filter, and Gain in the `OmniQmlRegistration` system with these robust, native C++ implementations.

## [1.0.7] - 2026-04-02
### Added
- Solved the Dual-Hierarchy Architectural Crisis: Autonomously refactored all generated backend classes from legacy `QWidget` inheritance to modern `QQuickPaintedItem` / `QQuickItem`. The backend is now natively designed for SceneGraph hardware acceleration and QML integration.
- Fully implemented `OmniButton` and `OmniSlider` as `QQuickPaintedItem`s, providing dynamic theming, `Q_PROPERTY` exposure, and native mouse/hover routing directly within the C++ layer.
- Expanded OS Event routing: `OmniApp::notify` now intercepts `QKeyEvent`s and `QTouchEvent`s, completely bypassing standard Qt global focus limitations.
- Built out the `OmniInputManager` to map independent Focus Trees (`DeviceId -> Focused Widget`). Multi-cursor users can now theoretically type in separate text fields concurrently.
- Replaced dummy facades in `OmniQmlRegistration` with the real, newly-implemented `OmniButton` and `OmniSlider`.

## [1.0.6] - 2026-04-02
### Added
- Implemented `OmniQmlRegistration` to automatically register all generated core classes to the QML Engine.
- Upgraded `OmniApplication::loadMainSource` to actually initialize a `QQmlApplicationEngine`, bind signal/slots, and load the QML payloads.
- Hacked the core OS event loop (`OmniApplication::notify`) to intercept `QMouseEvent`s at the root level, routing absolute global coordinates into the `OmniInputManager`.
- Physically wired up the `OmniDeveloperOverlay` to iterate through all connected `OmniInputDevice`s and paint distinct, color-coded "Virtual Cursors" over the entire workspace.
- This represents the completion of the baseline "Local Multi-Cursor" architecture rendering pipeline.

# Changelog

All notable changes to this project will be documented in this file.
The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/).

## [1.0.4] - 2026-04-02
### Added
- Implemented foundational `JuceWidget` C++ wrapper for multimedia integration.
- Introduced `OmniInputManager` singleton for multi-device HID identity tracking.
- Created `OmniDeveloperOverlay` with ImGui integration bridge.
- Published `MULTI_CURSOR_SPEC.md` and `OMNIUI_IMPLEMENTATION_DETAIL.md` documenting the path to an OS-level collaborative toolkit.
- Updated `OmniApplication` to manage multi-framework lifecycles (Qt, JUCE, ImGui).

## [1.0.3] - 2026-04-02
### Added
- Conducted extreme deep-dive codebase analysis mapping out OS-level capabilities.
- Documented full architectural details for JUCE, JavaFX, and Dear ImGui integration in `OS_TOOLKIT_ANALYSIS.md`.
- Updated `HANDOFF.md` with targeted instructions for the next agent cycle.

## [1.0.2] - 2026-04-02
### Added
- Created `OS_TOOLKIT_ANALYSIS.md` detailing the architectural parity with Qt6/Qt7 and the path to becoming an OS-ready toolkit.

## [1.0.1] - 2026-04-01
### Added
- Comprehensive documentation overhaul (VISION.md, MEMORY.md, DEPLOY.md, ROADMAP.md, TODO.md, IDEAS.md).
- Global AI instruction files (AGENTS.md, CLAUDE.md, GEMINI.md, GPT.md, copilot-instructions.md) standardizing development workflow.
- Established autonomous session loop protocols for multi-model AI handoffs.
- Initiated deep analysis of `bobui` towards 100% Qt6 parity.