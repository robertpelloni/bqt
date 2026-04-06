# BobUI Umbrella Component Matrix Tests

## Date
2026-04-05

## Scope
This note documents stronger validation for the BobUI umbrella package shims after multiple direct module-package slices were added.

Affected files:
- `cmake/tests/bobui_package_forwarding_smoke.cmake`

## Goal
Verify that the additive BobUI umbrella packages can forward a broader supported component set through one `find_package(...)` call, rather than only proving a tiny minimal component subset.

## What changed
The umbrella forwarding smoke test now validates:

```cmake
find_package(BobUI6 6.12 REQUIRED CONFIG COMPONENTS
    Concurrent Core DBus Gui Network OpenGL OpenGLWidgets PrintSupport Sql Test Widgets Xml)
find_package(BobUI REQUIRED CONFIG COMPONENTS
    Concurrent Core DBus Gui Network OpenGL OpenGLWidgets PrintSupport Sql Test Widgets Xml)
```

It then confirms that the expected BobUI compatibility targets exist for the routed component set.

## Why this matters
Earlier validation showed that:
- top-level BobUI package forwarding worked,
- direct module-package slices worked.

This stronger test now proves those two ideas compose together: the umbrella BobUI packages can request a meaningful multi-module surface at once.

That is a more realistic downstream consumption pattern than testing only `Core` and `Widgets` through the umbrella package.

## Verified
```bash
cmake -P cmake/tests/bobui_qtbase_native_configure_preflight.cmake
cmake -P cmake/tests/bobui_compatibility_helpers_mapping.cmake
cmake -P cmake/tests/bobui_package_forwarding_smoke.cmake
cmake -P cmake/tests/bobui_export_publication_configure_smoke.cmake
go test ./internal/...
go build -buildvcs=false .
```

## Result
All validation passed in the current environment, with the native configure preflight still reporting the expected compiler-visibility skip.

## Key finding
The BobUI compatibility path now has a stronger umbrella-package story: a broad supported component set can be requested through the BobUI umbrella packages while preserving the additive compatibility-first model.
