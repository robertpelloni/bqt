# BobQ Development Roadmap

## Phase 1: Foundation (Completed)
- [x] Initial framework architecture and meta-object system setup.
- [x] Initial Go port foundations (`internal/`, `main.go`).
- [x] Rename branding from Bobui to BobQ.
- [x] Integrate JUCE and Ultimate++ as submodules.
- [x] Establish universal AI agent instructions and autonomous pipeline.

## Phase 2: Unification & Interoperability (In Progress)
- [x] Integrate U++ widgets through the BobQ standard API interface.
- [ ] Map JUCE audio primitives into the `OmniAudioGraph` and `OmniSynthesizer` (Go port in progress).
- [ ] Establish a unified Event Loop to drive Qt, GTK, JUCE, and U++ components seamlessly.
- [ ] Achieve 100% 1:1 feature parity with GTK.

## Phase 3: The Go Transcendence
- [x] Implement native Go routines for audio synthesis (`OmniSynthesizer`).
- [ ] Methodical 1:1 porting of remaining C++ components (`OmniFilter`, `OmniGain`) to Go.
- [ ] Implement Go routines to handle BobQ signal/slot emission seamlessly.

## Phase 4: World Domination
- [ ] 1:1 parity with JavaFX, WinUI, and DearImGui.
- [ ] Build robust native UI shells (web, desktop, embedded) via `bobfilez`.
