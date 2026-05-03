# BobUI CMake Module Shims Tests for Sql and Xml

## Date
2026-04-05

## Scope
This note documents validation for the Sql and Xml BobUI module-package shims.

Affected files:
- `cmake/BobUI6SqlConfig.cmake`
- `cmake/BobUISqlConfig.cmake`
- `cmake/BobUI6XmlConfig.cmake`
- `cmake/BobUIXmlConfig.cmake`
- `cmake/tests/bobui_package_forwarding_smoke.cmake`
- `cmake/tests/bobui_export_publication_configure_smoke.cmake`

## Validation performed

### 1. Package forwarding smoke test
Command:
```bash
cmake -P cmake/tests/bobui_package_forwarding_smoke.cmake
```

Verified:
- existing BobUI umbrella/Core/Widgets/Gui/Network forwarding still works,
- direct `BobUI6Sql` / `BobUISql` package lookup works,
- direct `BobUI6Xml` / `BobUIXml` package lookup works,
- expected BobUI compatibility targets exist for Sql and Xml.

### 2. Publication configure smoke test
Command:
```bash
cmake -P cmake/tests/bobui_export_publication_configure_smoke.cmake
```

Verified:
- the publication helper emits Sql module-package compat files,
- the publication helper emits Xml module-package compat files,
- expected config/helper/version files exist in build-style and install-style directories.

### 3. Regression checks
Commands:
```bash
cmake -P cmake/tests/bobui_compatibility_helpers_mapping.cmake
go test ./internal/...
go build -buildvcs=false .
```

Verified:
- helper mapping remains stable,
- the verified Go baseline remains green.

## Result
All validation passed.

## Key finding
The BobUI CMake compatibility path now spans umbrella forwarding/publication plus direct module-package slices for Core, Gui, Network, Sql, Widgets, and Xml.
