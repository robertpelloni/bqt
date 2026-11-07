# Short-Term Execution Goals (TODO)

## Phase 1 Parity Wrappers (Completed)
1. ~~**Build System Validation:**~~ Modularized `CMakeLists.txt` via `BobQSubmodules.cmake`.
2. ~~**Submodule Wrapper:**~~ Implemented `BobQJuceHost`.
3. ~~**Go Port Coverage:**~~ Generated `ANALYSIS.md`.
4. ~~**Documentation Sync:**~~ Generated `SUBMODULE_DASHBOARD.md`.
5. ~~**GTK Parity Research:**~~ Implemented `BobQBox` and `BobQGrid`.
6. ~~**Core Framework Renaming:**~~ Renamed bobui/qt to bqt globally across core source.

## Go Runtime Integration (Priority 1)
- [x] Add richer runtime integration for `Popup` and `Dialog`.
- [x] Add richer runtime integration for `Drawer`, `ToolTip`, and `Menu`.
- [x] Improve `ScrollView` and `ScrollBar` behavior.
- [x] Write tests for all above widgets inside `internal/ui/widgets/`.

## C++ Core Fixes (Priority 2)
- [x] Consolidate BQt CMake compatibility validation command and wire it into Go CI.
- [x] Complete the generic component registry (`BobQUppComponentRegistry`).

## Refactoring & Documentation
- [x] Outline U++ and JUCE bridging details explicitly inside `Manual.md` and the universal instruction suite.
- [x] Audit the BobQ C++ codebase to ensure cleanliness of the GTK implementation.
- [x] Continue porting `OmniAudioGraph` to pure Go.
- [x] Port components to Java and C# (Starting with `AudioGraph`)
- [x] Begin porting components to Rust (Starting with `AudioGraph`)
- [x] Port `OmniSynthesizer` to Rust, Java, and C#
- [x] Implement Go routines to handle BQt signal/slot emission seamlessly.
- [x] Map JUCE audio primitives into the `OmniAudioGraph` and `OmniSynthesizer` (Go port in progress).

- [x] Establish a unified Event Loop to drive BQt components seamlessly.
- [x] Implement system tray icon (`TrayIcon`) with server lifecycle actions and shutdown handlers.
- [x] Consolidate and redesign dashboard UI (`DemoSurface`) into a unified single-page layout grouped by high-value capabilities with descriptive tooltips and interactive parameter controls.
- [x] Verify and test `VoiceEngine` microphone streaming buffer capture lifecycle.
- [x] Implement automated test coverage for `internal/vm` subsystem (Lisp, MacroRecorder, NeuralEngine, AIAssistant).
- [x] Implement automated test coverage for `internal/net` subsystem (DiscoveryBeacon, StreamAssetToMesh).
- [x] Implement automated test coverage for `internal/state` subsystem (`ValueTree`).
- [x] Implement automated test coverage for `internal/ui/theme` subsystem (`Theme`, `StyleSheet`).
- [x] Implement automated test coverage for `internal/ui/anim` subsystem (`Animator`, `Animation`).
- [x] Fix `DeviceState` struct tags and pass `go vet` across `internal/...`.
- [x] Verify and test `FilterNode` TPT low-pass DSP math in `internal/audio`.
- [x] Verify and test `BobQBox` GTK sequential packing and layout in `internal/ui/widgets`.
- [x] Verify and test `Grid` flexible layout distribution in `internal/ui/widgets`.
- [x] Verify and test modal `Dialog` state and layout in `internal/ui/widgets`.
- [x] Verify and test transient `Popup` state and layout in `internal/ui/widgets`.
- [x] Verify and test `TabBar` active indexing and interactive button layout in `internal/ui/widgets`.
- [x] Verify and test `SplitView` ratio fallback and child widget layout in `internal/ui/widgets`.
- [x] Verify and test `ScrollBar` viewport clamping and multi-orientation layout in `internal/ui/widgets`.
- [x] Verify and test `ToolTip` visibility states and layout positioning in `internal/ui/widgets`.
- [x] Verify and test `Drawer` visibility states, width fallback, and child layout in `internal/ui/widgets`.
- [x] Verify and test `Menu` bounds clamping, item rendering, and selection dismissal in `internal/ui/widgets`.
- [x] Verify and test `Terminal` output ingestion and buffer trimming in `internal/ui/widgets`.
- [x] Verify and test `NotificationCenter` singleton lifecycle and toast expiration in `internal/ui/widgets`.
- [x] Verify and test `Calendar` fallback initialization and date formatting in `internal/ui/widgets`.
- [x] Verify and test `TableView` empty fallback and cell distribution in `internal/ui/widgets`.
- [x] Verify and test `ScrollView` list-driven layout and cross-axis viewports in `internal/ui/widgets`.
- [x] Verify and test `TouchArea` directional swipe gestures and layout in `internal/ui/widgets`.
- [x] Verify and test `AdminShell` user listing and permission lock status in `internal/ui/widgets`.
- [x] Verify and test `AuthShell` secure boot interaction and database null-safety in `internal/ui/widgets`.
- [x] Verify and test `LoginView` authentication transition and rendering in `internal/ui/widgets`.
- [x] Verify and test `RewindSlider` timeline visibility and slider rendering in `internal/ui/widgets`.
- [x] Verify and test `MarkdownView` header parsing and vertical layout in `internal/ui/widgets`.
