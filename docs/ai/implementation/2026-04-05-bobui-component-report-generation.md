# BobUI Component Report Generation

## Date
2026-04-05

## Scope
This note documents the automation added for generating the human-readable BobUI supported component status report from the authoritative manifest.

Affected files:
- `cmake/bobui_generate_supported_component_report.cmake`
- `docs/ai/design/2026-04-05-bobui-cmake-supported-component-status.md`
- `cmake/tests/bobui_supported_component_report_consistency.cmake`

## Goal
Reduce drift between the machine-readable component manifest and the human-readable status report by generating the report from the manifest.

## What changed
### 1. Added report generator script
`cmake/bobui_generate_supported_component_report.cmake` now:
- includes `cmake/BobUISupportedComponents.cmake`,
- generates the supported component status report content from that manifest,
- writes to the checked-in design report by default,
- supports an `OUTPUT_PATH` override for test usage.

### 2. Shifted the report toward generated output
The status report is no longer just manually curated prose plus a table. It is now derived from the supported component manifest, which makes the package status surface easier to maintain as more components are added.

### 3. Strengthened the report consistency test
The report consistency validation can now compare generated output against the checked-in report, which is stronger than checking for a few expected substrings.

## Why this matters
The BobUI CMake compatibility surface now has:
- a machine-readable manifest,
- smoke tests for routing and publication,
- a human-readable support report,
- and generation logic tying the report back to the manifest.

That is a healthier migration posture than relying on manual documentation discipline alone.

## Verified
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
