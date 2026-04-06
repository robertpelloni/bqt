# BobUI Full Compatibility Validation

## Date
2026-04-05

## Scope
This note documents the consolidated validation entry point for the additive BobUI CMake compatibility migration path.

Affected files:
- `cmake/tests/bobui_full_compatibility_validation.cmake`

## Goal
Provide a single repeatable command that runs the full BobUI CMake compatibility validation stack in the intended order.

## What it runs
The consolidated validation script runs:
1. `bobui_supported_component_manifest_consistency.cmake`
2. `bobui_supported_component_report_consistency.cmake`
3. `bobui_qtbase_native_configure_preflight.cmake`
4. `bobui_compatibility_helpers_mapping.cmake`
5. `bobui_package_forwarding_smoke.cmake`
6. `bobui_export_publication_configure_smoke.cmake`

## Why this helps
Before this step, the compatibility stack was verified, but the invocation was a manual checklist.

The new entry point makes it easier to:
- run the whole compatibility suite locally,
- wire the suite into future CI,
- reduce command drift between sessions,
- make handoffs more reliable.

## Command
```bash
cmake -P cmake/tests/bobui_full_compatibility_validation.cmake
```

## Interpretation
A passing run means:
- the supported component manifest is aligned,
- the generated report is aligned,
- the native configure preflight behaved as expected for the current environment,
- the helper mapping logic still works,
- umbrella/direct package forwarding still works,
- publication/build-install-style package generation still works.

This command complements, but does not replace, the verified Go baseline:
```bash
go test ./internal/...
go build -buildvcs=false .
```

## Recommended next use
Use this consolidated command as the default BobUI CMake compatibility validation step going forward, especially before future package-surface additions.
