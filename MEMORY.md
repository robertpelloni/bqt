# OmniUI / BobQ Internal Memory & Codebase Observations

- **Naming Conventions:** The core framework was renamed from `bobui` to `bobq`. Internal namespace and folder restructurings have taken place. Do not blindly replace `Qt` across the tree; doing so breaks the Qt build scripts. Instead, `bobq` wrappers and namespaces sit atop the modified Qt base.
- **Event Loops & Rendering:** The codebase contains multiple rendering strategies. With the introduction of JUCE and Ultimate++, we face multiple event loops. Future agents must carefully bind these loops to a singular BobQ master clock/loop, likely via the `OmniMasterClock` component.
- **Go Port Status:** A port resides in `internal/` and `main.go`. It currently uses `gioui.org` but the goal is pure BobQ CGO/Go parity.
- **Submodules:**
  - `submodules/juce`: Used for audio/DSP and plugin widget parity.
  - `submodules/ultimatepp`: Used for alternative C++ layout patterns.
- **CMake:** The primary build system relies on `CMakeLists.txt` heavily mapped to Qt's internal build macros (`qt_internal_qtbase_build_repo`). Modifying it requires extreme care.
