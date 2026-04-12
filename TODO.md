# BobQ Short-Term TODO List

## Phase 1 Parity Wrappers (Completed)
1. ~~**Build System Validation:**~~ Modularized `CMakeLists.txt` via `BobQSubmodules.cmake`.
2. ~~**Submodule Wrapper:**~~ Implemented `BobQJuceHost`.
3. ~~**Go Port Coverage:**~~ Generated `ANALYSIS.md`.
4. ~~**Documentation Sync:**~~ Generated `SUBMODULE_DASHBOARD.md`.
5. ~~**GTK Parity Research:**~~ Implemented `BobQBox` and `BobQGrid`.

## Phase 2: Unification & Interoperability
6. ~~**Ultimate++ Submodule Wrapper:**~~ Implement a `BobQUltimatePPHost` wrapper class that allows placing an Ultimate++ `Ctrl` or layout component inside a BobQ `QQuickPaintedItem`.
7. ~~**Unified Event Loop Sync:**~~ Connect the Qt `QEventLoop` explicitly to the U++ `Ctrl::ProcessEvents()` or JUCE MessageManager.
8. **Audio Graph Port:** Begin porting `OmniAudioGraph` DSP concepts to the Go engine in `internal/audio/`.
9. **Component Registry:** Expose U++ widgets (Buttons, Lists) dynamically through standard BobQ API structures to make them usable interchangeably with native Qt widgets.
10. **Documentation Check:** Outline U++ and JUCE bridging details explicitly inside `Manual.md` and the universal instruction suite.
