# Managed Window Interaction and Content

## Date
2026-04-05

## Scope
This note documents the next runtime-composition step after basic window-manager integration.

Affected files:
- `internal/ui/widgets/tabbar.go`
- `internal/ui/widgets/window.go`
- `internal/ui/wm.go`
- `internal/ui/engine.go`

## Goal
Make managed windows and tabs more meaningful by adding:
- clickable tab selection
- tab-scoped window visibility
- richer managed-window content

## What changed
### 1. `TabBar` is now interactive
The Go `TabBar` baseline now uses real clickable buttons and supports:
- active tab selection by click
- active title lookup
- programmatic click support for deterministic tests

### 2. `Window` now carries tab/content semantics
Managed windows now have:
- `Tab`
- `Body`

The window baseline also renders richer title/body metadata so managed windows are no longer only colored rectangles.

### 3. `WindowManager` now filters visible windows by active tab
The manager now exposes:
- `ActiveTab()`
- `VisibleWindows()`
- `SpawnManagedWindow(...)`

Windows without a tab remain shared; tab-bound windows are filtered against the currently active tab.

### 4. Engine runtime seeding is now tab-aware
The live runtime now seeds managed windows with:
- explicit tab ownership
- visible per-window body content

This makes the runtime composition feel much closer to an actual windowed framework baseline.

## Why this approach
This is the smallest meaningful interaction/content upgrade for managed windows:
- it makes tabs real
- it makes windows semantically richer
- it avoids pretending to have full docking/workspace complexity already

## Verified
```bash
go test ./internal/...
go build -buildvcs=false .
```

## Remaining gaps
- moving/resizing/closing managed windows
- richer tab docking behavior
- shared window layering rules
- managed-window content drawn from real runtime subsystems instead of seed strings
