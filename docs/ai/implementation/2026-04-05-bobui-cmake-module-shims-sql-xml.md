# BobUI CMake Module Shims for Sql and Xml

## Date
2026-04-05

## Scope
This note documents the next direct BobUI module-package compatibility slice after the Gui/Network work.

Affected files:
- `cmake/BobUI6SqlConfig.cmake`
- `cmake/BobUISqlConfig.cmake`
- `cmake/BobUI6XmlConfig.cmake`
- `cmake/BobUIXmlConfig.cmake`
- `cmake/tests/bobui_package_forwarding_smoke.cmake`
- `cmake/tests/bobui_export_publication_configure_smoke.cmake`

## Goal
Continue expanding the additive BobUI module-package surface through small, verified, consumer-facing package slices instead of broad source renaming.

## What changed

### 1. Added direct Sql module shims
The repository now includes:
- `BobUI6Sql`
- `BobUISql`

These forward to:
- `Qt6Sql`

and expose:
- `BobUI6::Sql`
- `BobUI::Sql`

### 2. Added direct Xml module shims
The repository now includes:
- `BobUI6Xml`
- `BobUIXml`

These forward to:
- `Qt6Xml`

and expose:
- `BobUI6::Xml`
- `BobUI::Xml`

### 3. Extended smoke tests
The forwarding smoke test now validates direct package lookup for:
- `BobUI6Sql`
- `BobUISql`
- `BobUI6Xml`
- `BobUIXml`

The publication configure smoke test now validates that the publication helper can emit:
- Sql module-package compat files
- Xml module-package compat files

in both build-style and install-style package directories.

## Why this matters
Sql and Xml are useful public package surfaces that deepen the BobUI naming story without forcing deep runtime churn.

This continues proving that BobUI identity can expand through real consumer-facing package compatibility instead of dangerous whole-repo rename pressure.

## Verified
```bash
cmake -P cmake/tests/bobui_compatibility_helpers_mapping.cmake
cmake -P cmake/tests/bobui_package_forwarding_smoke.cmake
cmake -P cmake/tests/bobui_export_publication_configure_smoke.cmake
go test ./internal/...
go build -buildvcs=false .
```

## Remaining gaps
- direct BobUI module-package coverage is still selective,
- stronger native qtbase configure/install validation remains environment-gated,
- source-identifier renaming is still intentionally deferred.

## Recommended next step
Continue expanding only high-value public module packages in small validated slices, or re-run the qtbase-native configure preflight in a compiler-capable environment.
