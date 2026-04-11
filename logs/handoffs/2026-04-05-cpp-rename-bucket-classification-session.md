# Handoff Archive - 2026-04-05 - C++ Rename Bucket Classification Session

## Summary
This session converted the previously documented side-by-side migration strategy into a concrete bucket classification using the observed rename-artifact distribution from the polluted legacy C++ tree.

## What changed
- Added `docs/ai/design/2026-04-05-cpp-rename-bucket-classification.md` documenting:
  - first-pass path-distribution counts from the rename explosion,
  - canonical vs generated vs duplicate classification rules,
  - bucket-by-bucket risk assessment,
  - recommendation to start with `cmake/` package/export compatibility surfaces.
- Added `docs/ai/implementation/2026-04-05-first-cpp-compatibility-bucket-plan.md` documenting the first actual implementation bucket and its guardrails.
- Updated roadmap/todo/memory/handoff/version/changelog to record the new decision.

## Findings
- The rename explosion is heavily concentrated in tests, CMake/package files, corelib support surfaces, and documentation/assets.
- This confirms that a blind global rename would create huge churn in non-canonical or low-value areas before improving side-by-side buildability.
- The safest first real C++ migration bucket is `cmake/` plus package/export compatibility naming, not deep runtime source rewrites.
- Tests, docs, qdoc assets, third-party code, and broad GUI/widgets internals should remain deferred for now.

## Validation
- `go test ./internal/...` passed.
- `go build -buildvcs=false .` succeeded.

## Recommended next steps
1. Compare tracked top-level `cmake/` files against renamed copies and mark canonical inputs.
2. Define additive BobUI package/export compatibility surfaces.
3. Implement only the smallest reviewable `cmake/` compatibility slice.
4. Continue avoiding any repo-wide `qt` -> `bobui` replacement.
