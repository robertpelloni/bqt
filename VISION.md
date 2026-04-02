# Project Vision: BobUI / OmniUI

## Ultimate Goal
The ultimate goal of the `bobui` project is to achieve **absolute, comprehensive, 100% feature and function parity with Qt6**, serving as an Omni-Workspace UI framework. 

This encompasses:
- Full support for Qt Core, Gui, Widgets, QML, and all standard Qt6 modules.
- A modernized, robust, and elegant design under the `OmniUI` umbrella.
- Seamless cross-platform compilation and deployment leveraging the existing CMake infrastructure.
- Complete documentation of all functionality, represented comprehensively in the UI where applicable.

## Design Philosophy
1. **Uncompromising Parity:** Every API, class, and tool available in Qt6 must have a direct, fully functional equivalent in BobUI.
2. **Robustness & Elegance:** Code must be extremely robust, well-documented, configurable, and free of regressions. Refactoring for elegant design is paramount.
3. **Comprehensive UI:** Any feature implemented on the backend must be completely wired up and represented in the frontend/UI, with tooltips, descriptions, and manual documentation.
4. **Autonomous Expandability:** The codebase must be highly structured to allow LLM agents (Gemini, Claude, GPT, Jules) to autonomously analyze, implement, test, and document features in a continuous loop.
5. **No Half-Measures:** There should be no "partially finished" code. All features are fully tested, polished, and robust before moving to the next.

## Long-Term Direction
- Fully integrate the `OmniUI` system to provide a superior, AI-first developer experience.
- Combine redundant functionalities found in referenced submodules/libraries to create the most complete UI framework possible.
- Continuously sync with upstream sources while protecting local feature branches and avoiding regressions.