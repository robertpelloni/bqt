# OmniUI Deployment Guide

## Prerequisites
- BobUI 6.5 or higher installed.
- CMake 3.16 or higher.
- A C++20 compatible compiler (GCC 11+, Clang 14+, MSVC 2022+).
- Node.js 18+ (for the CLI).

## Local Development Build
1. Clone the repository.
2. Initialize submodules:
   ```bash
   git submodule update --init --recursive
   ```
   *(Note: The JUCE dependency is currently mocked in `deps/juce/juce_mock.cpp` for simulation purposes. A real deployment requires replacing this with the actual JUCE repo).*
3. Create a build directory:
   ```bash
   mkdir build && cd build
   ```
4. Run CMake:
   ```bash
   cmake ../OmniUI/apps/OmniOmega
   ```
5. Compile:
   ```bash
   cmake --build . -j$(nproc)
   ```

## Cloud / WebAssembly Deployment (Planned)
To deploy OmniUI apps to the web:
1. Ensure Emscripten is installed and activated.
2. Run CMake with the Emscripten toolchain:
   ```bash
   emcmake cmake ../OmniUI/apps/OmniOmega
   ```
3. Host the resulting `.wasm` and `.js` files on a static server.

## Node.js CLI Installation
To use the `omni` CLI globally:
```bash
cd OmniUI/cli
npm install -g .
```
You can now run `omni <command>` from anywhere.
