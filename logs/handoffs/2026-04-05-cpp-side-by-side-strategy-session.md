# Handoff Archive - 2026-04-05 - C++ / Go Side-by-Side Strategy Session

## Summary
This session translated the user's broad rename goal into a safe migration strategy instead of performing a destructive global `qt` → `bobui` replacement.

## What changed
- Added `docs/ai/design/2026-04-05-cpp-go-side-by-side-migration-strategy.md`.
- Added `docs/ai/implementation/2026-04-05-cpp-rename-safety-rules.md`.
- Updated roadmap/todo/memory/handoff/version/changelog to reflect the staged side-by-side migration strategy.

## Key findings
- A global rename would be high-risk and likely break side-by-side compile goals.
- The correct path is compatibility-first, bucketed migration.
- The active Go track should remain the verified anchor while the C++ tree is migrated cautiously.
- The giant untracked rename tree must be treated as a separate cleanup/classification project, not bulk-staged as implementation progress.

## Validation
- `go test ./internal/...` passed.
- `go build -buildvcs=false .` succeeded.

## Recommended next steps
1. Classify untracked C++ rename artifacts as canonical vs generated vs duplicate.
2. Choose one safe package/export/header compatibility bucket for actual C++ implementation work.
3. Continue preserving verified Go progress while C++ migration proceeds in slices.
