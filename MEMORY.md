# Codebase Memory and Observations

## Ongoing Observations
- **Session Learnings (2026-04-01):** OmniUI is currently missing its core C++ binding layer (`OmniApp`, `JuceWidget`). It exists only as a CLI wrapper. The C++ integration code must be built from scratch, as Qt6 parity is the goal but the bridge code is missing. There are no submodules in the project currently. The local `dev` branch is up to date with `main`, and there is no upstream remote configured, so upstream syncs cannot be performed autonomously until `upstream` is set.
- **State:** The repository `bobui` structure mirrors a qtbase clone/fork (CMakeLists.txt, configure scripts, qt-cmake wrappers, mkspecs, src, tests).
- **Core Directive:** The project requires heavy documentation, autonomous AI loops, and rigorous git/submodule maintenance alongside the primary goal of 100% Qt6 parity.
- **Submodules:** Currently, `git submodule status` did not output any submodules, implying either none are configured yet, or they are managed differently. We must continuously check and track them if added.
- **Documentation:** The project heavily relies on `.md` files to guide multi-model AI agents. Strict adherence to updating these files (especially `CHANGELOG.md` and `VERSION.md`) is required.
- **Design Preferences:**
  - Robust, fully-wired UI.
  - No orphaned backend features.
  - Comprehensive comments explaining *why* code exists.
  - Intelligent git merging (never lose feature progress).
  - Single source of truth for versions (`VERSION.md`).