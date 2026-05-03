# Handoff Archive - 2026-04-05 - BobUI Supported Component Matrix Session

## Summary
This session centralized the currently supported BobUI CMake component surface into an explicit manifest and added a consistency test to keep the growing package matrix aligned with the shim files and smoke coverage.

## What changed
- Added `cmake/BobUISupportedComponents.cmake`.
- Added `cmake/tests/bobui_supported_component_manifest_consistency.cmake`.
- Added design/testing docs for the supported BobUI component matrix.
- Updated session metadata to record the new manifest-backed validation step.

## Validation
- `cmake -P cmake/tests/bobui_supported_component_manifest_consistency.cmake` passed.
- `cmake -P cmake/tests/bobui_qtbase_native_configure_preflight.cmake` reported the expected compiler-visibility skip.
- Existing BobUI smoke tests passed.
- `go test ./internal/...` passed.
- `go build -buildvcs=false .` succeeded.

## Findings
- The supported BobUI package surface is now explicit rather than implicit.
- A centralized manifest plus consistency test is a better way to scale the additive compatibility layer than relying only on manual discipline.
- The migration path remains compatibility-first and avoids unsafe whole-tree rename pressure.

## Recommended next steps
1. Keep the manifest authoritative whenever new BobUI package components are added.
2. Re-run the qtbase-native configure preflight in a compiler-capable environment when available.
3. Continue avoiding any blind repo-wide `qt` -> `bobui` replacement.
