# BobQ Development Roadmap

## Phase 1: Foundation (Completed)
- [x] Initial framework architecture and meta-object system setup.
- [x] Initial Go port foundations (`internal/`, `main.go`).
- [x] Rename branding from Bobui to BobQ.
- [x] Integrate JUCE and Ultimate++ as submodules.
- [x] Establish universal AI agent instructions and autonomous pipeline.

## Phase 2: Unification & Interoperability (In Progress)
- [ ] Establish a unified Event Loop to drive Qt, GTK, JUCE, and U++ components seamlessly.
- [ ] Map JUCE audio primitives into the `OmniAudioGraph` and `OmniSynthesizer`.
- [ ] Expose U++ widgets through the BobQ standard API interface.
- [ ] Achieve 100% 1:1 feature parity with GTK (implementing C-style structural paradigms).

## Phase 3: The Go Transcendence
- [ ] Methodical 1:1 porting of C++ components to Go.
- [ ] Implement Go routines to handle BobQ signal/slot emission.
- [ ] Port the entire JUCE DSP stack to native Go where feasible.

## 4. Framework Parity Work
- [ ] Continue runtime-verifying parity claims instead of expanding only by file count.
- [ ] Strengthen model/view parity (`TableView`, selection, sorting, delegates).
- [ ] Tighten JUCE-style parameter/state automation and mesh-aware DSP routing.
- [x] Document a grounded comparison between the active Go `bobui` track and `BTK` to clarify strategic boundaries.
- [ ] Use the BTK comparison to keep `bobui` focused on experimental framework primitives, collaboration semantics, and bridge/runtime ideas rather than inflated breadth claims.

## 5. Stabilization
- [ ] Replace remaining placeholder Gio widgets with richer implementations.
- [ ] Continue reducing package coupling.
- [x] Add tests for Go kernel/data/net packages.
- [x] Improve `ScrollView` scrollbar occupancy/overlay policy and lightweight two-axis behavior.
- [x] Integrate the Go `WindowManager` into the live runtime composition path.
- [x] Add clickable tab selection and tab-aware managed-window visibility/content.
- [x] Add managed-window move/resize/close baseline behavior.
- [x] Add managed-window focus/activation baseline behavior.
- [x] Add pointer-driven activation routing from direct window interaction.

## 6. Strategic Alignment
- [x] Record the current `BTK` relationship in `docs/ai/design/2026-04-04-bobui-vs-btk-comparison.md`.
- [x] Document a safe staged strategy for C++/Go side-by-side migration instead of a destructive global `qt` -> `bobui` rename.
- [x] Classify the current C++ rename-artifact explosion into migration buckets and select `cmake/` package/export compatibility as the first safe implementation bucket.
- [x] Implement the first additive `cmake/` package/export compatibility slice without staging unrelated rename artifacts.
- [x] Wire the new BobUI CMake compatibility shims into install/export generation without replacing the canonical Qt package layout in one shot.
- [x] Add stronger native/prefix validation for the additive BobUI package-generation path through a configure-time publication smoke test.
- [x] Extend the additive BobUI CMake compatibility path into a first direct module-package slice for `Core` and `Widgets`.
- [x] Extend BobUI module-package shims into a second high-value public slice for `Gui` and `Network`.
- [x] Extend BobUI module-package shims into a third high-value public slice for `Sql` and `Xml`.
- [x] Extend BobUI module-package shims into a fourth high-value public slice for `OpenGL` and `OpenGLWidgets`.
- [x] Extend BobUI module-package shims into a fifth high-value public slice for `DBus` and `PrintSupport`.
- [x] Extend BobUI module-package shims into a sixth high-value public slice for `Concurrent` and `Test`.
- [x] Strengthen umbrella BobUI package validation to cover a broader supported multi-module component set.
- [x] Centralize the supported BobUI CMake component matrix and validate it with a manifest consistency test.
- [x] Add a checked human-readable status report for the supported BobUI component matrix.
- [x] Generate the supported component status report from the manifest-backed component inventory.
- [x] Add a consolidated validation entry point for the BobUI CMake compatibility stack.
- [x] Wire the exact verified Go baseline and the CI-friendly BobUI compatibility gate into the Go workflow.
- [x] Add a repeatable qtbase-native configure preflight gate that distinguishes missing-toolchain environments from real configure regressions.
- [ ] Execute the qtbase-native configure gate in an environment with a visible C/C++ compiler toolchain.
- [ ] Extend BobUI module-package shims to other high-value public modules in small verified slices.
- [ ] Keep `bobui` positioned as a fast idea/incubation track for ownership semantics, bridge contracts, and compile-safe parity exploration.
- [ ] Promote ideas from `bobui` into larger native framework tracks only after they are scoped, validated, and clearly separated from shell/product concerns.
