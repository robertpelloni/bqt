# BobUI Supported Component Manifest Tests

## Date
2026-04-05

## Scope
This note documents validation for the centralized BobUI supported component manifest.

Affected files:
- `cmake/BobUISupportedComponents.cmake`
- `cmake/tests/bobui_supported_component_manifest_consistency.cmake`

## Validation performed
Command:
```bash
cmake -P cmake/tests/bobui_supported_component_manifest_consistency.cmake
```

Verified:
- the centralized supported component list is non-empty,
- every supported component has both a `BobUI6<Component>Config.cmake` and `BobUI<Component>Config.cmake`,
- the forwarding smoke test references each supported component through:
  - direct package lookup,
  - `BobUI6::<Component>` target validation,
  - `BobUI::<Component>` target validation,
- the publication smoke test references each supported component package.

## Why this is useful
The manifest consistency test makes the growing BobUI package matrix less fragile. It helps ensure that supported components, shim files, and smoke coverage stay aligned as the additive migration expands.

## Verified alongside this step
```bash
cmake -P cmake/tests/bobui_supported_component_manifest_consistency.cmake
cmake -P cmake/tests/bobui_qtbase_native_configure_preflight.cmake
cmake -P cmake/tests/bobui_compatibility_helpers_mapping.cmake
cmake -P cmake/tests/bobui_package_forwarding_smoke.cmake
cmake -P cmake/tests/bobui_export_publication_configure_smoke.cmake
go test ./internal/...
go build -buildvcs=false .
```

## Result
All validation passed in the current environment, with the native configure preflight still reporting the expected compiler-visibility skip.
