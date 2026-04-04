# Project Vision: BobUI / OmniUI

## Scope Clarification
`bobui` is the **framework/kernel project**.

It is **not** the end-user operating shell/desktop product. The shell/OS experience belongs to the adjacent `bobfilez` project, which consumes `bobui` as its UI, input, audio, networking, and synchronization foundation.

## Ultimate Goal
Build a high-performance, multi-user application framework that reaches or exceeds the practical strengths of:
- **Qt 6 / Qt 7 direction** for application framework breadth
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

## Engineering Philosophy
- Prefer small, verifiable improvements over sprawling unverified additions.
- Avoid claiming parity where build/runtime proof does not yet exist.
- Separate framework concerns (`bobui`) from shell concerns (`bobfilez`).
- Preserve past exploratory work, but clearly label experimental layers.
