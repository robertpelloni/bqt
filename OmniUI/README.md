<<<<<<< HEAD
# OmniUI / BobUI Framework

`bobui` is the **framework/kernel** project.

It provides the reusable foundation for:
- multi-device input and ownership
- UI widgets and layouts
- media / DSP primitives
- networking / synchronization primitives
- developer tooling overlays
- cross-platform runtime experiments

The **desktop shell / operating environment** belongs to the adjacent `bobfilez` project, which consumes `bobui`.

## Current State
This repository contains two major implementation tracks:
1. **C++ / Qt / OmniUI** in `OmniUI/`
2. **Experimental Go port** in `internal/` and `main.go`

Both tracks contain substantial work. Neither should be assumed complete without build verification.

## Design Direction
BobUI aims to match or exceed practical strengths from:
- Qt 6 / Qt 7 direction
- JUCE
- JavaFX
- Dear ImGui / ImPlot

with special emphasis on:
- local multi-user interaction
- remote collaborative synchronization
- AI-aware context and automation
- high-performance desktop and WASM deployment

## Important Boundary
If you are looking for the actual OS shell, taskbar, launcher, or desktop product, that belongs in **`bobfilez`**, not in this repo.
=======
# OmniUI: The Universal Interface Standard

OmniUI is the definitive C++ UI framework for modern cross-platform development.

| Feature Category | Modules | Status |
|------------------|---------|--------|
| **Core UI** | OmniWidgets, OmniLayout | Stable (v15.0) |
| **Audio/DSP** | OmniAudio, OmniMidi | Stable (v15.0) |
| **Visual Scripting** | OmniNodes, OmniScript | Stable (v15.0) |
| **3D & XR** | Omni3D, OmniXR | Stable (v15.0) |
| **Connectivity** | OmniNet, OmniCloud | Stable (v15.0) |
| **Hardware** | OmniHardware, OmniEmbedded | Stable (v15.0) |
| **Simulation** | OmniPhysics, OmniGfx | Stable (v15.0) |
| **Gaming** | OmniGame | Stable (v15.0) |
| **System** | OmniSystem | Stable (v20.0) |

## Flagship Applications
- **OmniStudio**: Integrated Development Environment.
- **OmniSynth**: Modular Synthesizer.
- **OmniChat**: AI Assistant.
- **OmniDash**: IoT Dashboard.

## Getting Started
```bash
npm install -g omni-ui-cli
omni init my-app
cd my-app
omni dev
```

See [Manual](docs/Manual.md) for detailed guides.
>>>>>>> feature/omni-ui-framework-18001284211800334382
