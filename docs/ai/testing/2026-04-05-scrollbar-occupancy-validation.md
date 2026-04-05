# ScrollBar Occupancy and Two-Axis Validation

## Date
2026-04-05

## Scope
Validate the occupied-space and two-axis scroll polish:
- `internal/ui/widgets/scrollview.go`
- `internal/ui/widgets/demo_surface.go`

## Commands
```bash
go test ./internal/...
go build -buildvcs=false .
```

## Result
Both commands succeeded.

## Validated baseline outcomes
- `ScrollView` compiles with overlay/occupy placement modes.
- `ScrollView` compiles with occupied-space relayout logic.
- Secondary-axis viewport state compiles and is surfaced through the horizontal/vertical scrollbar pair.
- `DemoSurface` compiles while exercising occupied vertical and horizontal scrollbar behavior.

## Current limitations
This validation is compile/build focused and does not yet include automated assertions around occupied-space layout geometry or cross-axis drag behavior.
