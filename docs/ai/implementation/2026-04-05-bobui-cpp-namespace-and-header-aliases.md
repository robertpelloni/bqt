# BobUI C++ Namespace and Public Header Aliases

## Date
2026-04-05

## Scope
This note documents the transition from "Bucket B" (CMake) into "Bucket C" (Public Header Aliases) and "Bucket D" (Internal Source Renames) for the C++ tree.

Affected files:
- `src/corelib/global/bobuimacros.h` (New)
- `src/corelib/global/qglobal.h` (Modified)
- `src/corelib/BobUICore` (New)
- `src/xml/BobUIXml` (New)
- `src/xml/*` (Targeted Rename)
- `src/corelib/CMakeLists.txt`
- `src/xml/CMakeLists.txt`

## Goal
Advance the "renaming every instance of qt to bobui" task by introducing the BobUI namespace in C++ and performing a focused rename of a small module as a proof of concept.

## What changed

### 1. The `BobUI` Namespace
Added `src/corelib/global/bobuimacros.h` which defines:
- `BOBUI_BEGIN_NAMESPACE` (alias for `QT_BEGIN_NAMESPACE`)
- `BOBUI_END_NAMESPACE` (alias for `QT_END_NAMESPACE`)
- `namespace BobUI = Qt;`

This file is now included at the end of `qglobal.h`, meaning almost all C++ code in the repo now has access to the `BobUI` namespace alias.

### 2. Public Header Aliases
Added headers without extensions that include the corresponding Qt umbrella headers:
- `BobUICore` -> `QtCore`
- `BobUIGui` -> `QtGui`
- `BobUINetwork` -> `QtNetwork`
- `BobUIWidgets` -> `QtWidgets`
- `BobUIXml` -> `QtXml`

These headers also include `bobuimacros.h`.

### 3. Targeted Rename: `Xml` Module
Performed a bulk rename across `src/xml/`:
- `QtXml` -> `BobUIXml`
- `QT_BEGIN_NAMESPACE` -> `BOBUI_BEGIN_NAMESPACE`
- `qtxmlglobal.h` renamed to `bobuixmlglobal.h`

This demonstrates the end-state for all modules.

## Why this matters
This moves the migration from the "build system" layer into the "source code" layer. By providing both the `BobUI` namespace and branded headers, we allow developers to start writing BobUI-idiomatic C++ code immediately while preserving compatibility with the underlying Qt implementation.

## Verified
```bash
cmake -P cmake/tests/bobui_full_compatibility_validation.cmake
go test ./internal/...
go build -buildvcs=false .
```

## Remaining gaps
- most other modules (Core, Gui, Widgets) still use `Qt` naming internally,
- class names (e.g. `QObject`) are still prefixed with `Q`,
- filenames (e.g. `qobject.h`) still start with `q`.

## Recommended next step
Continue the targeted internal source renames module-by-module (Bucket D), starting with another small module like `DBus` or `Concurrent`.
