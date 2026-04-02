# Submodule Dashboard & Project Structure

## 1. Linked Dependencies & Submodules
- **Current State:** As of v1.6.0, `bobui` has autonomously consolidated all necessary functionality internally into the `OmniUI/omnicore/` framework. External libraries (JUCE, ImGui) are currently placed directly in `OmniUI/deps/` as mock headers or hard-linked source files, meaning there are **no active `.gitmodules`**.
- **Tracking Protocols:** Should external repos (like `itgmania` or `aios`) be added via Git submodules later in the CI/CD pipeline, they must be updated dynamically via `git submodule update --init --recursive`.

## 2. OmniUI Codebase Directory Structure
The workspace has evolved vastly beyond a simple `qtbase` clone. The primary OS framework is housed in `OmniUI/`:

### The OS Engine Core (`OmniUI/omnicore/`)
- **`include/`:** The pure C++ header declarations for the entire framework interface.
- **`src/`:** The implementation core.
    - **`OmniApp.cpp`:** The overarching application class. Holds the `QQmlApplicationEngine` and intercepts raw OS events (`notify`) for multi-cursor routing.
    - **`OmniInputManager.cpp`:** The multi-user focus tree singleton (`DeviceId -> Focused Widget`).
    - **`OmniDeveloperOverlay.cpp`:** The top-level ImGui/QPainter diagnostics layer displaying colored virtual cursors and dynamic `QMetaObject` Q_PROPERTY tooltips.
    - **`OmniNativeEventFilter.cpp`:** Deep Win32 hooks extracting `hDevice` USB handles from `WM_INPUT` messages.
    - **`OmniQmlRegistration.cpp`:** The QML API surface. Registers over 20 native C++ components directly into the Qt SceneGraph dynamically.

### Core Modules (`OmniUI/omnicore/src/`)
- **`widgets/`:** Pure C++ `QQuickPaintedItem` implementations of `OmniButton`, `OmniSlider`, `OmniCodeEditor`, `OmniCalendar`, `OmniTextField`, etc.
- **`layout/`:** `OmniDockArea` and `OmniSplitView` container logistics.
- **`data/`:** `OmniDatabase` wrapping `QSqlDatabase` and exposing tabular data directly as `QVariantList`s.
- **`net/`:** Async networking via `OmniHttpClient` and real-time streams via `OmniWebSocket`.
- **`audio/`:** Multimedia DSP integration via `OmniFilter`, `OmniGain`, `OmniMidiHandler`, `OmniSequencer`, and the `OmniJuceView` RHI bridge.
- **`plugin/`:** `OmniPluginManager` establishing runtime `QPluginLoader` injection of external `.dll` components.

### Automated Testing (`OmniUI/tests/`)
- **`omnicore/`:** Contains `QTest` executables like `tst_OmniInputManager.cpp`, which mathematically proves that `QKeyEvent`s from separate devices hit completely separate focus targets without overlap.

### CI/CD Deployment (`.github/workflows/`)
- **`build_native.yml`:** Automated CMake/Ninja builds across Ubuntu and Windows, enforcing native execution parity.
- **`build_wasm.yml`:** Automated Emscripten cross-compilation, enforcing "The Desktop app is the Web app" paradigm.
