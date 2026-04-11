# BobUI Supported Component Report Tests

## Date
2026-04-05

## Scope
This note documents validation for the human-readable BobUI supported component status report.

Affected files:
- `docs/ai/design/2026-04-05-bobui-cmake-supported-component-status.md`
- `cmake/tests/bobui_supported_component_report_consistency.cmake`

## Validation performed
Command:
```bash
cmake -P cmake/tests/bobui_supported_component_report_consistency.cmake
```

Verified:
- the support report exists,
- every manifest component has a table row,
- every manifest component includes:
  - `BobUI6<Component>`
  - `BobUI<Component>`
  - `BobUI6::<Component>`
  - `BobUI::<Component>`

## Why this is useful
The manifest consistency test ensures the machine-readable inventory stays aligned with the shim files and smoke tests.

This report consistency test ensures the human-readable status document stays aligned with that same inventory. Together they make the growing BobUI compatibility surface easier to review and harder to let drift.

## Verified alongside this step
```bash
cmake -P cmake/tests/bobui_supported_component_manifest_consistency.cmake
cmake -P cmake/tests/bobui_supported_component_report_consistency.cmake
cmake -P cmake/tests/bobui_qtbase_native_configure_preflight.cmake
cmake -P cmake/tests/bobui_compatibility_helpers_mapping.cmake
cmake -P cmake/tests/bobui_package_forwarding_smoke.cmake
cmake -P cmake/tests/bobui_export_publication_configure_smoke.cmake
go test ./internal/...
go build -buildvcs=false .
```

## Result
All validation passed in the current environment, with the native configure preflight still reporting the expected compiler-visibility skip.
