# Project Vision: BobUI / OmniUI

## Scope Clarification
`bobui` is the **framework/kernel project**.

It is **not** the end-user operating shell/desktop product. The shell/OS experience belongs to the adjacent `bobfilez` project, which consumes `bobui` as its UI, input, audio, networking, and synchronization foundation.

## Ultimate Goal
Build a high-performance, multi-user application framework that reaches or exceeds the practical strengths of:
- **BobUI 6 / BobUI 7 direction** for application framework breadth
- **JUCE** for audio/DSP and media workflows
- **JavaFX** for styling, layout, and binding ergonomics
- **Dear ImGui / ImPlot** for immediate-mode tooling and debugging surfaces

while also introducing a first-class foundation for:
- **local multi-user interaction** (multiple devices, device identity, ownership, contention handling)
- **remote multi-user collaboration** (cursor/state/asset synchronization)
- **AI-aware interaction** (DOM/context serialization, automation hooks)
- **cross-platform deployment** (desktop first, WASM where realistic)

## Product Boundary
### BobUI owns
- input and device identity
- widget toolkit and layout system
- media/audio graph primitives
- networking/state synchronization primitives
- developer tooling overlays and introspection
- framework-level themes, styling, animation, plotting, document views
- portable runtime/service abstractions

### Bobfilez owns
- desktop shell / launcher / taskbar / start menu
- login/desktop metaphors as product UX
- workflow-specific end-user applications
- system-level shell composition and user-facing orchestration

## Current Strategic Direction
1. **Stabilize reality over aspiration**: verify what actually builds and runs.
2. **Reduce drift** between documentation and code.
3. **Prioritize framework correctness** over adding more speculative subsystems.
4. **Keep Go work clearly marked** as experimental or partial until build-verified.
5. **Harden multiplayer fundamentals**: ownership, collaboration, synchronization, permissions.
6. **Deepen verified interaction primitives**: keep adding small but real input surfaces such as scrolling and touch/swipe behavior before claiming broader parity.
7. **Prefer executable local runtimes over empty contracts**: where possible, move framework surfaces like `WebView` from API-only shape toward small, verifiable runtime behavior.
8. **Prefer honest partial spatial models over fake completeness**: lightweight two-axis viewport state is better than claiming full 2D scrolling before the container/runtime architecture exists.
9. **Exercise primitives in a live runtime as early as possible**: once widgets and bridges become behaviorally meaningful, move them into a real frame loop so interaction claims are grounded.
10. **Use adjacent framework research productively**: treat `BTK` as a larger native-framework reference and keep `bobui` focused on the abstractions it is best suited to incubate quickly.
11. **Migrate the legacy C++ track compatibility-first**: BobUI naming should first appear through package/export/header compatibility surfaces before any broad source-identifier rewrite, so the C++ and Go tracks can keep converging side by side.
12. **Prefer additive package-discovery shims before export rewiring**: a small BobUI-facing CMake forwarding layer is a safer first migration move than trying to rename the entire native framework surface at once.
13. **Extract migration logic into testable helpers**: compatibility-bucket code should be factored so it can be validated in isolated configure-time tests before broader native integration work.
14. **Grow package identity from umbrella to modules in slices**: once a top-level BobUI package surface is stable, extend compatibility into a few high-value public modules before considering wider native renaming.

## Engineering Philosophy
- Prefer small, verifiable improvements over sprawling unverified additions.
- Avoid claiming parity where build/runtime proof does not yet exist.
- Separate framework concerns (`bobui`) from shell concerns (`bobfilez`).
- Preserve past exploratory work, but clearly label experimental layers.
