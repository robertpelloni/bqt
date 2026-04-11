# Live Gio Frame Loop Integration

## Date
2026-04-05

## Scope
This note documents the shift from safe initialization-only behavior to a real Gio window/frame loop for the Go `bobui` runtime.

Affected files:
- `internal/ui/engine.go`

## Goal
Render the verified Go baseline inside a real Gio window so the current demo surface and interaction primitives can run in an actual frame loop.

## What changed
### 1. `Engine.Run()` now opens a Gio window
The engine now creates a real Gio window and applies basic options such as:
- title
- size

### 2. `Engine.Run()` now processes real window events
The engine now loops on window events and handles:
- `app.FrameEvent`
- `app.DestroyEvent`

### 3. Frame rendering now uses `app.NewContext(...)`
Each frame now:
- resets/draws through Gio ops
- creates a `layout.Context` from the frame event
- renders BobUI content
- submits the frame back to Gio

### 4. Login -> runtime surface flow
The live runtime now uses the existing BobUI components instead of a separate throwaway path:
- `LoginView` first
- then the runtime surface after authentication
- `DemoSurface` rendered inside the live frame loop
- `Shell` taskbar/start menu layered into the same runtime surface

### 5. Existing runtime hooks remain active
The engine still initializes and exercises:
- WebView bridge runtime handlers
- touch/swipe demo content
- scroll demo content

## Design notes
- This is intentionally a first live frame loop, not a final window manager/runtime architecture.
- The goal is to give the verified primitives a real rendering and input context.
- Existing compile-safe components were reused rather than replaced.

## Why this approach
A live frame loop dramatically increases the value of the previous work. Scrollbars, touch/swipe, and WebView runtime semantics are much more meaningful once they can be exercised in a real Gio window.

## Verified
```bash
go test ./internal/...
go build -buildvcs=false .
```

## Remaining gaps
- richer window/runtime composition
- explicit redraw/animation policies beyond event-driven interaction
- automated runtime tests
- deeper docking/window-manager integration

## Recommended next step
Add automated validation around WebView bridge/runtime behavior and consider richer runtime composition around the live Gio surface.
