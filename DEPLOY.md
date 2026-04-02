# Deployment Instructions

## Overview
BobUI utilizes a CMake-based build system, heavily inspired by and maintaining parity with Qt6.

## Prerequisites
- CMake 3.16+
- A compliant C++17/C++20 compiler (GCC, Clang, MSVC)
- Python 3 (for sync scripts if applicable)

## Standard Build Process
1. **Configure:**
   ```bash
   mkdir build && cd build
   ../configure -prefix /usr/local/bobui
   ```
   *Alternatively, use CMake directly:*
   ```bash
   cmake -B build -S . -DCMAKE_INSTALL_PREFIX=/usr/local/bobui
   ```

2. **Build:**
   ```bash
   cmake --build build --parallel
   ```

3. **Test:**
   ```bash
   ctest --test-dir build --output-on-failure
   ```

4. **Install/Deploy:**
   ```bash
   cmake --install build
   ```

## Versioning
The version is controlled by `VERSION.md`. Ensure that packaging scripts read this file dynamically rather than relying on hardcoded strings.