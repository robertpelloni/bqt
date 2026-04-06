# BobUI CMake Module Shims Tests for Gui and Network

## Date
2026-04-05

## Scope
This note documents validation for the Gui and Network BobUI module-package shims.

Affected files:
- `cmake/BobUI6GuiConfig.cmake`
- `cmake/BobUIGuiConfig.cmake`
- `cmake/BobUI6NetworkConfig.cmake`
- `cmake/BobUINetworkConfig.cmake`
- `cmake/tests/bobui_package_forwarding_smoke.cmake`
- `cmake/tests/bobui_export_publication_configure_smoke.cmake`

## Validation performed

### 1. Package forwarding smoke test
Command:
```bash
cmake -P cmake/tests/bobui_package_forwarding_smoke.cmake
```

Verified:
- existing BobUI umbrella/Core/Widgets forwarding still works,
- direct `BobUI6Gui` / `BobUIGui` package lookup works,
- direct `BobUI6Network` / `BobUINetwork` package lookup works,
- expected BobUI compatibility targets exist for Gui and Network.

### 2. Publication configure smoke test
Command:
```bash
cmake -P cmake/tests/bobui_export_publication_configure_smoke.cmake
```

Verified:
- the publication helper emits Gui module-package compat files,
- the publication helper emits Network module-package compat files,
- expected config/helper/version files exist in build-style and install-style directories.

### 3. Regression checks
Commands:
```bash
cmake -P cmake/tests/bobui_compatibility_helpers_mapping.cmake
go test ./internal/...
go build -buildvcs=false .
```

Verified:
- helper mapping remains stable,
- the verified Go baseline remains green.

## Result
All validation passed.

## Key finding
The BobUI CMake compatibility path now spans:
- umbrella package forwarding/publication,
- Core/Widgets direct module-package shims,
- Gui/Network direct module-package shims.

This keeps moving the C++ side toward BobUI identity without broad unsafe renaming.
