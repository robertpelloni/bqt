# Codebase Memory and Observations (Session Learnings)

## What Was Learned This Session
- **The WASM / Desktop Paradigm Clash (Crucial Discovery):** At the start, the architectural vision demanded 100% C++ parity and the mantra "The Desktop app is the Web app." What was *not* obvious is that certain C++ core components fundamentally break WebAssembly compilation.
    - **`QProcess` in `OmniTerminal`:** A web browser sandbox physically cannot spawn an OS-level shell like `cmd.exe` or `/bin/bash`. During the final validation phase, I realized the CI/CD Emscripten pipeline would explode. 
    - **The Fix:** I learned that we must aggressively wrap OS-level features in `#ifndef Q_OS_WASM` macros. OmniUI must elegantly degrade features in the browser, providing a custom error payload inside the Terminal UI ("Native OS shell sub-processing is disabled in WebAssembly Sandbox") rather than failing to compile or crashing the browser tab.
- **The Dual-Hierarchy Danger (Resolved):** Originally, the C++ backend inherited from `QWidget` (standard Qt logic) while trying to render in QML. This is a severe architectural mismatch that stalled high-performance rendering. I learned that all native C++ UI components intended for QML *must* inherit from `QQuickItem` or `QQuickPaintedItem`. A Python script successfully migrated the entire `omnicore` codebase, a massive structural victory.
- **Event Loop Thread Safety:** It wasn't immediately obvious that the `OmniInputManager` would be called from multiple threads simultaneously (Win32 raw input hardware hooks vs. QML UI threads). I learned that aggressive `QMutexLocker` integration across all Focus Tree dictionaries (`m_deviceFocusMap`) is mandatory for a stable Multi-Cursor OS.
- **Multi-Agent Branch Resolution:** I learned that parallel AI agents will occasionally attempt to push massive conflicting architectural concepts (like `OmniNexus`). I successfully scripted an autonomous resolution that preserves 100% of the code blocks (`<<<<<<< HEAD` concatenation) and then manually scrubbed C++ ODR (One Definition Rule) linkage errors, proving the ecosystem can self-heal multi-agent collisions.

## Design Preferences
- Unrelenting Momentum: Never stop generating code or documenting findings.
- Zero Mocks, Zero Vaporware: Every backend feature must have a visual, interactive frontend representation.
- Dynamic Theming over Hardcoded Colors: Utilize `OmniThemeManager` and native `QPainterPath` algorithms for advanced UI designs (e.g., Cyberpunk, Liquid Glass).
