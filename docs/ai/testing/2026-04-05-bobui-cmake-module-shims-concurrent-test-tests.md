# BobUI CMake Module Shims Tests for Concurrent and Test

## Date
2026-04-05

## Scope
This note documents validation for the Concurrent and Test BobUI module-package shims.

Affected files:
- `cmake/BobUI6ConcurrentConfig.cmake`
- `cmake/BobUIConcurrentConfig.cmake`
- `cmake/BobUI6TestConfig.cmake`
- `cmake/BobUITestConfig.cmake`
- `cmake/tests/bobui_package_forwarding_smoke.cmake`
- `cmake/tests/bobui_export_publication_configure_smoke.cmake`

## Validation performed

### 1. Package forwarding smoke test
Command:
```bash
cmake -P cmake/tests/bobui_package_forwarding_smoke.cmake
```

Verified:
- existing BobUI umbrella/Concurrent/Core/DBus/Gui/Network/OpenGL/OpenGLWidgets/PrintSupport/Sql/Test/Widgets/Xml forwarding still works,
- direct `BobUI6Concurrent` / `BobUIConcurrent` package lookup works,
- direct `BobUI6Test` / `BobUITest` package lookup works,
- expected BobUI compatibility targets exist for Concurrent and Test.

### 2. Publication configure smoke test
Command:
```bash
cmake -P cmake/tests/bobui_export_publication_configure_smoke.cmake
```

Verified:
- the publication helper emits Concurrent module-package compat files,
- the publication helper emits Test module-package compat files,
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
The BobUI CMake compatibility path now spans umbrella forwarding/publication plus direct module-package slices for Concurrent, Core, DBus, Gui, Network, OpenGL, OpenGLWidgets, PrintSupport, Sql, Test, Widgets, and Xml.
