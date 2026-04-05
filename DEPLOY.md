# Deployment Instructions

## Scope
This file covers the `bobui` framework repository.

The interactive shell / desktop experience belongs to the adjacent `bobfilez` project.

## Verified Go Baseline
### Commands that currently work
```bash
go test ./internal/...
go build -buildvcs=false .
```

## Go Runtime Requirements
- Go toolchain matching `go.mod`
- Gio-compatible platform support for the UI runtime
- The Go path now opens a real Gio window/frame loop in `internal/ui/engine.go`
- No external browser runtime is currently required for the verified Go `WebView` baseline; the current bridge execution model remains local and compile-safe

## C++ / BobUI Track
The repository still contains the older C++ / BobUI / OmniUI track in `OmniUI/`.
Treat it as a separate implementation path from the Go tree.

Current migration policy for the C++ track:
- do not perform a global `qt` -> `bobui` replacement,
- classify rename artifacts before merging them,
- start with additive `cmake/` package/export compatibility surfaces,
- preserve the verified Go baseline while the C++ compatibility story is clarified.

Current additive compatibility files:
- `cmake/BobUICompatibilityHelpers.cmake`
- `cmake/BobUI6Config.cmake`
- `cmake/BobUIConfig.cmake`
- `cmake/BobUI6CoreConfig.cmake`
- `cmake/BobUICoreConfig.cmake`
- `cmake/BobUI6WidgetsConfig.cmake`
- `cmake/BobUIWidgetsConfig.cmake`

Current additive export wiring:
- `cmake/QtBaseGlobalTargets.cmake` now publishes additive `BobUI6/` and `BobUI/` package directories alongside the canonical `Qt6/` package directory.
- `cmake/QtBobUIHelpers.cmake` isolates the BobUI package-publication logic.
- `cmake/tests/bobui_package_forwarding_smoke.cmake` provides an install-layout-style forwarding validation path.
- `cmake/tests/bobui_export_publication_configure_smoke.cmake` provides configure-time publication validation.

These changes preserve the canonical Qt package surface while layering BobUI discovery on top.

## CI/CD
Current workflows exist for:
- BobUI/C++ native builds
- BobUI/C++ WASM builds
- Go WASM builds

The Go CI pipeline should be updated to explicitly run the verified Go commands listed above.

## Release Verification
```bash
python scripts/check_release.py
```
