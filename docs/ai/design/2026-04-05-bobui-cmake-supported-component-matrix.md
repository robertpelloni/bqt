# BobUI CMake Supported Component Matrix

## Date
2026-04-05

## Scope
This document records the currently supported BobUI-facing CMake component matrix for the additive compatibility migration path.

## Purpose
The repository now has enough additive BobUI package surface that the supported component set should be explicit instead of implied by scattered shim files.

This matrix is not a claim that the native C++ tree has been renamed wholesale.
It is a statement of which BobUI-facing package components are currently supported by the additive CMake compatibility layer.

## Current supported components
The centralized manifest is:
- `cmake/BobUISupportedComponents.cmake`

Current supported components:
- `Concurrent`
- `Core`
- `DBus`
- `Gui`
- `Network`
- `OpenGL`
- `OpenGLWidgets`
- `PrintSupport`
- `Sql`
- `Test`
- `Widgets`
- `Xml`

## Supported package forms
For each supported component `Foo`, the migration currently aims to provide:
- `BobUI6Foo`
- `BobUIFoo`
- `BobUI6::Foo`
- `BobUI::Foo`

In addition, the umbrella packages are:
- `BobUI6`
- `BobUI`

which can route supported components through `find_package(... COMPONENTS ...)`.

## Why this matters
This matrix clarifies three things:
1. what downstream package names are intentionally supported today,
2. what the smoke tests are expected to cover,
3. how far the additive BobUI package migration has progressed without pretending the entire native tree has already been renamed.

## Design principle
The matrix should grow only when all of the following are true:
- a BobUI6 component config shim exists,
- a versionless BobUI component config shim exists,
- forwarding smoke coverage exists,
- publication smoke coverage exists,
- the verified Go baseline remains green.

## Non-goals
This matrix does **not** imply:
- full native qtbase build validation in the current environment,
- deep source identifier renaming,
- plugin/internal/private package migration,
- repo-wide `qt` -> `bobui` replacement.

## Recommended evolution rule
Treat this matrix as the authoritative compatibility inventory for the additive CMake migration path. When a new component is added, update:
- the manifest,
- the corresponding shim files,
- forwarding/publication smoke tests,
- testing/session docs.
