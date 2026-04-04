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

## C++ / Qt Track
The repository still contains the older C++ / Qt / OmniUI track in `OmniUI/`.
Treat it as a separate implementation path from the Go tree.

## CI/CD
Current workflows exist for:
- Qt/C++ native builds
- Qt/C++ WASM builds
- Go WASM builds

The Go CI pipeline should be updated to explicitly run the verified Go commands listed above.

## Release Verification
```bash
python scripts/check_release.py
```
