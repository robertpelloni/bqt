# Session Handoff Summary

### Completed Tasks
- Renamed the core framework identity from `bobui`/`bobqt` to `BQt` inside the Go internal backend and `cmake/tests/` scripts.
- Ported the `AudioGraph` component and its lifecycles to Go, Rust, C#, and Java achieving 1:1 cross-language feature parity.
- Ported `OmniGain` and `OmniSynthesizer` to Rust, C#, and Java with parity to the Go implementation.
- Mapped JUCE audio primitives into the Go `AudioGraph` via `JUCEAudioGraphAdapter` and `JUCEProxyNode`.
- Implemented native Go routines to handle BQt signal/slot emission seamlessly.
- Successfully reconciled the `origin/bqt-renaming-and-audio-graph-*` feature branch into `main`.
- Updated WebAssembly architecture CI flags (`wasm_singlethread`) and resolved missing Qt modules in GitHub Actions.

### Upcoming Milestones
- Finish bridging the Ultimate++ elements via `BobQUppComponentRegistry`.
- Start porting the UI backend and widget components to the remaining target languages.
- Finish testing the native CMake build environments.

### Context / Notes
- **Do not use global find and replace for "bobui/qt -> bqt"**: This has consistently broken the complex CMake layouts and C++ submodules. The rename operations must remain file-specific or tightly scoped to single directories.
- Keep updating `CHANGELOG.md` properly using reverse chronological order at the top of the file, not the bottom.
