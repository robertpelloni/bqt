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
- [x] Verify and test `MeshExplorer` file list rendering and remote tagging in `internal/ui/widgets`.
- [x] Verify and test QuickControls (`ProgressBar`, `Dial`, `CheckBox`) in `internal/ui/widgets`.
- [x] Verify and test `DrawGrabGlow` multi-pass halo painting in `internal/ui/widgets`.
- [x] Verify and test `Plot` vertical column bar chart layout in `internal/ui/widgets`.
- [x] Verify and test `ParticleSystem` physics and layout in `internal/ui/widgets`.
- [x] Verify and test `VectorIcon` GPU path outline rendering in `internal/ui/widgets`.
- [x] Verify and test `BQtNotebook` tab click switching and active child layout in `internal/ui/widgets`.
- [x] Verify and test `CodeEditor` text input and flex layout in `internal/ui/widgets`.
- [x] Verify and test `DockArea` child layout rendering in `internal/ui/widgets`.
- [x] Verify and test `NodeEditor` layout, positioning, and clipping in `internal/ui/widgets`.
- [x] Verify and test `OmniStudio` IDE shell composition and nil child handling in `internal/ui/widgets`.
- [x] Verify and test `ShaderEffect` plasma and grid graphics layouts in `internal/ui/widgets`.
- [x] Verify and test `Raymarcher` rendering and zero-size fallback in `internal/ui/widgets`.
- [x] Verify and test `RHINode` zero and non-zero texture handle rendering in `internal/ui/widgets`.
- [x] Verify and test `Scene3D` node iteration and render execution in `internal/ui/widgets`.
- [x] Verify and test `ComputeKernel` GPU compute dispatch and buffer sync in `internal/ui`.
- [x] Verify and test `HapticsEngine` peer proximity drone triggers in `internal/ui`.
- [x] Verify and test `LiveReloader` watcher lifecycle in `internal/ui`.
- [x] Verify and test `ShaderSync` P2P mesh shader skin synchronization in `internal/ui`.
- [x] Expand unit test coverage for `WebView.Layout` and `BridgeContractJSON` in `internal/ui/widgets`.
- [x] Verify and test `MeshNode.Render` in `internal/ui/widgets`.
- [x] Verify and test audio/desktop/teleport/voice/stream mesh network services in `internal/net`.
- [x] Verify and test git/history/replicated-db/package-registry/timemachine in `internal/data`.
- [x] Verify and test UI widgets signal dispatch and event loop synchronization in `internal/ui`.
- [x] Verify and test ActionRegistry, AccessibilityManager, SearchEngine, and Property in `internal/kernel`.
- [x] Verify and test PluginManager registration and duplicate rejection in `internal/kernel`.
- [x] Verify and test ConsensusEngine, GridCompute, and SelfHealer in `internal/kernel`.
- [x] Expand unit test coverage for TouchArea helpers and Window.SetActive in `internal/ui/widgets`.
- [x] Verify and test AudioPlayer methods and Cluster DSP distribution in `internal/audio`.
- [x] Verify and test MidiHandler NoteOn/NoteOff and MidiMesh broadcasting in `internal/audio`.
- [x] Verify and test NodeHealth telemetry data and mesh broadcast in `internal/net`.
- [x] Verify and test NewShell constructor in `internal/ui/widgets`.
- [x] Verify and test TabBar edge cases (nil receiver, empty tabs, OOB index) in `internal/ui/widgets`.
- [x] Verify and test BQtHeaderBar custom widgets layout in `internal/ui/widgets`.
- [x] Verify and test ScrollView occupied placement and scroll deltas in `internal/ui/widgets`.
- [x] Verify and test TouchArea acceptSource, sourceString, and statusText in `internal/ui/widgets`.
- [x] Verify and test Plot single-item and high-density datasets in `internal/ui/widgets`.
- [x] Verify and test TrayIcon menu button clicks (mute, disconnect, restart, quit) in `internal/ui/widgets`.
- [x] Verify and test DemoSurface parameter adjustments and navigation clicks in `internal/ui/widgets`.
- [x] Verify and test Dialog button dismissals and TouchArea threshold/gesture lifecycle in `internal/ui/widgets`.
- [x] Verify and test BQtNotebook out-of-bounds indexing and ParticleSystem inactive state in `internal/ui/widgets`.
- [x] Verify and test SynthWidget buffer adjustments and zero constraints in `internal/ui/widgets`.
- [x] Verify and test Raymarcher maxInt helper and Menu empty items handling in `internal/ui/widgets`.
- [x] Verify and test ScrollBar helpers and zero track constraints in `internal/ui/widgets`.
- [x] Verify and test TableView tableMax helper and alternating row styling in `internal/ui/widgets`.
- [x] Verify and test ScrollView non-zero scroll deltas and cross-axis viewport clamping in `internal/ui/widgets`.
- [x] Verify and test Window active title bar styling and custom MinSize clamping in `internal/ui/widgets`.
- [x] Verify and test TouchArea event loop gesture integration in `internal/ui`.
