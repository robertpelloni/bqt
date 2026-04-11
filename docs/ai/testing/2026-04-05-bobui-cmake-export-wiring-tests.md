# BobUI CMake Export Wiring Tests

## Date
2026-04-05

## Scope
This note documents validation for the additive BobUI CMake export-wiring step.

Affected files:
- `cmake/QtBaseGlobalTargets.cmake`
- `cmake/BobUICompatibilityHelpers.cmake`
- `cmake/tests/bobui_compatibility_helpers_mapping.cmake`
- `cmake/tests/bobui_package_forwarding_smoke.cmake`

## Validation performed

### 1. Helper mapping test
Command:
```bash
cmake -P cmake/tests/bobui_compatibility_helpers_mapping.cmake
```

Verified:
- BobUI package-name mapping still behaves as intended,
- forwarded `find_package(...)` argument handling remains stable.

### 2. End-to-end package forwarding smoke test
Command:
```bash
cmake -P cmake/tests/bobui_package_forwarding_smoke.cmake
```

Verified:
- a fake prefix containing `Qt6`, `Qt6Core`, and `Qt6Widgets` packages can be consumed through the BobUI shims,
- `find_package(BobUI6 REQUIRED COMPONENTS Core Widgets)` succeeds,
- `find_package(BobUI REQUIRED COMPONENTS Core Widgets)` succeeds,
- the expected compatibility targets are created.

### 3. Go baseline regression check
Commands:
```bash
go test ./internal/...
go build -buildvcs=false .
```

Verified:
- the additive CMake export-wiring work did not disturb the verified Go baseline.

## Result
All validation passed.

## Key finding
The BobUI CMake migration path now has both:
- helper-level validation, and
- install-layout-style forwarding validation.

That is a materially stronger position than a source-tree-only shim layer.

## Remaining gaps
- no full native qtbase configure/build validation was completed in this session,
- install/export wiring currently covers the top-level BobUI package layer, not every module-specific package.
