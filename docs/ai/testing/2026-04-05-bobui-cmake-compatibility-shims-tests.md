# BobUI CMake Compatibility Shims Tests

## Date
2026-04-05

## Scope
This note documents validation for the first BobUI CMake compatibility-bucket implementation.

Affected files:
- `cmake/BobUICompatibilityHelpers.cmake`
- `cmake/BobUI6Config.cmake`
- `cmake/BobUIConfig.cmake`
- `cmake/tests/bobui_compatibility_helpers_mapping.cmake`

## Validation performed

### 1. Script-mode CMake helper validation
Command:
```bash
cmake -P cmake/tests/bobui_compatibility_helpers_mapping.cmake
```

Verified:
- `BobUI6` maps to `Qt6`
- `BobUI6Core` maps to `Qt6Core`
- `BobUICore` maps to `QtCore`
- non-BobUI package names remain unchanged
- forwarded `find_package(...)` arguments preserve version, `EXACT`, `QUIET`, `REQUIRED`, and component lists

### 2. Go baseline regression check
Commands:
```bash
go test ./internal/...
go build -buildvcs=false .
```

Verified:
- the new CMake compatibility slice does not disturb the currently verified Go framework baseline

## Result
All validation passed.

## Findings
The repository now has a real additive BobUI-facing CMake surface that can be evolved incrementally. The helper-only/script-mode test strategy is a good fit for this bucket because it validates forwarding logic without pretending that the entire native build/install/export pipeline has already been migrated.

## Remaining gaps
- the BobUI CMake shims are not yet integrated into install/export generation,
- there is not yet a native end-to-end `find_package(BobUI6 ...)` install-tree validation,
- no deep module-source renaming has been attempted, by design.
