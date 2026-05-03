include("${CMAKE_CURRENT_LIST_DIR}/BobUISupportedComponents.cmake")

if(NOT DEFINED OUTPUT_PATH OR "${OUTPUT_PATH}" STREQUAL "")
    set(OUTPUT_PATH
        "${CMAKE_CURRENT_LIST_DIR}/../docs/ai/design/2026-04-05-bobui-cmake-supported-component-status.md")
endif()

get_filename_component(_bobui_output_dir "${OUTPUT_PATH}" DIRECTORY)
file(MAKE_DIRECTORY "${_bobui_output_dir}")

set(_bobui_report_content [=[# BobUI CMake Supported Component Status

## Date
2026-04-05

## Purpose
Provide a compact human-readable status report for the currently supported additive BobUI CMake compatibility surface.

Authoritative source of truth:
- `cmake/BobUISupportedComponents.cmake`

This report is intended for review, handoff, and progress tracking. The manifest remains authoritative.

It is generated from the supported component manifest via:
- `cmake/bobui_generate_supported_component_report.cmake`

## Current support table

| Component | BobUI6 package | BobUI package | Primary targets | Umbrella routing | Direct package smoke | Publication smoke |
|---|---|---|---|---|---|---|
]=])

foreach(_component IN LISTS BOBUI_SUPPORTED_COMPONENTS)
    string(APPEND _bobui_report_content
        "| ${_component} | `BobUI6${_component}` | `BobUI${_component}` | `BobUI6::${_component}`, `BobUI::${_component}` | Yes | Yes | Yes |\n")
endforeach()

string(APPEND _bobui_report_content [=[

## Validation sources
- Manifest consistency: `cmake/tests/bobui_supported_component_manifest_consistency.cmake`
- Report consistency: `cmake/tests/bobui_supported_component_report_consistency.cmake`
- Umbrella/direct package routing: `cmake/tests/bobui_package_forwarding_smoke.cmake`
- Publication/build-install-style coverage: `cmake/tests/bobui_export_publication_configure_smoke.cmake`
- Native readiness gate: `cmake/tests/bobui_qtbase_native_configure_preflight.cmake`
- Go regression baseline:
  - `go test ./internal/...`
  - `go build -buildvcs=false .`

## Interpretation
This report means the additive BobUI CMake compatibility layer currently supports the components above with:
- explicit package names,
- target aliases,
- umbrella-package routing,
- direct package smoke coverage,
- publication smoke coverage.

It does **not** mean:
- the native C++ tree has been globally renamed,
- all qtbase modules are covered,
- full native qtbase configure/build is verified in the current environment.

## Maintenance rule
When a new supported component is added:
1. update `cmake/BobUISupportedComponents.cmake`,
2. add the matching BobUI6/BobUI config shims,
3. update smoke coverage,
4. regenerate this report,
5. rerun manifest/report consistency and smoke validation.
]=])

file(WRITE "${OUTPUT_PATH}" "${_bobui_report_content}")
message(STATUS "Generated BobUI supported component report: ${OUTPUT_PATH}")
