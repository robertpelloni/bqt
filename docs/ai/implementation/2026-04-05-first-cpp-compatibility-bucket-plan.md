# First C++ Compatibility Bucket Plan

## Date
2026-04-05

## Goal
Define the first safe implementation bucket for C++ BobUI naming work while preserving the side-by-side Go track and avoiding a destructive repo-wide rename.

## Selected first bucket
### `cmake/` and package/export compatibility surfaces

This bucket was selected because it has the best ratio of:
- high branding/discovery leverage,
- low runtime breakage risk,
- natural compatibility-alias support,
- relevance to side-by-side compilation goals.

## Intended outcomes
This bucket should eventually enable the repository to expose BobUI-branded discovery and export surfaces while preserving compatibility with legacy Qt-oriented layout and naming where needed.

Examples of target outcomes:
- BobUI-facing package config entry points
- alias/discovery compatibility between BobUI and legacy names
- explicit package/export documentation describing coexistence
- install/export layout decisions recorded before deep source churn

## What is safe inside this bucket
Potential safe tasks:
1. Compare tracked `cmake/` sources against renamed artifact copies.
2. Identify which `cmake/` files are canonical inputs versus generated outputs.
3. Introduce new BobUI-facing package/export entry points only where they can coexist with legacy names.
4. Document exact compatibility expectations for consumers.
5. Keep changes reversible and module-scoped.

## What is not safe inside this bucket
Do not include any of the following in Bucket 1:
- global text replacement of `qt` or `Q*`
- runtime ABI/API rewrites
- broad include-path migration
- test asset renames
- qdoc/qmake mass rename work
- third-party subtree changes
- generated support-file adoption without source verification

## Proposed execution order

### Step 1 - Canonical source check
For each targeted `cmake/` file:
- identify tracked original,
- determine whether the renamed copy is a duplicate or a legitimate new compatibility surface,
- reject generated outputs as merge candidates.

### Step 2 - Define BobUI-facing compatibility contract
Write down:
- package names,
- alias target expectations,
- config/export naming,
- install-path coexistence rules,
- deprecation posture for legacy names.

### Step 3 - Implement only additive compatibility surfaces first
Prefer:
- additive alias configs,
- additive BobUI package discovery entry points,
- explicit forwarding or compatibility wrappers,
- no destructive removal of legacy names in the same bucket.

### Step 4 - Verify isolation
At minimum preserve:
```bash
go test ./internal/...
go build -buildvcs=false .
```

If practical for the chosen CMake slice, also verify:
- syntax/configuration sanity for changed CMake inputs,
- no accidental staging of unrelated rename artifacts.

### Step 5 - Document and hand off
Record:
- exact files touched,
- which renamed artifacts were rejected as non-canonical,
- compatibility guarantees introduced,
- next bucket recommendation.

## Acceptance criteria
Bucket 1 is successful only if:
1. the change set is small and reviewable,
2. no giant rename tree is staged,
3. the BobUI package/export story becomes clearer,
4. the Go verification baseline remains green,
5. follow-on source migration becomes easier rather than riskier.

## Follow-on bucket after this one
If the CMake/package/export bucket succeeds, the next likely safe bucket is:
- **public alias headers for framework-owned helper surfaces**

That would still preserve compatibility-first discipline before any broad internal source rename.

## Bottom line
The first implementation bucket should improve **identity and discovery**, not rewrite the entire C++ framework. That is the safest way to move toward the user’s end goal of clean side-by-side C++ and Go compilation.
