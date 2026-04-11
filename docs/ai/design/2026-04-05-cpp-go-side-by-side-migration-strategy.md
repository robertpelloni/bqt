# C++ / Go Side-by-Side Migration Strategy

## Date
2026-04-05

## Scope
This document defines the safe migration strategy for reducing legacy `Qt` / `qt` / `Q*` branding and structure in the C++ tree while preserving side-by-side compilation with the active Go `bobui` track.

It explicitly rejects a one-shot global text replacement.

## Problem
The repository currently contains:
- an active, verified Go `bobui` framework track under `internal/`
- a very large C++ / Qt / CopperSpice-derived tree
- a known explosion of untracked rename artifacts in the legacy C++ path

The user goal is valid:
- move the legacy C++ tree toward BobUI naming/identity
- continue the Go port/fork/translation
- keep both tracks buildable side by side

The dangerous mistake would be to attempt a repo-wide blind rename of `qt` to `bobui` immediately.

## Why a Global Rename Is Unsafe
A wholesale rename would hit all of the following at once:
- source code identifiers
- include paths
- qmake / qdoc / CMake metadata
- package names
- exported macros
- generated files and support files
- tests and test assets
- install layout and tool wrappers
- third-party and compatibility assumptions

Given the current repo state, that would almost certainly:
- destroy buildability
- stage duplicate/untracked artifacts accidentally
- blur canonical vs generated files
- make recovery and verification far harder

## Strategic Principle
**Do not rename globally. Rename in verified buckets while preserving compatibility shims.**

That means:
1. identify a bucket
2. isolate canonical source from generated/duplicated rename artifacts
3. introduce compatibility aliases/shims where needed
4. verify the bucket
5. commit that bucket only
6. move to the next bucket

## Side-by-Side Goal
The desired end state is not “Go replaces C++ immediately.”

The desired near-term state is:
- the C++ path continues compiling
- the Go path continues compiling
- BobUI-branded surfaces become more explicit over time
- compatibility remains long enough to avoid breaking the repo during migration

## Safe Migration Phases

### Phase 0 - Freeze unsafe bulk rename behavior
Rules:
- do not bulk-add the huge untracked rename tree
- do not perform blind repo-wide `qt` → `bobui` replacements
- do not merge generated rename artifacts before identifying their source-of-truth status

### Phase 1 - Inventory and classify rename pressure
Create explicit buckets such as:
- branding/docs bucket
- package/cmake export bucket
- public header alias bucket
- source file rename bucket
- test/data asset bucket
- generated/support-file bucket

Each bucket should answer:
- is this canonical source?
- is this generated?
- is it safe to rename now?
- what compatibility shim is required?

### Phase 2 - Compatibility-first C++ layer
Introduce BobUI-facing compatibility without immediately deleting legacy names.

Examples of safe compatibility work:
- BobUI-branded CMake/package config files
- alias targets
- forwarder headers
- namespace/type aliases where feasible
- install-layout compatibility

Goal:
- expose BobUI identity first
- preserve legacy compilation paths long enough to avoid destabilization

### Phase 3 - Public API / include bucket migration
Migrate in small public-facing slices:
- one module at a time
- headers before deep source churn where possible
- explicit compatibility notes for each module

Candidates for early migration:
- framework-owned helper headers
- non-generated support headers
- package/export-facing include surfaces

Avoid early aggressive migration of:
- deeply integrated QML/Quick stacks
- generator-sensitive files
- broad third-party mirrors

### Phase 4 - Internal source bucket migration
Once a bucket has:
- compatibility shims
- clear source-of-truth status
- module-level verification plan

Then rename deeper internal sources in that module.

### Phase 5 - Tooling/docs/test asset cleanup
After canonical code is stable, reconcile:
- qdoc/qmake/cmake docs
- test assets and fixture names
- wrappers and tool entry points
- generated/support duplication

## Rename Buckets Recommended First

### Bucket A - Documentation and planning
Safe first bucket because it changes narrative, not runtime.

Examples:
- migration docs
- parity docs
- repo hygiene docs
- module planning documents

### Bucket B - CMake/package compatibility surface
Very high leverage.

Goal:
- make BobUI identity visible in package discovery/export
- preserve legacy compatibility simultaneously

### Bucket C - Public alias headers for framework-owned helpers
Goal:
- introduce BobUI-facing entry points without forcing immediate deep refactors

### Bucket D - Narrow module-by-module internal migration
Only after A/B/C are under control.

## What Not To Rename Early
Do **not** start by renaming all of these globally:
- every `Q*` symbol
- every `qt_` helper
- every qdoc/qmake snippet
- every test asset filename
- every generated support file
- third-party mirrored source trees

These should only be touched after a module-level decision and verification plan exists.

## Go Track Relationship
The Go track should continue advancing independently and verifiably.

The Go track already provides:
- compile-safe runtime primitives
- live Gio runtime composition
- executable WebView runtime model
- increasing automated coverage

This makes the Go path a stable verification anchor while the C++ tree is migrated cautiously.

## Guardrails for Side-by-Side Compilation
Every migration bucket should satisfy the following before merge:
1. Go track still passes:
   - `go test ./internal/...`
   - `go build -buildvcs=false .`
2. C++ bucket changes are isolated and intentional
3. No giant unrelated untracked rename tree is staged
4. Compatibility story is written down
5. The bucket is reversible if needed

## Immediate Next Safe Actions
1. Keep the Go track verified and moving.
2. Treat the C++ rename explosion as a cleanup/migration project, not as incidental noise to stage.
3. Introduce BobUI-facing compatibility shims before deep source churn.
4. Create explicit bucket docs before touching broad C++ rename surfaces.

## Recommended First Concrete C++ Migration Work
1. classify untracked rename artifacts as canonical vs duplicate vs generated
2. identify one safe module/package/export bucket
3. add compatibility aliases/shims
4. verify that bucket in isolation
5. commit only that bucket

## Bottom Line
The right path to “both the C++ and Go versions compile cleanly side by side” is:
- **not** a broad global rename
- **yes** to staged compatibility-first migration
- **yes** to explicit bucket planning
- **yes** to preserving verified Go progress while cleaning the C++ track carefully

That path is slower than a search-and-replace, but it is the one that actually converges toward a working repository instead of a broken one.
