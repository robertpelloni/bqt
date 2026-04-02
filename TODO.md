# Task List / TODO

## Immediate Tasks
- [ ] **Submodules:** Create `SUBMODULE_DASHBOARD.md` outlining all submodules, versions, dates, build numbers, and project structure explanation.
- [ ] **Git Operations:** Pull upstream changes (if applicable), merge all local feature branches into main intelligently (avoiding feature loss), and push.
- [ ] **Codebase Analysis:** Deeply scan `OmniUI` and the rest of `bobui` to find partially finished code, missing UI hooks, or unpolished features.
- [ ] **Testing:** Run existing test suites to find failing/incomplete areas.

## Pending Features
- [ ] **OmniApplication Class:** Implement `OmniApp.h` and `OmniApp.cpp` to wrap `QApplication` and initialize JUCE (`initializeJuce()`), load main QML/JS source (`loadMainSource()`).
- [ ] **JuceWidget:** Implement a `QWidget` subclass that creates a native window handle and attaches a `juce::Component`.
- [ ] **TypeScript/QML Bindings:** Implement the C++ side of the TypeScript-to-QML bindings for Core Widgets (`Button`, `Slider`, etc.).
- [ ] **WASM Integration:** Ensure the `CMakeLists.txt` for `OmniUI` handles Emscripten builds properly.
- [ ] **OmniAudio:** Implement `AudioProcessor`, `AudioGraph`, `AudioSource`, etc., bridging JUCE DSP to QML.

## Documentation Maintenance
- [ ] Ensure all referenced submodules are documented and linked in the global reference.
- [ ] Consistently update `HANDOFF.md` at the end of this session.