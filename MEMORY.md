# Codebase Memory and Observations

## High-Confidence Findings
- `bobui` should be treated as the **framework/kernel**, not the full shell/desktop product. The shell belongs to `bobfilez`.
- The repository now contains **both** a large C++/BobUI/OmniUI codebase and a large experimental Go port.
- The Go port is **not fully build-verified**. A direct `go test ./internal/...` currently exposes package cycles and incomplete integrations.
- The C++ tree contains many exploratory components; some are stubs or partial implementations intended as architectural placeholders.
- Documentation drift is severe: several files claim total completion, while the actual code still needs stabilization and verification.

## Verified Technical Problems
- Go package cycles exist, especially around `internal/net`, `internal/kernel`, `internal/vm`, and `internal/ui`.
- Some Go files use placeholder or invalid APIs and need compile-focused cleanup.
- Some docs still describe `bobui` as a full OS shell instead of a framework consumed by `bobfilez`.
- `VERSION.md` and `CHANGELOG.md` became inconsistent due prior uncontrolled bumps.

## Design Preferences Going Forward
- Treat previous large-scale additions as **experimental until compiled**.
- Prefer fixing compile/runtime integrity over adding new speculative layers.
- Preserve interesting ideas, but move them to `IDEAS.md` unless or until implemented and verified.
- Keep `bobui` focused on framework parity and multiplayer foundations.

## Immediate Priorities
1. Stabilize Go package graph.
2. Normalize docs to actual scope.
3. Verify build/test status incrementally.
4. Implement missing framework features only after baseline integrity is restored.

## 2026-04-04 BTK Comparison Findings
- The active Go `bobui` framework track is much smaller than `BTK` and should be treated as an experimental, fast-iteration framework lane rather than a mature native framework replacement today.
- `BTK` is substantially stronger in native framework breadth, module depth, packaging, and staged compatibility/migration strategy.
- `bobui` remains stronger in explicit framework-vs-shell separation, collaboration framing, and lightweight modern bridge-contract experimentation.
- `BTK` has already started formalizing BobUI-inspired ownership semantics in stronger C++ types such as `BtkInputOwner`, `BtkFocusToken`, and `BtkInputArbitrator`.
- The right strategic relationship is complementary, not competitive: `BTK` as native framework mainline, `bobui` as idea incubator and parity/abstraction lab.
- `bobui` should keep prioritizing verified runtime semantics, especially around `ScrollBar`/`ScrollView` interaction and `WebView` bridge execution, instead of broad but shallow surface expansion.
