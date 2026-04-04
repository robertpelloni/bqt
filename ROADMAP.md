# Project Roadmap

## 1. Scope & Ownership
- [x] Clarify that `bobui` is the framework/kernel project.
- [ ] Keep shell/desktop product concerns in `bobfilez`.

## 2. Verified Build Stability (Current)
- [x] Internal Go packages compile with `go test ./internal/...`.
- [x] Root Go binary builds with `go build -buildvcs=false .`.
- [ ] Add these exact verification steps to CI.
- [ ] Continue reducing placeholder/stub logic in Go packages.

## 3. Go Port Stabilization
- [ ] Replace compile-safe placeholder widgets with real Gio implementations incrementally.
- [ ] Audit package boundaries so UI, VM, kernel, and net stay acyclic.
- [ ] Add tests for Go kernel modules where feasible.

## 4. Framework Parity Work
- [ ] Verify which parity features are truly functional vs architectural placeholders.
- [ ] Prioritize high-value framework primitives: state binding, multiplayer ownership, synchronization, plotting, markdown, tree/list views, and audio graph stability.
- [ ] Defer “grand singularity” ideas until build/runtime proof exists.

## 5. Documentation Integrity
- [ ] Continue rewriting docs to match verified state.
- [ ] Keep changelog honest about what was fixed versus what remains experimental.
