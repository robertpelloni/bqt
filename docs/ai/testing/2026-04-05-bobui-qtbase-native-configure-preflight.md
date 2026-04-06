# BobUI qtbase Native Configure Preflight

## Date
2026-04-05

## Scope
This note documents the first repeatable preflight step for stronger qtbase-native configure validation.

Affected files:
- `cmake/tests/bobui_qtbase_native_configure_preflight.cmake`

## Goal
Turn the current environment-dependent qtbase-native configure attempt into an explicit, testable preflight gate instead of relying on manual trial and error.

## Why this was needed
A direct top-level configure attempt showed:
- CMake selected a native Visual Studio generator,
- but no usable C or C++ compiler was visible in the current environment,
- so top-level qtbase configure could not complete.

Observed failure shape:
- `No CMAKE_CXX_COMPILER could be found`
- `No CMAKE_C_COMPILER could be found`

This is an environment/toolchain visibility problem, not a reason to do a broad unsafe rename.

## What the preflight does
`cmake/tests/bobui_qtbase_native_configure_preflight.cmake`:
1. honors `BOBUI_SKIP_NATIVE_CONFIGURE=ON` by reporting a clean explicit skip,
2. otherwise searches `PATH` for plausible C/C++ compilers,
3. if none are visible, reports a clean skip with an explanatory message,
4. if compilers are visible, attempts a conservative top-level configure using:
   - `QT_BUILD_TESTS=OFF`
   - `QT_BUILD_EXAMPLES=OFF`
   - `BUILD_SHARED_LIBS=OFF`
4. removes the temporary build directory afterward.

## Why this is useful
This gives the migration path an honest native-validation checkpoint:
- it distinguishes environment/toolchain problems from project-logic problems,
- it is safe to run repeatedly,
- it prevents people from misreading a missing compiler as evidence that the BobUI package-surface work is broken.

## Result in the current environment
The preflight reports a skip because no usable native compiler is visible in `PATH`.

In CI, the consolidated BobUI compatibility command currently passes `BOBUI_SKIP_NATIVE_CONFIGURE=ON` to avoid turning environment/toolchain provisioning differences into false negatives.

That means:
- the stronger native qtbase configure step is **not yet green here**,
- but the reason is now explicit and reproducible,
- and the additive BobUI CMake migration work remains separately validated by the existing smoke tests.

## Verified alongside this step
```bash
cmake -P cmake/tests/bobui_qtbase_native_configure_preflight.cmake
cmake -P cmake/tests/bobui_compatibility_helpers_mapping.cmake
cmake -P cmake/tests/bobui_package_forwarding_smoke.cmake
cmake -P cmake/tests/bobui_export_publication_configure_smoke.cmake
go test ./internal/...
go build -buildvcs=false .
```

## Recommended next step
When a usable native compiler environment is available, rerun the preflight. If it advances past the compiler-discovery gate, the resulting top-level configure outcome will be much more informative for the next C++ migration step.
