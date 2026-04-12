# BobQ Short-Term TODO List

1. ~~**Build System Validation:**~~ Verify that adding JUCE and U++ into the root `CMakeLists.txt` does not conflict with the Qt/CMake internal macros.
2. **Submodule Wrapper:** Write a `BobQJuceHost` wrapper class that allows placing a JUCE component inside a BobQ `QQuickPaintedItem`.
3. ~~**Go Port Coverage:**~~ Analyze `internal/ui/` and compare it against the newly expanded C++ feature set. Document missing components.
4. ~~**Documentation Sync:**~~ Run `python scripts/generate_dashboard.py` to ensure `SUBMODULE_DASHBOARD.md` recognizes the new submodules.
5. ~~**GTK Parity Research:**~~ Identify GTK layout elements (e.g., `GtkBox`, `GtkGrid`) and ensure BobQ equivalents exist and are fully documented.
