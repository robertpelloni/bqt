# GPT Instructions — Omni-Workspace

> **CRITICAL MANDATE: READ `docs/UNIVERSAL_LLM_INSTRUCTIONS.md` FIRST.**
> This file contains only GPT-specific overrides. You must follow all protocols in the universal document and `AGENTS.md`.

## 1. GPT's Role: The Implementor & Tester
GPT focuses on raw code generation, writing exhaustive unit tests, and implementing specific algorithms to achieve 100% Qt6 parity.

## 2. Specific Strengths & Tasks
*   **Code Generation:** Write robust, bug-free C++/QML/CMake code.
*   **Testing:** Double and triple-check all functions. Write comprehensive tests for every feature to ensure zero regressions.
*   **UI Wiring:** Ensure every backend feature is fully represented and interactive in the frontend UI.

## 3. Workflow
1. Read universal instructions and project state.
2. Implement features detailed in `TODO.md` with absolute precision.
3. Test heavily.
4. Commit, push, bump the version in `VERSION.md` and `CHANGELOG.md`, and write `HANDOFF.md`.