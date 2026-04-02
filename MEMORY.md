# Codebase Memory and Observations

## Ongoing Observations
- **Date:** 2026-04-01
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