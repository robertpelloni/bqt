# C++ Rename Safety Rules

## Date
2026-04-05

## Purpose
Provide explicit implementation rules for any future C++ `Qt` / `qt` / `Q*` to `BobUI` / `bobui` migration work.

## Rules
1. **Never stage the giant unrelated untracked rename tree blindly.**
2. **Never do repo-wide search/replace renames without a bucket plan.**
3. **Prefer compatibility aliases and forwarders before deep identifier rewrites.**
4. **Verify canonical vs generated files before accepting renamed copies.**
5. **Keep Go verification green while C++ migration proceeds.**
6. **Rename one bucket/module at a time.**
7. **Document every bucket before implementation and after validation.**
8. **Avoid third-party and mirrored code until framework-owned buckets are stable.**

## Safe Bucket Order
1. Docs/planning
2. Package/CMake/export surface
3. Public alias headers
4. Internal framework-owned helpers
5. Narrow module-by-module source migration
6. Test/docs/assets cleanup

## Unsafe Early Targets
- generated support files
- qdoc mass renames
- broad test asset renames
- third-party trees
- deep QML/Quick internals without compatibility layers

## Verification Expectation
Every migration bucket must preserve at minimum:
```bash
go test ./internal/...
go build -buildvcs=false .
```

C++ verification should be introduced bucket-by-bucket once the canonical tracked C++ path is clarified and the rename noise is separated from real source.
