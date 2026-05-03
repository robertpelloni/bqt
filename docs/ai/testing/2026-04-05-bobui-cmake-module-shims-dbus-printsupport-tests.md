# BobUI CMake Module Shims Tests for DBus and PrintSupport

## Date
2026-04-05

## Scope
This note documents validation for the DBus and PrintSupport BobUI module-package shims.

Affected files:
- `cmake/BobUI6DBusConfig.cmake`
- `cmake/BobUIDBusConfig.cmake`
- `cmake/BobUI6PrintSupportConfig.cmake`
- `cmake/BobUIPrintSupportConfig.cmake`
- `cmake/tests/bobui_package_forwarding_smoke.cmake`
- `cmake/tests/bobui_export_publication_configure_smoke.cmake`

## Validation performed

### 1. Package forwarding smoke test
Command:
```bash
cmake -P cmake/tests/bobui_package_forwarding_smoke.cmake
```

Verified:
- existing BobUI umbrella/Core/DBus/Gui/Network/OpenGL/OpenGLWidgets/Sql/Widgets/Xml forwarding still works,
- direct `BobUI6DBus` / `BobUIDBus` package lookup works,
- direct `BobUI6PrintSupport` / `BobUIPrintSupport` package lookup works,
- expected BobUI compatibility targets exist for DBus and PrintSupport.

### 2. Publication configure smoke test
Command:
```bash
cmake -P cmake/tests/bobui_export_publication_configure_smoke.cmake
```

Verified:
- the publication helper emits DBus module-package compat files,
- the publication helper emits PrintSupport module-package compat files,
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
The BobUI CMake compatibility path now spans umbrella forwarding/publication plus direct module-package slices for Core, DBus, Gui, Network, OpenGL, OpenGLWidgets, PrintSupport, Sql, Widgets, and Xml.
