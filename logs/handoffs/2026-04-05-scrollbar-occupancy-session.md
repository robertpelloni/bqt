# Handoff Archive - 2026-04-05 - ScrollBar Occupancy / Two-Axis Session

## Summary
This session improved the Go scroll baseline beyond the earlier interactive primary-axis behavior by adding occupied-space scrollbar placement and lightweight two-axis state.

## What changed
- Reworked `internal/ui/widgets/scrollview.go` to support overlay vs occupy placement for vertical and horizontal scrollbars.
- Added occupied-space relayout logic so content can reserve room for visible scrollbars.
- Added lightweight secondary-axis viewport behavior using the non-primary scrollbar's normalized position/page state.
- Updated `internal/ui/widgets/demo_surface.go` to visibly exercise occupied vertical and horizontal scrollbar behavior and report live state.
- Added implementation/testing docs under `docs/ai/implementation/` and `docs/ai/testing/`.

## Validation
- `go test ./internal/...` passed.
- `go build -buildvcs=false .` succeeded.

## Key finding
A truthful two-axis scroll baseline does not require pretending the framework already has full 2D virtualization. Explicit secondary-axis viewport state is enough to make the current scroll container materially more realistic while preserving scope discipline.

## Recommended next steps
1. Wire the demo surface into a fuller Gio runtime loop.
2. Add automated validation around WebView bridge queueing/correlation/error paths.
3. Consider richer scroll corner/theming behavior once more live runtime wiring exists.
