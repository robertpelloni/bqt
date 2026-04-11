# BobUI CMake Publication Helper

## Date
2026-04-05

## Scope
This note documents the extraction of the BobUI package-publication logic into a dedicated helper file so it can be validated through a real configure-time smoke test.

Affected files:
- `cmake/QtBobUIHelpers.cmake`
- `cmake/QtBaseGlobalTargets.cmake`
- `cmake/tests/bobui_export_publication_configure_smoke.cmake`

## Goal
Strengthen validation of the additive BobUI export-wiring path by making the publication logic independently testable in a native configure flow.

## What changed

### 1. Extracted publication logic into `QtBobUIHelpers.cmake`
The function `qt_internal_install_bobui_compat_package(...)` now lives in:
- `cmake/QtBobUIHelpers.cmake`

This helper remains responsible for:
- computing BobUI package build/install directories,
- copying BobUI compatibility source files into the package directories,
- generating `BobUI*ConfigVersion.cmake` files,
- copying the compatibility package files into build-tree package directories in prefix-style scenarios.

### 2. Simplified `QtBaseGlobalTargets.cmake`
`QtBaseGlobalTargets.cmake` now:
- includes `QtBobUIHelpers.cmake`,
- calls the helper for `BobUI6` and `BobUI`,
- tracks the helper file via `_qt_internal_append_cmake_configure_depends(...)`.

This keeps the top-level package-generation path cleaner and makes the BobUI publication logic more maintainable.

### 3. Added configure-time publication smoke validation
`cmake/tests/bobui_export_publication_configure_smoke.cmake` now:
- creates a temporary CMake project,
- copies the BobUI helper/shim files into a local `cmake/` directory,
- defines minimal `qt_path_join(...)` and `qt_copy_or_install(...)` test stubs,
- calls `qt_internal_install_bobui_compat_package(...)` for `BobUI6` and `BobUI`,
- asserts that the expected config/helper/version files are produced in both build-style and install-style directories,
- verifies that the generated version file contains the expected version string.

## Why this matters
This is stronger validation than helper-only or forwarding-only tests because it exercises the actual package-publication function under a real configure invocation.

It does not claim full qtbase build verification, but it does prove that the publication helper itself behaves coherently in a realistic CMake configuration flow.

## Verified
```bash
cmake -P cmake/tests/bobui_compatibility_helpers_mapping.cmake
cmake -P cmake/tests/bobui_package_forwarding_smoke.cmake
cmake -P cmake/tests/bobui_export_publication_configure_smoke.cmake
go test ./internal/...
go build -buildvcs=false .
```

## Recommended next step
Continue expanding validation around the BobUI CMake compatibility path in small, testable increments. The next likely target is either:
- stronger qtbase-native configure/install validation, or
- similarly additive BobUI module-package publication where it can remain isolated and reversible.
