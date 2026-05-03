# Handoff Archive - 2026-04-04 - BTK Comparison Session

## Summary
This session focused on researching `https://github.com/robertpelloni/btk`, comparing it directly against the active Go-based `bobui` framework track, and recording actionable findings in-project.

## Research performed
- Cloned `BTK` at `master` into a temporary workspace.
- Inspected:
  - `README.md`
  - `CMakeLists.txt`
  - `.gitmodules`
  - `docs/ai/...`
  - `src/core/kernel/btkinputowner.h`
  - `src/core/kernel/btkfocustoken.h`
  - `src/gui/kernel/btkinputarbitrator.h`
- Cross-checked BobUI's active Go framework surfaces:
  - `internal/ui/widgets/webview.go`
  - `internal/ui/engine.go`
  - `internal/kernel/input_manager.go`
  - `internal/kernel/permission_manager.go`
  - `internal/kernel/user_manager.go`
  - `internal/data/history_sync.go`

## Key findings
1. BTK is substantially stronger in mature native framework depth and modular breadth.
2. BobUI is stronger in explicit framework-vs-shell separation and future-facing collaboration framing.
3. BTK has already begun formalizing BobUI-like ownership concepts through `BtkInputOwner`, `BtkFocusToken`, and `BtkInputArbitrator`.
4. BobUI currently has the cleaner modern lightweight WebView bridge abstraction, even though BTK has a much larger legacy web stack.
5. The right relationship is not competition; it is division of labor:
   - BTK as native framework mainline
   - BobUI as rapid idea incubator and parity/abstraction lab

## Repo documentation updates made
- Added `docs/ai/design/2026-04-04-bobui-vs-btk-comparison.md`
- Updated roadmap, todo, memory, vision-adjacent planning notes, changelog, version, and handoff to reflect the comparison and resulting strategy.

## Validation
- `go test ./internal/...` passed.
- `go build -buildvcs=false .` succeeded.

## Git hygiene
- Intentionally avoided staging the giant unrelated untracked C++/rename tree.
- Only session-relevant documentation and metadata changes should be committed.

## Recommended next steps
1. Implement runtime-integrated `ScrollBar` and `ScrollView` interaction in BobUI.
2. Add touch/swipe primitives in the Go widget baseline.
3. Evolve the Go `WebView` from contract-only bridge behavior toward executable runtime semantics.
4. Keep using BTK comparison work to clarify which ideas belong in BobUI as experiments versus BTK as long-term native framework API.
