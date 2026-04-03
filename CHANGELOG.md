# Changelog

All notable changes to this project will be documented in this file.
The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/).

## [1.1.0] - 2026-04-03
### Added
- **Liquid Glass Edition**: Complete Notepad++ UI overhaul with Apple-inspired liquid glass aesthetics.
- `LiquidGlass.h` — Comprehensive glass material system:
  - `LiquidGlassPalette`: Centralized color/opacity token system (17 design tokens).
  - `LiquidGlassDWM`: Windows DWM integration (Win11 Acrylic, Win10 Acrylic, Vista blur-behind).
    - `enableBlurBehind()`: Real OS-level backdrop blur via DWM.
    - `setDarkMode()`: DWM dark title bar chrome.
    - `setRoundedCorners()`: Windows 11 window corner rounding preference.
    - `setBorderColor()`: DWM border accent color tinted to glass rim.
  - `LiquidGlassPainter`: Multi-layer glass surface renderer:
    - `drawGlassBackground()`: 7-layer glass: tint→sheen→refraction→rim highlight→rim shadow→border.
    - `drawAccentGlow()`: Radial focus glow for active elements.
    - `drawLiquidDrop()`: Animated teardrop/bubble with specular catch-light.
  - `LiquidGlassEffect` (QGraphicsEffect subclass): Software multi-pass Gaussian blur fallback
    for inner panels (3× box blur approximation = O(n) per pixel).
  - `LiquidGlassStyleSheet`: Complete QSS token library for: MainWindow, MenuBar, Menu,
    TabWidget/TabBar, StatusBar, ToolBar, Editor (QPlainTextEdit + scrollbars), Dialog.
- `NppLiquidGlass_Main.cpp` — Full Liquid Glass main window:
  - `BubbleOverlay`: Animated liquid bubble decoration widget (mouse-transparent, 18 bubble cap,
    sinusoidal drift, alpha lifecycle fade-in/out, 5-color palette, ~30fps timer).
  - `GlassEditorPanel`: Glass-framed editor widget with paintEvent glass rim painting.
  - `GlassStatusWidget`: Custom glass status bar with live stats (Ln/Col/Sel/Words/Encoding/EOL/Zoom/Version).
  - `GlassFindDialog`: Frameless, draggable glass find/replace dialog (regex, case, whole word).
  - `BobNppGlassWindow`: Full-featured main window with:
    - DWM glass activated in showEvent (deferred HWND).
    - Full menu parity: File/Edit/Search/View/Encoding/Language/TextFX/Macro/Run/Window/Settings/Help.
    - All TextFX operations wired with status bar feedback.
    - Glass intensity toggle (Mica/Acrylic/MicaTabbed) in Settings menu.
    - Bubble animations toggle in View menu.
    - Find/Replace: find-next, replace, replace-all, wrap-around.
    - Unsaved changes detection on tab close.
    - Multi-file open, Save All, Reload, Clone Tab.
    - HTML entity encode/decode in TextFX HTML Tidy submenu.
    - Blank operations: Trim Trailing/Leading Space, Tab→Space.
- CMakeLists.txt: Added `npp_liquid_glass` target (WIN32 subsystem) with dwmapi linkage.
- `VERSION` file at repo root (single source of truth — read at runtime by the app).
- Version propagated to both CMake targets via `file(READ VERSION)`.
- Per-monitor DPI awareness V2 enabled at startup (Windows 10 1703+ HWND-based).

## [1.0.3] - 2026-04-02
### Added
- Conducted extreme deep-dive codebase analysis mapping out OS-level capabilities.
- Documented full architectural details for JUCE, JavaFX, and Dear ImGui integration in `OS_TOOLKIT_ANALYSIS.md`.
- Updated `HANDOFF.md` with targeted instructions for the next agent cycle.

## [1.0.2] - 2026-04-02
### Added
- Created `OS_TOOLKIT_ANALYSIS.md` detailing the architectural parity with Qt6/Qt7 and the path to becoming an OS-ready toolkit.

## [1.0.1] - 2026-04-01
### Added
- Comprehensive documentation overhaul (VISION.md, MEMORY.md, DEPLOY.md, ROADMAP.md, TODO.md, IDEAS.md).
- Global AI instruction files (AGENTS.md, CLAUDE.md, GEMINI.md, GPT.md, copilot-instructions.md) standardizing development workflow.
- Established autonomous session loop protocols for multi-model AI handoffs.
- Initiated deep analysis of `bobui` towards 100% Qt6 parity.
### Added
- Conducted extreme deep-dive codebase analysis mapping out OS-level capabilities.
- Documented full architectural details for JUCE, JavaFX, and Dear ImGui integration in `OS_TOOLKIT_ANALYSIS.md`.
- Updated `HANDOFF.md` with targeted instructions for the next agent cycle.

## [1.0.2] - 2026-04-02
### Added
- Created `OS_TOOLKIT_ANALYSIS.md` detailing the architectural parity with Qt6/Qt7 and the path to becoming an OS-ready toolkit.

## [1.0.1] - 2026-04-01
### Added
- Comprehensive documentation overhaul (VISION.md, MEMORY.md, DEPLOY.md, ROADMAP.md, TODO.md, IDEAS.md).
- Global AI instruction files (AGENTS.md, CLAUDE.md, GEMINI.md, GPT.md, copilot-instructions.md) standardizing development workflow.
- Established autonomous session loop protocols for multi-model AI handoffs.
- Initiated deep analysis of `bobui` towards 100% Qt6 parity.