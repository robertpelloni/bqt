# OmniUI Strategic Roadmap

## Status: v41.1.0 - The "Nexus" Complete Refactor Phase

### Accomplished (v40 -> v41.1)
The focus of this phase was transitioning from "Breadth" (creating stubs for 40+ conceptual modules) to "Depth".
- **Backend Refactoring:** All 30 major modules (from OmniKernel down to OmniML) have had their C++ backends completely rewritten. We replaced mocked static outputs with robust state machines, mathematical physics integrations, custom timers, complex algorithms, QJSEngine wrappers, and custom QQuickPaintedItem renderers.
- **Frontend Refactoring:** Every corresponding QML application has been rebuilt into highly-detailed, beautiful, color-coded, and deeply interactive dashboards. The UIs are packed with custom <Canvas> graphing elements, physics sandboxes, and exhaustive tooltips explaining the simulation variables.
- **Documentation:** The `Manual.md` and `CHANGELOG.md` have been meticulously updated with precise documentation explaining how the new C++ backends function and how the UIs interact with them.

### Immediate Next Steps (v42.0 - The "Integration" Phase)
While the individual modules are incredibly robust, they currently run in isolated ApplicationWindow instances via their respective `main.cpp` binaries.
1. **The OmniNexus Shell:** We need to build a master application that integrates *all* 30+ modules into a single, cohesive Desktop Environment or Window Manager.
2. **Unified Simulation Clock:** Currently, modules like OmniPhysics, OmniKernel, and OmniGenesis use their own internal `QTimer` loops. We need to wire them up so the `OmniSimulationEngine` (or `OmniTimeMachine`) drives the tick rate for the entire framework universally.
3. **Cross-Module Interplay:**
    - Wire `OmniHardware` (GPIO pins) to trigger `OmniPortal` gate openings.
    - Tie `OmniMind` (BCI) intention detection directly into the `OmniCreator` compiler start/stop functions.
    - Have the `OmniUniverse` constants directly alter the gravity scaling in `OmniPhysics`.

### Long Term Objectives (v43.0+)
- **Network Multiplayer:** Expand `OmniNet` to actually bind two different physical instances of OmniUI together, synchronizing their `OmniUniverseManager` states.
- **Mobile Porting:** Ensure the massive QML layouts dynamically compress down into drawer-based UIs for iOS/Android targets using Qt Quick Controls.
- **WebAssembly Build:** Establish a CI/CD pipeline to compile the entire OmniUI C++ backend into WASM for browser-based demonstration.
