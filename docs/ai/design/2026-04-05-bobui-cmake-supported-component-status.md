# BobUI CMake Supported Component Status

## Date
2026-04-05

## Purpose
Provide a compact human-readable status report for the currently supported additive BobUI CMake compatibility surface.

Authoritative source of truth:
- `cmake/BobUISupportedComponents.cmake`

This report is intended for review, handoff, and progress tracking. The manifest remains authoritative.

## Current support table

| Component | BobUI6 package | BobUI package | Primary targets | Umbrella routing | Direct package smoke | Publication smoke |
|---|---|---|---|---|---|---|
| Concurrent | `BobUI6Concurrent` | `BobUIConcurrent` | `BobUI6::Concurrent`, `BobUI::Concurrent` | Yes | Yes | Yes |
| Core | `BobUI6Core` | `BobUICore` | `BobUI6::Core`, `BobUI::Core` | Yes | Yes | Yes |
| DBus | `BobUI6DBus` | `BobUIDBus` | `BobUI6::DBus`, `BobUI::DBus` | Yes | Yes | Yes |
| Gui | `BobUI6Gui` | `BobUIGui` | `BobUI6::Gui`, `BobUI::Gui` | Yes | Yes | Yes |
| Network | `BobUI6Network` | `BobUINetwork` | `BobUI6::Network`, `BobUI::Network` | Yes | Yes | Yes |
| OpenGL | `BobUI6OpenGL` | `BobUIOpenGL` | `BobUI6::OpenGL`, `BobUI::OpenGL` | Yes | Yes | Yes |
| OpenGLWidgets | `BobUI6OpenGLWidgets` | `BobUIOpenGLWidgets` | `BobUI6::OpenGLWidgets`, `BobUI::OpenGLWidgets` | Yes | Yes | Yes |
| PrintSupport | `BobUI6PrintSupport` | `BobUIPrintSupport` | `BobUI6::PrintSupport`, `BobUI::PrintSupport` | Yes | Yes | Yes |
| Sql | `BobUI6Sql` | `BobUISql` | `BobUI6::Sql`, `BobUI::Sql` | Yes | Yes | Yes |
| Test | `BobUI6Test` | `BobUITest` | `BobUI6::Test`, `BobUI::Test` | Yes | Yes | Yes |
| Widgets | `BobUI6Widgets` | `BobUIWidgets` | `BobUI6::Widgets`, `BobUI::Widgets` | Yes | Yes | Yes |
| Xml | `BobUI6Xml` | `BobUIXml` | `BobUI6::Xml`, `BobUI::Xml` | Yes | Yes | Yes |

## Validation sources
- Manifest consistency: `cmake/tests/bobui_supported_component_manifest_consistency.cmake`
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
4. update this report,
5. rerun manifest consistency and smoke validation.
