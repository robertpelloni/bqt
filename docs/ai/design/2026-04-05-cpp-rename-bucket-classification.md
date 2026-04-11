# C++ Rename Bucket Classification

## Date
2026-04-05

## Scope
This document classifies the currently observed C++ rename-artifact pressure into migration buckets so future BobUI naming work can proceed without a destructive repo-wide `qt` -> `bobui` replacement.

It uses the current polluted working tree only as an **inventory signal**. It does **not** treat the untracked rename copies as canonical source.

## Why this document exists
The side-by-side migration strategy established that the repository must not accept a blind global rename. The next missing piece was a practical classification of where rename pressure is actually concentrated.

That classification now exists and gives us a defensible first implementation target.

## Observed distribution from the rename explosion
A first-pass path census of the untracked rename artifacts showed the following concentration areas:

| Bucket prefix | Approx count |
|---|---:|
| `tests/` | 285 |
| `src/corelib/` | 166 |
| `cmake/` | 151 |
| `src/` (mixed residual paths) | 97 |
| `src/gui/` | 91 |
| `src/widgets/` | 88 |
| `src/testlib/` | 82 |
| `src/3rdparty/` | 66 |
| `doc/` | 63 |
| `src/plugins/` | 44 |
| `src/network/` | 21 |
| `examples/` | 15 |
| `util/` | 11 |
| `bin/` | 10 |
| `libexec/` | 9 |
| `coin/` | 5 |
| `mkspecs/` | 5 |
| `internal/` | 4 |

Representative examples included:
- `cmake/BobUIAndroidHelpers.cmake`
- `src/corelib/BobUI6AndroidMacros.cmake`
- `src/gui/BobUIGui.dynlist.in`
- `src/widgets/BobUI6WidgetsMacros.cmake`
- `doc/config/exampleurl-bobui3d.qdocconf`
- `tests/auto/bic/data/BobUICore.6.0.0.linux-gcc-amd64.txt`
- `src/3rdparty/blake2/0001-mark-the-API-static-so-it-cant-be-exported-from-BobUICore.patch`

## Classification model
Each rename artifact bucket must be classified as one of the following before any implementation merge:

1. **Canonical migration candidate**
   - Framework-owned and plausibly intended to become source of truth.
   - Safe to migrate with compatibility shims.

2. **Compatibility-surface candidate**
   - Not necessarily runtime code, but high leverage for side-by-side identity.
   - Often includes CMake/package/export surfaces, alias headers, wrappers, and install naming.

3. **Generated/support artifact**
   - Files produced by generators, packaging, docs, or build tooling.
   - Should not be renamed blindly until generator/source ownership is known.

4. **Duplicate rename copy / likely non-canonical**
   - A renamed counterpart of existing tracked content with unclear source-of-truth status.
   - Must not be bulk-staged.

5. **Protected / defer bucket**
   - Third-party trees, mirrored code, mass test assets, or deeply integrated subsystems.
   - Avoid until framework-owned buckets are stable.

## Bucket-by-bucket assessment

### 1. `cmake/` — **best first compatibility bucket**
Assessment:
- High leverage.
- Mostly branding/package/export/tooling surfaces.
- Lower risk than deep source identifier churn.
- Directly relevant to the side-by-side compile goal.

Why it is a strong first target:
- BobUI identity can be surfaced in package discovery and export layers before rewriting internals.
- Compatibility aliasing is natural in CMake.
- The work can be scoped tightly.

Risk notes:
- Must distinguish tracked package/export inputs from generated config outputs.
- Must avoid accidentally adopting generated or duplicated renamed files as canonical without comparison.

### 2. `src/corelib/` — **important, but split required**
Assessment:
- Contains a mix of potentially useful package/helper CMake surfaces and deeper code-support artifacts.
- Too broad for a first direct rename bucket.

Safe sub-split:
- Framework-owned helper/build files: candidate after top-level `cmake/` bucket.
- Core runtime/API identifiers: defer until compatibility surfaces exist.

### 3. `src/gui/` and `src/widgets/` — **mixed and risky early**
Assessment:
- Contains docs, assets, dynlist inputs, module metadata, and likely real code.
- Too easy to mix harmless branding work with deep ABI/source changes.

Recommendation:
- Do not bulk-migrate.
- Revisit only after package/export and alias-header strategy is established.

### 4. `tests/` — **do not start here**
Assessment:
- Largest bucket by count.
- Mostly fixtures, snapshots, asset names, and compatibility assertions.
- Renaming tests early creates lots of churn without proving build safety.

Recommendation:
- Defer until canonical package/module names are settled.

### 5. `doc/` and docs-related paths — **safe for narrative, unsafe as source-of-truth signal**
Assessment:
- Reasonably safe for planning and narrative documentation.
- Unsafe to treat renamed qdoc/qdocconf copies as canonical without generator/source review.

Recommendation:
- Planning docs are safe.
- qdoc/qdocconf mass rename is not a good first implementation bucket.

### 6. `src/testlib/` and `src/3rdparty/` — **protected / defer**
Assessment:
- High risk.
- Contains third-party, attribution, vendored, and support material.

Recommendation:
- Do not touch in early migration.

### 7. `src/plugins/`, `src/network/`, `examples/`, `util/`, `bin/`, `libexec/`, `mkspecs/` — **secondary buckets**
Assessment:
- Each likely mixes runtime code, tooling glue, generated/support files, and branding.
- Useful later, but poor first targets.

Recommendation:
- Classify only after `cmake/` and package/export compatibility decisions are stable.

## Canonicality heuristics
Until a bucket is formally implemented, treat renamed files as non-canonical unless at least one of the following is proven:
- there is no tracked predecessor and the file is framework-owned,
- the renamed file is intentionally introduced as a new compatibility surface,
- generator/source ownership has been verified,
- a module-level comparison confirms the renamed file is the chosen source of truth.

Treat as likely non-canonical or generated when:
- the file is under `3rdparty/`, `tests/`, qdoc asset trees, or support-output directories,
- the filename suggests generated/configured output,
- the path is a renamed copy of an obviously existing Qt-era file.

## First implementation recommendation
The safest real implementation entry point is:

### **Bucket 1: CMake / package / export compatibility surface**
Scope:
- top-level framework-owned `cmake/` package naming
- alias/discovery surfaces
- install/export compatibility naming
- BobUI-facing package metadata that can coexist with legacy Qt-facing expectations

Explicitly out of scope for Bucket 1:
- deep runtime source renames
- public `Q*` type rewrites
- test asset renames
- qdoc mass migration
- third-party content
- deep GUI/widgets internals

## Why this is the correct next step
It moves the repository toward the user’s actual goal:
- both C++ and Go paths surviving side by side
- BobUI identity becoming visible in the C++ build surface
- without detonating the legacy source tree

## Bottom line
The rename explosion is not one problem; it is several different categories of problem collapsed together.

The evidence says:
- **do not** start with tests, docs, third-party, or deep source identifiers
- **do** start with `cmake/` and package/export compatibility surfaces
- **do** keep treating the giant rename tree as inventory to classify, not as content to bulk-stage
