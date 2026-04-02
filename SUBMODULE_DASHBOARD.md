# Submodule Dashboard & Project Structure

## Submodules Status
- **Current State:** No Git submodules (`.gitmodules`) are currently configured in this repository. 
- **Tracking:** If submodules are added in the future (e.g., specific libraries or dependencies that are not managed by CMake/npm/pip directly), they will be listed here with their respective paths, versions/hashes, dates, and build numbers.

## Project Directory Structure
The repository is structured as an Omni-Workspace framework, mirroring the extensive layout of `qtbase` to maintain parity:

- `bin/`: Executables and wrapper scripts (e.g., `qt-cmake`, `qmake` wrappers).
- `build/` & `build_output/`: Standard CMake output directories for compiled binaries and intermediate artifacts.
- `cmake/`: Comprehensive CMake modules, find scripts, and configuration logic for building the entire framework cross-platform.
- `coin/`: Continuous integration configuration files.
- `config.tests/`: Feature detection scripts used during the CMake configuration phase.
- `dist/`: Packaging and distribution scripts/assets.
- `doc/` & `docs/`: Markdown files, LLM instructions, and generated help documentation.
- `examples/`: Sample applications demonstrating framework usage.
- `lib/` & `libexec/`: Compiled libraries and internal executable helpers.
- `LICENSES/`: License texts conforming to REUSE standards.
- `mkspecs/`: Compiler and platform specifications used by the legacy `qmake` system and internal build processes.
- `OmniUI/`: **The primary UI overlay and abstraction layer** where the BobUI specific modernized, robust features are developed to interface with the Qt6-equivalent core.
- `qmake/`: Legacy build system tools maintained for backwards compatibility.
- `src/`: The core C++ source code for all modules (Core, Gui, Widgets, etc.), aiming for 100% parity with Qt6.
- `tests/`: Exhaustive unit and integration tests to ensure zero regressions and perfect functional parity.
- `util/`: Miscellaneous utility scripts and developer tools.