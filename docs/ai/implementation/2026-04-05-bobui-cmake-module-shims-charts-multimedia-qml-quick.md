# BobUI CMake Module Shims for Charts, Multimedia, Qml, and Quick

## Date
2026-04-05

## Scope
This note documents the next direct BobUI module-package compatibility slice after the Linguist/ShaderTools/Svg work.

Affected files:
- `cmake/BobUI6ChartsConfig.cmake`
- `cmake/BobUIChartsConfig.cmake`
- `cmake/BobUI6MultimediaConfig.cmake`
- `cmake/BobUIMultimediaConfig.cmake`
- `cmake/BobUI6QmlConfig.cmake`
- `cmake/BobUIQmlConfig.cmake`
- `cmake/BobUI6QuickConfig.cmake`
- `cmake/BobUIQuickConfig.cmake`
- `cmake/BobUISupportedComponents.cmake`

## Goal
Continue expanding additive BobUI package identity into high-value public modules that are critical for modern application development.

## What changed

### 1. Added direct Charts module shims
The repository now includes shims for `BobUI6Charts` and `BobUICharts`.

### 2. Added direct Multimedia module shims
The repository now includes shims for `BobUI6Multimedia` and `BobUIMultimedia`.

### 3. Added direct Qml module shims
The repository now includes shims for `BobUI6Qml` and `BobUIQml`.

### 4. Added direct Quick module shims
The repository now includes shims for `BobUI6Quick` and `BobUIQuick`.

## Why this matters
QML and Qt Quick are the foundation of modern Qt UI development. Providing `BobUI6Qml` and `BobUI6Quick` shims is a major milestone in the migration, as it allows developers to start referencing the BobUI namespace for the declarative part of the framework.

Multimedia and Charts are also frequently used in sophisticated applications, so including them continues to broaden the "ready for real world use" claim of the BobUI compatibility layer.

## Verified
```bash
cmake -P cmake/tests/bobui_full_compatibility_validation.cmake
go test ./internal/...
go build -buildvcs=false .
```

## Remaining gaps
- some specialized modules (e.g. `Positioning`, `Bluetooth`) are still missing if not present in the local tree,
- stronger native qtbase configure/install validation remains environment-gated,
- source-identifier renaming is still intentionally deferred.

## Recommended next step
Investigate "Bucket C" (Public Header Aliases) to allow C++ source code to use BobUI-branded includes like `#include <BobUICore/QObject>`.
