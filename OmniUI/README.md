# OmniUI: The Universal Interface Standard (BobUI)

OmniUI is the definitive C++ UI framework for modern cross-platform development, designed to be the ultimate foundation for a next-generation, multi-user collaborative Operating System.

It combines the industrial strength of **Qt 6** with the multimedia prowess of **JUCE**, the styling of **JavaFX**, and the transcendence of **Dear ImGui**.

## Core Paradigm: The Multi-Cursor OS
OmniUI fundamentally rejects the single-user window manager limitation. By natively intercepting Win32 `RAWINPUT` and routing it into distinct Focus Trees, the framework allows infinite users to concurrently interact with the same application natively.

## Feature Ecosystem

| Feature Category | Modules | Core Capabilities |
|------------------|---------|--------------------|
| **Core UI** | OmniWidgets, OmniLayout | 100% C++ QQuickPaintedItem native rendering. |
| **Audio/DSP** | OmniAudio, OmniMidi | Direct JUCE DSP mathematical integration & MIDI hardware hooks. |
| **Data & Net** | OmniData, OmniNet | Native SQLite mapping to QVariantList & Async Websockets. |
| **Extensibility**| OmniPlugin | Runtime `.dll` / `.so` injection via QPluginLoader. |
| **Connectivity** | OmniCloud, OmniNexus | Global IPC & Master Clock Unification (Upcoming). |
| **3D & Simulation** | Omni3D, OmniPhysics | Hardware-accelerated SceneGraph integration (Upcoming). |

## Getting Started
See the [User Manual](docs/Manual.md) for full API documentation.

### Quick Start
```bash
# OmniUI targets Emscripten (WASM) and Native Desktop dynamically.
git clone --recursive https://github.com/robertpelloni/bobui
cd bobui
mkdir build && cd build
cmake -G Ninja ..
cmake --build .
```
