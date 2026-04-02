# OmniUI: The Definitive C++ UI Standard

## Overview
OmniUI is a next-generation UI framework that combines the industrial strength of **Qt 6** with the multimedia prowess of **JUCE**. It is designed to be the "Electron Killer" for C++, offering a developer experience (DX) akin to modern web development while maintaining native performance and compiling seamlessly to WebAssembly (WASM).

## Features
- **Qt + JUCE Fusion**: Use Qt for complex layouts, styling, and accessibility, while embedding JUCE components for high-performance audio/visualizers.
- **TypeScript First**: Write your UI logic and business logic in TypeScript. OmniUI transpiles this to optimized QML/C++ bindings.
- **WebAssembly Native**: Zero-config WASM builds. Your desktop app is your web app.
- **Developer Experience**: A CLI tool `omni` that handles scaffolding, building, and hot-reloading.

## Getting Started
See the [User Manual](docs/Manual.md) for full documentation.

### Quick Start
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
