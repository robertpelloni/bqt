# OmniUI Dependency & Submodule Dashboard

## Internal Module Structure
The OmniUI framework is structured into a massive core C++ library (`OmniUICore`) containing 30 interconnected physics, simulation, and hardware modules, exposed to QML via `OmniQmlRegister.h`.

| Module Category | Core Path (`core/src/`) | QML UI Path (`apps/` & `examples/`) | Status |
| :--- | :--- | :--- | :--- |
| **System Kernel** | `kernel/`, `system/` | `OmniKernelMon/`, `OmniDash/` | Refactored (v41.0) |
| **Cosmology** | `genesis/`, `void/`, `universe/`| `OmniGenesisApp/`, `OmniVoidWatcher/`, `OmniGodMode/` | Refactored (v41.0) |
| **Astrophysics** | `space/`, `galactic/` | `OmniSpaceControl/`, `OmniCiv/` | Refactored (v41.0) |
| **Quantum/Nano** | `quantum/`, `nano/` | `OmniQuantumIDE/`, `OmniNanoFactory/` | Refactored (v41.0) |
| **Metaphysics** | `multiverse/`, `spirit/`, `time/`| `OmniMultiverseExplorer/`, `OmniSpiritGuide/`, `OmniTimeTraveler/`| Refactored (v41.0) |
| **Cybernetics** | `singularity/`, `bci/` | `OmniSingularityControl/`, `OmniMind/` | Refactored (v41.0) |
| **Hardware** | `embedded/`, `vr/` | `OmniHardware/`, `OmniVRLauncher/` | Refactored (v41.0) |
| **Engineering** | `ml/`, `net/`, `web3/` | `OmniMLTrainer/`, `CollabSpace/`, `Web3Demo/` | Refactored (v41.0) |
| **Graphics/DSP** | `3d/`, `audio/`, `script/`, `nodes/` | `HelloOmni/`, `OmniSynth/`, `OmniScribe/`, `OmniStudio/`| Refactored (v41.0) |
| **Nexus Core** | `omega/` | `OmniOmega/` | Refactored (v41.0) |

## External Dependencies (Submodules)
OmniUI abstracts massive amounts of hardware and rendering complexity using industry-standard submodules.

1. **Qt 6 Framework**
   - **Version:** ^6.5.0
   - **Location:** System-installed (Resolved via `find_package(Qt6)`)
   - **Role:** Provides `QObject` meta-system, signal/slot IPC, `QJSEngine`, and the entire `QML`/`QtQuick` UI rendering pipeline.
   - **Notes:** Currently relies on `Qt6::Core`, `Qt6::Gui`, `Qt6::Qml`, and `Qt6::Quick`.

2. **JUCE Audio Framework**
   - **Version:** v7.0.0 (Mocked for current phase)
   - **Location:** `OmniUI/deps/juce/`
   - **Role:** Handles deep C++ cross-platform audio DSP routing and low-latency ASIO/CoreAudio interfacing for the `OmniSynth` module.
   - **Notes:** Currently utilizing a `juce_mock.h` stub in `deps/juce` to allow the QML integration layer to compile without the massive JUCE payload. **TODO:** Replace stub with actual `git submodule add https://github.com/juce-framework/JUCE.git` in future architecture phases.

3. **CMake Build System**
   - **Version:** >= 3.16
   - **Location:** Global `CMakeLists.txt`
   - **Role:** Orchestrates the compilation of `OmniUICore` as a static library, and links it against the 25+ independent `main.cpp` entry points in the `apps/` and `examples/` directories.

## Project Structure Overview
```
OmniUI/
├── CMakeLists.txt          # Master build orchestrator
├── VERSION                 # Current active framework version (v41.1.0)
├── MANUAL.md               # Exhaustive documentation for every C++ module and UI capability
├── ROADMAP.md              # Long-term strategic goals (e.g. The Nexus Integration Phase)
├── HANDOFF.md              # Detailed LLM agent-to-agent session summaries
├── core/                   # The C++ Backend Engine
│   ├── include/            # Headers defining Q_PROPERTIES and Q_INVOKABLES
│   │   └── OmniQmlRegister.h # The master Qt Meta-Object registration file
│   └── src/                # Implementation of math, physics, and state machine loops
├── apps/                   # The primary QML user interfaces
│   ├── OmniGodMode/        # Example app wrapping the OmniUniverse physics module
│   ├── OmniMLTrainer/      # Example app wrapping the OmniNeuralNetwork module
│   └── ...                 # 20+ other specific dashboard applications
├── examples/               # Sandbox applications testing specific edge cases
│   ├── PhysicsLab/         # Tests AABB 2D Collision physics and bouncing
│   └── Web3Demo/           # Tests mempool and simulated JSON RPC routing
├── cli/                    # Experimental JS scaffolding scripts (Node.js)
└── deps/                   # External C++ submodules (JUCE mock)
```
