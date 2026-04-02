# Ideas & Brainstorming

This document contains creative and constructive ideas for improving the project from various perspectives: refactoring, restructuring, porting, and concept pivoting.

## Refactoring & Restructuring
1. **CMake Modernization:** Deeply audit the `qt-cmake` scripts and `CMakeLists.txt` to ensure they use the absolute latest CMake 3.20+ paradigms (target-centric, properties over variables), discarding older Qt5/Qt6 transitional cruft.
2. **OmniUI Abstraction Layer:** Isolate `OmniUI` into a distinct architectural layer that can seamlessly wrap underlying Qt6-equivalent widgets, allowing for an easier "skinning" or "behavior override" system without modifying core widget code.
3. **AI-First Tooling integration:** Build native LLM context-gathering tools *into* the UI framework itself, so that debugging the UI can automatically generate prompts for the agents.

## Porting & Language Pivoting
1. **Rust Core Integration:** Identify highly parallel or memory-critical subsystems (e.g., event loops, rendering pipelines) and experiment with rewriting them in Rust, binding them to the C++ frontend via CXX.
2. **WASM First:** Ensure the entire framework compiles natively to WebAssembly from day one, rather than treating it as an afterthought. Create an `OmniUI-Web` target.

## Concept Pivots
1. **The "Self-Hosting" AI Engine:** Shift the framework from just being a UI toolkit to being an "OS for AI Agents," where `bobui` provides the native windowing and rendering for local, lightweight LLM nodes.
2. **Declarative UI via Markdown:** Extend QML to support generating complex UIs directly from structured Markdown or `IDEAS.md` files, allowing agents to "write" UI simply by writing docs.