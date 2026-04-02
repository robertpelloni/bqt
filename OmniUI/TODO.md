# OmniUI Issue Tracker & Micro-Tasks

## High Priority Bugs
- [ ] Fix `OmniPhysicsWorld` AABB collision overlapping edge-cases (fast objects tunnelling through static walls).
- [ ] Resolve the case where `OmniCompiler` log output grows endlessly without memory culling.
- [ ] Handle potential crash in `OmniWeb3` if a huge token mint request results in floating point inaccuracy on gas estimation.
- [ ] Ensure the QML `<Canvas>` in `OmniBioExplorer` (ECG) clears the buffer properly on system suspend to prevent UI hangs.

## Feature Completeness (Missing Implementations)
- [ ] **Omni3D:** Needs dynamic lighting and shading calculations. The wireframe mode works, but standard solid polygons require a software Z-Buffer implementation in C++.
- [ ] **OmniML:** Needs actual back-propagation math (even simple matrix mult) instead of stochastic gradient-descent simulation approximations.
- [ ] **OmniSpace:** Needs 3D orbital trajectory lines drawn behind the satellite, instead of just the instantaneous velocity vector.
- [ ] **OmniHardware:** Add support for decoding mock UART / I2C streams instead of just GPIO.

## Refactoring Opportunities
- [ ] Extract the common `QTimer` tick pattern from `OmniGenesis`, `OmniKernel`, `OmniPhysics`, etc. into a single `OmniClockTickBase` inherited class.
- [ ] Standardize the QML UI components: The repeated "Header" / "ToolBar" code in every `main.qml` should be pulled out into an `OmniGlobalHeader.qml` reusable component.
- [ ] Extract the recurring `QVariantList` log patterns (e.g., in `OmniAbyss`, `OmniCompiler`, `OmniWebSocket`) into a generic `OmniLogBuffer` utility class in C++.
