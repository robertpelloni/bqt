# BobUI CMake Module Shims Tests for OpenGL and OpenGLWidgets

## Date
2026-04-05

## Scope
This note documents validation for the OpenGL and OpenGLWidgets BobUI module-package shims.

Affected files:
- `cmake/BobUI6OpenGLConfig.cmake`
- `cmake/BobUIOpenGLConfig.cmake`
- `cmake/BobUI6OpenGLWidgetsConfig.cmake`
- `cmake/BobUIOpenGLWidgetsConfig.cmake`
- `cmake/tests/bobui_package_forwarding_smoke.cmake`
- `cmake/tests/bobui_export_publication_configure_smoke.cmake`

## Validation performed

### 1. Package forwarding smoke test
Command:
```bash
cmake -P cmake/tests/bobui_package_forwarding_smoke.cmake
```

Verified:
- existing BobUI umbrella/Core/Gui/Network/Sql/Widgets/Xml forwarding still works,
- direct `BobUI6OpenGL` / `BobUIOpenGL` package lookup works,
- direct `BobUI6OpenGLWidgets` / `BobUIOpenGLWidgets` package lookup works,
- expected BobUI compatibility targets exist for OpenGL and OpenGLWidgets.

### 2. Publication configure smoke test
Command:
```bash
cmake -P cmake/tests/bobui_export_publication_configure_smoke.cmake
```

Verified:
- the publication helper emits OpenGL module-package compat files,
- the publication helper emits OpenGLWidgets module-package compat files,
- expected config/helper/version files exist in build-style and install-style directories.

### 3. Regression checks
Commands:
```bash
cmake -P cmake/tests/bobui_qtbase_native_configure_preflight.cmake
cmake -P cmake/tests/bobui_compatibility_helpers_mapping.cmake
go test ./internal/...
go build -buildvcs=false .
```

Verified:
- the native configure preflight still reports the expected environment-gated skip,
- helper mapping remains stable,
- the verified Go baseline remains green.

## Result
All validation passed.

## Key finding
The BobUI CMake compatibility path now spans umbrella forwarding/publication plus direct module-package slices for Core, Gui, Network, OpenGL, OpenGLWidgets, Sql, Widgets, and Xml.
