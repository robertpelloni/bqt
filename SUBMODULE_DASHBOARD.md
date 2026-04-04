# Submodule Dashboard & Project Structure

## Current Reality
- There are **no active Git submodules** configured for this repository at the moment.
- `bobui` is the **framework/kernel** project.
- The **operating shell / OS product** belongs to the adjacent `bobfilez` project.

## Major Code Areas
### C++ Framework Track
- `OmniUI/omnicore/include/`
- `OmniUI/omnicore/src/`
- `OmniUI/examples/`
- `OmniUI/tests/`

### Go Port Track
- `main.go`
- `internal/kernel/`
- `internal/net/`
- `internal/state/`
- `internal/ui/`
- `internal/audio/`
- `internal/data/`
- `internal/vm/`

## Notes
- The Go track currently contains significant experimental work and requires build-focused stabilization.
- The C++ track remains the more mature implementation path until the Go build is verified.
