# Handoff Archive - 2026-04-05 - BobUI CMake Module Shims for OpenGL and OpenGLWidgets

## Summary
This session extended the additive BobUI CMake module-package compatibility path from Sql/Xml into OpenGL and OpenGLWidgets.

## What changed
- Added direct module config shims for `BobUI6OpenGL`, `BobUIOpenGL`, `BobUI6OpenGLWidgets`, and `BobUIOpenGLWidgets`.
- Extended forwarding smoke coverage to validate direct package lookup and target creation for OpenGL and OpenGLWidgets.
- Extended publication configure smoke coverage to validate build/install-style publication for OpenGL/OpenGLWidgets compat package files.
- Added implementation/testing docs and updated session metadata.

## Validation
- `cmake -P cmake/tests/bobui_qtbase_native_configure_preflight.cmake` reported the expected skip in the current environment.
- `cmake -P cmake/tests/bobui_compatibility_helpers_mapping.cmake` passed.
- `cmake -P cmake/tests/bobui_package_forwarding_smoke.cmake` passed.
- `cmake -P cmake/tests/bobui_export_publication_configure_smoke.cmake` passed.
- `go test ./internal/...` passed.
- `go build -buildvcs=false .` succeeded.

## Findings
- The additive module-package strategy continues to scale into graphics-oriented public modules.
- OpenGL and OpenGLWidgets deepen BobUI package identity in a realistic, consumer-facing area without requiring unsafe broad rename work.
- The migration path remains safer and more credible than a blind repo-wide `qt` -> `bobui` replacement.

## Recommended next steps
1. Re-run the qtbase-native configure preflight in a compiler-capable environment when available.
2. Extend BobUI module-package shims only to other high-value public modules in small verified slices.
3. Continue avoiding any blind repo-wide `qt` -> `bobui` replacement.
