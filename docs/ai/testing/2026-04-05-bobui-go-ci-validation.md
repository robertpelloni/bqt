# BobUI Go CI Validation

## Date
2026-04-05

## Scope
This note documents the new CI validation path for the verified Go baseline and the additive BobUI CMake compatibility surface.

Affected files:
- `.github/workflows/go_wasm.yml`
- `cmake/tests/bobui_full_compatibility_validation.cmake`
- `cmake/tests/bobui_qtbase_native_configure_preflight.cmake`

## Goal
Ensure CI runs the exact verified Go commands and the consolidated BobUI compatibility validation entry point.

## What changed
The Go workflow now includes a `validate-framework` job on `ubuntu-latest` that runs:
```bash
go test ./internal/...
go build -buildvcs=false .
cmake -DBOBUI_SKIP_NATIVE_CONFIGURE=ON -P cmake/tests/bobui_full_compatibility_validation.cmake
```

The existing Go WASM build now depends on that validation job.

## Why the native skip flag is used in CI
The stronger native qtbase configure probe is valuable, but it is environment-dependent.

For now, CI explicitly sets:
```bash
-DBOBUI_SKIP_NATIVE_CONFIGURE=ON
```

This keeps the CI gate aligned with the currently verified compatibility surface while avoiding false negatives caused by native compiler/toolchain provisioning differences.

## Result
The workflow is now closer to the project’s stated verification discipline:
- exact Go baseline commands are in CI,
- the additive BobUI CMake compatibility stack has a single CI-friendly entry point,
- native configure readiness remains visible as a separate concern.

## Recommended next step
When a compiler-capable native validation environment is available in CI, consider introducing a separate non-blocking or dedicated native-configure job rather than overloading the current compatibility gate.
