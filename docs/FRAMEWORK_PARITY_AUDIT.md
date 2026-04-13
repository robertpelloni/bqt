# Framework Parity Audit

## Scope
This audit is about the `bobui` framework/kernel itself, not the adjacent `bobfilez` shell.

## Verified Current Strengths
### Qt / JavaFX style strengths present
- Widget/layout foundation exists in C++ (`OmniUI/omnicore`) and experimental Go (`internal/ui`).
- State/binding concepts exist (`OmniValueTree`, `OmniProperty` in C++; `Property[T]` in Go).
- Multiplayer input foundation exists (`OmniInputManager`, `OmniUserManager`, mesh sync).
- Documentation, markdown, plotting, and list/tree controls exist in some form.

### JUCE style strengths present
- Audio graph primitives exist (`OmniAudioGraph`, filters, gain, MIDI handlers, sequencer).
- Time synchronization concepts exist (`OmniMasterClock`).
- Mesh audio concepts exist in the Go port.

### Dear ImGui style strengths present
- Immediate-mode experiments exist (`OmniImGuiCanvas` in C++).
- Node editor and docking concepts exist in the Go track.

## Verified Gaps / Incomplete Areas
### Qt 6 / Qt 7 direction
- No verified, compile-tested full-feature `TableView` equivalent in Go.
- Selection models, sorting models, and delegate systems are not at parity.
- RHI abstraction is present conceptually, but not fully verified in Go.

### JUCE
- Go audio graph is structurally present but not yet feature-complete versus JUCE’s mature plugin-hosting / parameter automation stack.
- Remote audio routing exists conceptually but needs runtime proof.

### JavaFX
- CSS/styling concepts exist, but the Go renderer does not yet expose a comparably expressive, verified styling surface.
- Advanced layout panes are present in mixed maturity; some Go widgets are still baseline implementations.

### Dear ImGui / ImPlot
- Plotting and immediate mode exist, but not yet fully verified for feature richness, tooling depth, and runtime stability.

## Recommended Next Real Work
1. Stabilize and verify the Go UI runtime package by package.
2. Add missing `TableView` support in Go.
3. Add selection, sorting, and column metadata primitives.
4. Tighten JUCE-style parameter/state automation.
5. Keep shell concerns in `bobfilez`.
