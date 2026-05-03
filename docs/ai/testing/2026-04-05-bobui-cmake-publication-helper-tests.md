# BobUI CMake Publication Helper Tests

## Date
2026-04-05

## Scope
This note documents validation for the extracted BobUI CMake publication helper.

Affected files:
- `cmake/QtBobUIHelpers.cmake`
- `cmake/QtBaseGlobalTargets.cmake`
- `cmake/tests/bobui_export_publication_configure_smoke.cmake`

## Validation performed

### 1. Configure-time publication smoke test
Command:
```bash
cmake -P cmake/tests/bobui_export_publication_configure_smoke.cmake
```

Verified:
- the extracted publication helper can be included by a real CMake configure flow,
- `qt_internal_install_bobui_compat_package(...)` publishes expected files for both `BobUI6` and `BobUI`,
- expected files are emitted into both build-style and install-style package directories,
- generated version files contain the expected project version.

### 2. Regression checks for prior CMake coverage
Commands:
```bash
cmake -P cmake/tests/bobui_compatibility_helpers_mapping.cmake
cmake -P cmake/tests/bobui_package_forwarding_smoke.cmake
```

Verified:
- helper-level mapping still passes,
- install-layout-style package forwarding still passes.

### 3. Go baseline regression check
Commands:
```bash
go test ./internal/...
go build -buildvcs=false .
```

Verified:
- the stronger CMake validation work did not disturb the verified Go baseline.

## Result
All validation passed.

## Key finding
The BobUI CMake migration path now has three meaningful validation layers:
- helper mapping validation,
- package forwarding validation,
- publication-helper configure validation.

That is a safer and more credible migration posture than broad rename churn.
